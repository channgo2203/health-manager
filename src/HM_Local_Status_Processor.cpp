/**
 * @file HM_Local_Status_Processor.cpp
 * @author Van C. Ngo
 * @brief This file contains the local status processor
 * @version 0.1
 * @date 2022-03-23
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include "HM_Local_Status_Processor.h"
#include "HM_Cfg_Handler.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>


const std::map<hm_local_status_t, std::string> local_status_to_string_map = {
    {hm_local_status_t::hm_local_status_ok, "OK"},
    {hm_local_status_t::hm_local_status_failed, "FAILED"},
    {hm_local_status_t::hm_local_status_expired, "EXPIRED"},
    {hm_local_status_t::hm_local_status_restarted, "RESTARTED"},
    {hm_local_status_t::hm_local_status_recovered, "RECOVERED"},
    {hm_local_status_t::hm_local_status_backup, "BACKUP"},
    {hm_local_status_t::hm_local_status_deactivated, "DEACTIVATED"}};

HMLocalStatusProcessor::HMLocalStatusProcessor(std::shared_ptr<HMCPDataIntf> checkpoint_data,
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
    update_status_func_map_[hm_local_status_t::hm_local_status_deactivated] =
        std::bind(&HMLocalStatusProcessor::update_se_local_status_deactivated, this, std::placeholders::_1);
    update_status_func_map_[hm_local_status_t::hm_local_status_ok] =
        std::bind(&HMLocalStatusProcessor::update_se_local_status_ok, this, std::placeholders::_1);
    update_status_func_map_[hm_local_status_t::hm_local_status_failed] =
        std::bind(&HMLocalStatusProcessor::update_se_local_status_failed, this, std::placeholders::_1);
    update_status_func_map_[hm_local_status_t::hm_local_status_expired] =
        std::bind(&HMLocalStatusProcessor::update_se_local_status_expired, this, std::placeholders::_1);
    update_status_func_map_[hm_local_status_t::hm_local_status_restarted] =
        std::bind(&HMLocalStatusProcessor::update_se_local_status_restarted, this, std::placeholders::_1);
    update_status_func_map_[hm_local_status_t::hm_local_status_recovered] =
        std::bind(&HMLocalStatusProcessor::update_se_local_status_recovered, this, std::placeholders::_1);
    update_status_func_map_[hm_local_status_t::hm_local_status_backup] =
        std::bind(&HMLocalStatusProcessor::update_se_local_status_backup, this, std::placeholders::_1);
}

bool HMLocalStatusProcessor::process_local_status()
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
        // Compute alive supervision results
        result &= compute_se_alive_supervision_results(active_mode_cfg);
        // Process local status for all SEs
        const auto& se_local_params = active_mode_cfg->get_local_supervision_params();
        for (const auto& se_lp : se_local_params)
        {
            result &= update_se_local_status(se_lp);
        }
        // Process local health status for all ESs
        const auto& hs_sup_params = active_mode_cfg->get_hs_supervision_params();
        for (const auto& hs_sp : hs_sup_params)
        {
            result &= update_es_local_health_status(hs_sp);
        }
    }
    return result;
}

bool HMLocalStatusProcessor::compute_se_alive_supervision_results(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg)
{
    bool result{true};
    const auto& alive_sups = active_mode_cfg->get_alive_supervisions();
    hm_cp_supervision_result_t sup_result{hm_cp_supervision_result_t::hm_cp_supervision_correct};
    const auto supervision_cycle_time = active_mode_cfg->get_supervision_cycle_time();
    for (const auto& sup : alive_sups)
    {
        const auto se_id = sup->get_se_id();
        const auto& cp_ids = sup->get_checkpoint_ids();
        uint32_t cycle_total{0u};
        const auto& sup_params = sup->get_alive_supervision_params();
        result &= checkpoint_data_->increase_supervision_cycle_total(se_id, cp_ids[0]);
        result &= checkpoint_data_->get_supervision_cycle_total(se_id, cp_ids[0], cycle_total);
        if (cycle_total >= sup_params.supervision_ref_cycles)
        {
            std::chrono::time_point<std::chrono::steady_clock> sup_time_point{};
            result &= checkpoint_data_->get_supervision_time_point(se_id, cp_ids[0], sup_time_point);
            result &= checkpoint_data_->set_supervision_time_total(se_id,
                                                                   cp_ids[0],
                                                                   std::chrono::duration_cast<std::chrono::milliseconds>
                                                                   (std::chrono::steady_clock::now() - sup_time_point).count());
            result &= sup->compute_supervision_result(checkpoint_data_, sup_result, supervision_cycle_time);
            if (result)
            {
                if (sup_result == hm_cp_supervision_result_t::hm_cp_supervision_incorrect)
                {
                    result &= supervised_entity_data_->increase_failed_alive_supervision_count(se_id);
                }
                // Clear counts
                result &= checkpoint_data_->set_supervision_cycle_total(se_id, cp_ids[0], 0u);
                result &= checkpoint_data_->set_alive_indications(se_id, cp_ids[0], 0u);
                result &= checkpoint_data_->set_supervision_time_point(se_id, cp_ids[0], std::chrono::steady_clock::now());
            }
        }
    }
    return result;
}

bool HMLocalStatusProcessor::update_se_local_status(const hm_local_status_param_t& se_local_params)
{
    bool result{false};
    hm_local_status_t current_local_status{hm_local_status_t::hm_local_status_deactivated};
    if (supervised_entity_data_->get_local_status(se_local_params.se_id, current_local_status))
    {
        result = update_status_func_map_[current_local_status](se_local_params);
        // Print out for testing
        print_local_status(se_local_params.se_id);
        // Clear supervision data
        supervised_entity_data_->set_failed_alive_supervision_count(se_local_params.se_id, 0u);
        supervised_entity_data_->set_failed_deadline_supervision_count(se_local_params.se_id, 0u);
        supervised_entity_data_->set_failed_logical_supervision_count(se_local_params.se_id, 0u);
    }
    return result;
}

bool HMLocalStatusProcessor::update_es_local_health_status(const hm_hs_supervision_param_t& hs_sup_params)
{
    bool result{true};
    // [TODO] Process health status local status
    return result;
}

bool HMLocalStatusProcessor::update_se_local_status_deactivated(const hm_local_status_param_t& se_local_params)
{
    // No processing, transitions from/to here are done by other components
    return true;
}

bool HMLocalStatusProcessor::update_se_local_status_expired(const hm_local_status_param_t& se_local_params)
{
    // No processing, transitions from here are done by other components
    return global_supervision_data_->increase_num_local_status_expired();
}

bool HMLocalStatusProcessor::update_se_local_status_ok(const hm_local_status_param_t& se_local_params)
{
    bool result{false};
    hm_local_status_t new_local_status{hm_local_status_t::hm_local_status_ok};
    uint32_t failed_deadline_count{0u};
    uint32_t failed_logical_count{0u};
    uint32_t failed_alive_count{0u};
    result = (supervised_entity_data_->get_failed_deadline_supervision_count(se_local_params.se_id, failed_deadline_count)) &&
        (supervised_entity_data_->get_failed_logical_supervision_count(se_local_params.se_id, failed_logical_count)) &&
        (supervised_entity_data_->get_failed_alive_supervision_count(se_local_params.se_id, failed_alive_count));
    if (result)
    {
        if (failed_deadline_count > 0u)
        {
            new_local_status = hm_local_status_t::hm_local_status_expired;
            result &= global_supervision_data_->increase_num_local_status_expired();
        }
        else if (failed_logical_count > 0u)
        {
            new_local_status = hm_local_status_t::hm_local_status_expired;
            result &= global_supervision_data_->increase_num_local_status_expired();
        }
        else if (failed_alive_count > 0u)
        {
            if (se_local_params.failure_alive_supervision_cycle_tolerance == 0u)
            {
                new_local_status = hm_local_status_t::hm_local_status_expired;
                result &= global_supervision_data_->increase_num_local_status_expired();
            }
            else
            {
                new_local_status = hm_local_status_t::hm_local_status_failed;
                result &= supervised_entity_data_->increase_failed_alive_supervision_cycles(se_local_params.se_id);
                result &= global_supervision_data_->increase_num_local_status_failed();
            }
        }
        // Update the new status
        result &= supervised_entity_data_->set_local_status(se_local_params.se_id, new_local_status);
    }
    return result;
}

bool HMLocalStatusProcessor::update_se_local_status_failed(const hm_local_status_param_t& se_local_params)
{
    bool result{false};
    hm_local_status_t new_local_status{hm_local_status_t::hm_local_status_failed};
    uint32_t failed_deadline_count{0u};
    uint32_t failed_logical_count{0u};
    uint32_t failed_alive_count{0u};
    uint32_t failed_alive_sup_cycles{0u};
    result = (supervised_entity_data_->get_failed_deadline_supervision_count(se_local_params.se_id, failed_deadline_count)) &&
        (supervised_entity_data_->get_failed_logical_supervision_count(se_local_params.se_id, failed_logical_count)) &&
        (supervised_entity_data_->get_failed_alive_supervision_count(se_local_params.se_id, failed_alive_count)) &&
        (supervised_entity_data_->get_failed_alive_supervision_cycles(se_local_params.se_id, failed_alive_sup_cycles));
    if (result)
    {
        if (failed_deadline_count > 0u)
        {
            new_local_status = hm_local_status_t::hm_local_status_expired;
            result &= global_supervision_data_->increase_num_local_status_expired();
        }
        else if (failed_logical_count > 0u)
        {
            new_local_status = hm_local_status_t::hm_local_status_expired;
            result &= global_supervision_data_->increase_num_local_status_expired();
        }
        else if (failed_alive_count == 0u)
        {
            const auto dec_cycles = failed_alive_sup_cycles - 1u;
            result &= supervised_entity_data_->set_failed_alive_supervision_cycles(se_local_params.se_id, dec_cycles);
            if (dec_cycles == 0u)
            {
                new_local_status = hm_local_status_t::hm_local_status_ok;
            }
            else
            {
                result &= global_supervision_data_->increase_num_local_status_failed();
            }
        }
        else if (failed_alive_count > 0u)
        {
            if (se_local_params.failure_alive_supervision_cycle_tolerance > failed_alive_sup_cycles)
            {
                result &= supervised_entity_data_->increase_failed_alive_supervision_cycles(se_local_params.se_id);
                result &= global_supervision_data_->increase_num_local_status_failed();
            }
            else
            {
                new_local_status = hm_local_status_t::hm_local_status_expired;
                result &= global_supervision_data_->increase_num_local_status_expired();
                result &= supervised_entity_data_->set_failed_alive_supervision_cycles(se_local_params.se_id, 0u);
            }
        }
        // Update the new status
        result &= supervised_entity_data_->set_local_status(se_local_params.se_id, new_local_status);
    }
    return result;
}

bool HMLocalStatusProcessor::update_se_local_status_restarted(const hm_local_status_param_t& se_local_params)
{
    bool result{false};
    hm_local_status_t new_local_status{hm_local_status_t::hm_local_status_restarted};
    uint32_t failed_deadline_count{0u};
    uint32_t failed_logical_count{0u};
    uint32_t failed_alive_count{0u};
    uint32_t restarted_cycle_count{0u};
    result = (supervised_entity_data_->get_failed_deadline_supervision_count(se_local_params.se_id, failed_deadline_count)) &&
        (supervised_entity_data_->get_failed_logical_supervision_count(se_local_params.se_id, failed_logical_count)) &&
        (supervised_entity_data_->get_failed_alive_supervision_count(se_local_params.se_id, failed_alive_count)) &&
        (supervised_entity_data_->get_restarted_cycle_count(se_local_params.se_id, restarted_cycle_count));
    if (result)
    {
        if ((failed_deadline_count > 0u) || (failed_logical_count > 0u) || (failed_alive_count > 0u))
        {
            const auto restarting_failed = (se_local_params.restarted_supervision_cycle_tolerance == 0u) ||
                (se_local_params.restarted_supervision_cycle_tolerance <= restarted_cycle_count);
            if (restarting_failed)
            {
                new_local_status = hm_local_status_t::hm_local_status_expired;
                result &= global_supervision_data_->increase_num_local_status_expired();
                result &= supervised_entity_data_->set_restarted_cycle_count(se_local_params.se_id, 0u);
            }
            else
            {
                new_local_status = hm_local_status_t::hm_local_status_restarted;
                result &= supervised_entity_data_->increase_restarted_cycle_count(se_local_params.se_id);
            }
        }
        else
        {
            const auto dec_cycles = restarted_cycle_count - 1u;
            const auto restarting_ok = (se_local_params.restarted_supervision_cycle_tolerance == 0u) ||
                (se_local_params.restarted_supervision_cycle_tolerance > dec_cycles);
            if (restarting_ok)
            {
                new_local_status = hm_local_status_t::hm_local_status_ok;

            }
            else
            {
                new_local_status = hm_local_status_t::hm_local_status_expired;
                result &= global_supervision_data_->increase_num_local_status_expired();
            }
            result &= supervised_entity_data_->set_restarted_cycle_count(se_local_params.se_id, 0u);
        }
        // Update the new status
        result &= supervised_entity_data_->set_local_status(se_local_params.se_id, new_local_status);
    }
    return result;
}

bool HMLocalStatusProcessor::update_se_local_status_recovered(const hm_local_status_param_t& se_local_params)
{
    bool result{false};
    hm_local_status_t new_local_status{hm_local_status_t::hm_local_status_recovered};
    uint32_t failed_deadline_count{0u};
    uint32_t failed_logical_count{0u};
    uint32_t failed_alive_count{0u};
    result = (supervised_entity_data_->get_failed_deadline_supervision_count(se_local_params.se_id, failed_deadline_count)) &&
        (supervised_entity_data_->get_failed_logical_supervision_count(se_local_params.se_id, failed_logical_count)) &&
        (supervised_entity_data_->get_failed_alive_supervision_count(se_local_params.se_id, failed_alive_count));
    if (result)
    {
        if ((failed_deadline_count > 0u) || (failed_logical_count > 0u) || (failed_alive_count > 0u))
        {
            new_local_status = hm_local_status_t::hm_local_status_recovered;
        }
        else
        {
            new_local_status = hm_local_status_t::hm_local_status_ok;
            // [TODO] Send command to make the corresponding backup SE to backup mode
            // Set the backup's  local status to be deactivated
        }
        // Update the new status
        result &= supervised_entity_data_->set_local_status(se_local_params.se_id, new_local_status);
    }
    return result;
}

bool HMLocalStatusProcessor::update_se_local_status_backup(const hm_local_status_param_t& se_local_params)
{
    bool result{false};
    hm_local_status_t new_local_status{hm_local_status_t::hm_local_status_backup};
    uint32_t failed_deadline_count{0u};
    uint32_t failed_logical_count{0u};
    uint32_t failed_alive_count{0u};
    uint32_t backup_cycle_count{0u};
    result = (supervised_entity_data_->get_failed_deadline_supervision_count(se_local_params.se_id, failed_deadline_count)) &&
        (supervised_entity_data_->get_failed_logical_supervision_count(se_local_params.se_id, failed_logical_count)) &&
        (supervised_entity_data_->get_failed_alive_supervision_count(se_local_params.se_id, failed_alive_count)) &&
        (supervised_entity_data_->get_backup_cycle_count(se_local_params.se_id, backup_cycle_count));
    if (result)
    {
        if ((failed_deadline_count > 0u) || (failed_logical_count > 0u) || (failed_alive_count > 0u))
        {
            const auto backup_failed = (se_local_params.backup_supervision_cycle_tolerance == 0u) ||
                (se_local_params.backup_supervision_cycle_tolerance <= backup_cycle_count);
            if (backup_failed)
            {
                new_local_status = hm_local_status_t::hm_local_status_expired;
                result &= global_supervision_data_->increase_num_local_status_expired();
                result &= supervised_entity_data_->set_backup_cycle_count(se_local_params.se_id, 0u);
            }
            else
            {
                new_local_status = hm_local_status_t::hm_local_status_backup;
                result &= supervised_entity_data_->increase_backup_cycle_count(se_local_params.se_id);
            }
        }
        else
        {
            const auto dec_cycles = backup_cycle_count - 1u;
            const auto backup_ok = (se_local_params.backup_supervision_cycle_tolerance == 0u) ||
                (se_local_params.backup_supervision_cycle_tolerance > dec_cycles);
            if (backup_ok)
            {
                new_local_status = hm_local_status_t::hm_local_status_ok;
            }
            else
            {
                new_local_status = hm_local_status_t::hm_local_status_expired;
                result &= global_supervision_data_->increase_num_local_status_expired();
            }
            result &= supervised_entity_data_->set_backup_cycle_count(se_local_params.se_id, 0u);
        }
        // Update the new status
        result &= supervised_entity_data_->set_local_status(se_local_params.se_id, new_local_status);
    }
    return result;
}

void HMLocalStatusProcessor::print_local_status(const hm_supervised_entity_id_t se_id)
{
    uint32_t failed_alive_count{0u};
    supervised_entity_data_->get_failed_alive_supervision_count(se_id, failed_alive_count);
    uint32_t failed_deadline_count{0u};
    supervised_entity_data_->get_failed_deadline_supervision_count(se_id, failed_deadline_count);
    hm_local_status_t local_status{hm_local_status_t::hm_local_status_ok};
    supervised_entity_data_->get_local_status(se_id, local_status);
    std::cout << "[HMLocalStatusProcessor] SE" << se_id
              << " Failed alive : " << failed_alive_count
              << " Failed deadline : " << failed_deadline_count
              << " Local status : " << local_status_to_string(local_status) << std::endl;
}

const std::string& HMLocalStatusProcessor::local_status_to_string(const hm_local_status_t status)
{
    return local_status_to_string_map.at(status);
}
