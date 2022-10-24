/**
 * @file HM_Global_Status_Processor.cpp
 * @author Van C. Ngo
 * @brief This file contains the global status processor
 * @version 0.1
 * @date 2022-04-09
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include "HM_Global_Status_Processor.h"
#include "HM_Cfg_Handler.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>


const std::map<hm_global_status_t, std::string> global_status_to_string_map = {
    {hm_global_status_t::hm_global_status_ok, "OK"},
    {hm_global_status_t::hm_global_status_failed, "FAILED"},
    {hm_global_status_t::hm_global_status_expired, "EXPIRED"},
    {hm_global_status_t::hm_global_status_stopped, "STOPPED"},
    {hm_global_status_t::hm_global_status_deactivated, "DEACTIVATED"}};

HMGlobalStatusProcessor::HMGlobalStatusProcessor(std::shared_ptr<HMCPDataIntf> checkpoint_data,
                                                 std::shared_ptr<HMHSDataIntf> health_status_data,
                                                 std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data,
                                                 std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data)
{
    checkpoint_data_ = std::move(checkpoint_data);
    health_status_data_ = std::move(health_status_data);
    supervised_entity_data_ = std::move(supervised_entity_data);
    global_supervision_data_ = std::move(global_supervision_data);
    is_valid_supervision_data = (checkpoint_data_ != nullptr) &&
        (health_status_data_ != nullptr) &&
        (supervised_entity_data_ != nullptr) &&
        (global_supervision_data_ != nullptr);
    // Initialize the function map
    update_status_func_map_[hm_global_status_t::hm_global_status_deactivated] =
        std::bind(&HMGlobalStatusProcessor::update_global_status_deactivated, this, std::placeholders::_1);
    update_status_func_map_[hm_global_status_t::hm_global_status_ok] =
        std::bind(&HMGlobalStatusProcessor::update_global_status_ok, this, std::placeholders::_1);
    update_status_func_map_[hm_global_status_t::hm_global_status_failed] =
        std::bind(&HMGlobalStatusProcessor::update_global_status_failed, this, std::placeholders::_1);
    update_status_func_map_[hm_global_status_t::hm_global_status_expired] =
        std::bind(&HMGlobalStatusProcessor::update_global_status_expired, this, std::placeholders::_1);
    update_status_func_map_[hm_global_status_t::hm_global_status_stopped] =
        std::bind(&HMGlobalStatusProcessor::update_global_status_stopped, this, std::placeholders::_1);
}

bool HMGlobalStatusProcessor::process_global_status()
{
    bool result{false};
    hm_mode_t active_mode_id{0u};
    std::shared_ptr<const HMModeCfgHandlerIntf> active_mode_cfg{nullptr};
    if (is_valid_supervision_data)
    {
        result = global_supervision_data_->get_active_mode_id(active_mode_id);
        active_mode_cfg = HMCfgHandler::get_mode_cfg_handler(active_mode_id);
        result &= (active_mode_cfg != nullptr);
    }
    if (result)
    {
        // Process global status
        hm_global_status_t current_global_status{hm_global_status_t::hm_global_status_deactivated};
        if (global_supervision_data_->get_global_status(current_global_status))
        {
            result &= update_status_func_map_[current_global_status](active_mode_cfg);
            // Print out for testing
            print_global_status();
            // Reset the local failed and expired status
            global_supervision_data_->set_num_local_status_failed(0u);
            global_supervision_data_->set_num_local_status_expired(0u);
        }
    }
    return result;
}

bool HMGlobalStatusProcessor::update_global_status_deactivated(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg)
{
    // No processing
    return true;
}

bool HMGlobalStatusProcessor::update_global_status_stopped(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg)
{
    // No processing, transitions from here are done by other components
    return true;
}

bool HMGlobalStatusProcessor::update_global_status_ok(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg)
{
    bool result{false};
    hm_global_status_t new_global_status{hm_global_status_t::hm_global_status_ok};
    const auto expired_sup_cycle_tolerance = active_mode_cfg->get_expired_supervision_cycle_tolerance();
    uint32_t se_failed_count{0u};
    uint32_t se_expired_count{0u};
    result = global_supervision_data_->get_num_local_status_failed(se_failed_count) &&
        global_supervision_data_->get_num_local_status_expired(se_expired_count);
    if (result)
    {
        if (se_expired_count > 0u)
        {
            if (expired_sup_cycle_tolerance == 0u)
            {
                new_global_status = hm_global_status_t::hm_global_status_stopped;
            }
            else
            {
                new_global_status = hm_global_status_t::hm_global_status_expired;
                result &= global_supervision_data_->increase_global_status_expired_count();
            }
        }
        else if (se_failed_count > 0u)
        {
            new_global_status = hm_global_status_t::hm_global_status_failed;
        }
        // Update the new status
        result &= global_supervision_data_->set_global_status(new_global_status);
    }
    return result;
}

bool HMGlobalStatusProcessor::update_global_status_failed(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg)
{
    bool result{false};
    hm_global_status_t new_global_status{hm_global_status_t::hm_global_status_failed};
    const auto expired_sup_cycle_tolerance = active_mode_cfg->get_expired_supervision_cycle_tolerance();
    uint32_t se_failed_count{0u};
    uint32_t se_expired_count{0u};
    result = global_supervision_data_->get_num_local_status_failed(se_failed_count) &&
        global_supervision_data_->get_num_local_status_expired(se_expired_count);
    if (result)
    {
        if (se_expired_count > 0u)
        {
            if (expired_sup_cycle_tolerance == 0u)
            {
                new_global_status = hm_global_status_t::hm_global_status_stopped;
            }
            else
            {
                new_global_status = hm_global_status_t::hm_global_status_expired;
                result &= global_supervision_data_->increase_global_status_expired_count();
            }
        }
        else if (se_failed_count == 0u)
        {
            new_global_status = hm_global_status_t::hm_global_status_ok;
        }
        // Update the new status
        result &= global_supervision_data_->set_global_status(new_global_status);
    }
    return result;
}

bool HMGlobalStatusProcessor::update_global_status_expired(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg)
{
    bool result{false};
    hm_global_status_t new_global_status{hm_global_status_t::hm_global_status_expired};
    const auto expired_sup_cycle_tolerance = active_mode_cfg->get_expired_supervision_cycle_tolerance();
    uint32_t se_failed_count{0u};
    uint32_t se_expired_count{0u};
    uint32_t expired_status_count{0u};
    result = global_supervision_data_->get_num_local_status_failed(se_failed_count) &&
        global_supervision_data_->get_num_local_status_expired(se_expired_count) &&
        global_supervision_data_->get_global_status_expired_count(expired_status_count);
    if (result)
    {
        if (se_expired_count > 0u)
        {
            if (expired_sup_cycle_tolerance > expired_status_count)
            {
                result &= global_supervision_data_->increase_global_status_expired_count();
            }
            else
            {
                new_global_status = hm_global_status_t::hm_global_status_stopped;
                result &= global_supervision_data_->set_global_status_expired_count(0u);
            }
        }
        // Update the new status
        result &= global_supervision_data_->set_global_status(new_global_status);
    }
    return result;
}

void HMGlobalStatusProcessor::print_global_status()
{
    uint32_t se_failed_count{0u};
    global_supervision_data_->get_num_local_status_failed(se_failed_count);
    uint32_t se_expired_count{0u};
    global_supervision_data_->get_num_local_status_expired(se_expired_count);
    hm_global_status_t global_status{hm_global_status_t::hm_global_status_ok};
    global_supervision_data_->get_global_status(global_status);
    std::cout << "[HMGlobalStatusProcessor] Failed count : " << se_failed_count
              << " Expired count : " << se_expired_count
              << " Global status : " << global_status_to_string(global_status)<< std::endl;
}

const std::string& HMGlobalStatusProcessor::global_status_to_string(const hm_global_status_t status)
{
    return global_status_to_string_map.at(status);
}
