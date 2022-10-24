/**
 * @file HM_Failure_Handler.cpp
 * @author Van C. Ngo
 * @brief This file contains the failure handler implementation
 * @version 0.1
 * @date 2022-05-23
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include "HM_Failure_Handler.h"
#include "HM_Cfg_Handler.h"
#include "HM_Local_Status_Processor.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>


HMFailureHandler::HMFailureHandler(std::shared_ptr<HMCPDataIntf> checkpoint_data,
                                   std::shared_ptr<HMHSDataIntf> health_status_data,
                                   std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data,
                                   std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data,
                                   std::unique_ptr<HMDataSenderIntf<HMTPRecoveryCmd>> recovery_cmd_sender)
{
    checkpoint_data_ = std::move(checkpoint_data);
    health_status_data_ = std::move(health_status_data);
    supervised_entity_data_ = std::move(supervised_entity_data);
    global_supervision_data_ = std::move(global_supervision_data);
    recovery_cmd_sender_ = std::move(recovery_cmd_sender);
    is_valid_supervision_data = (checkpoint_data_ != nullptr) &&
        (health_status_data_ != nullptr) &&
        (supervised_entity_data_ != nullptr) &&
        (global_supervision_data_ != nullptr) &&
        (recovery_cmd_sender_ != nullptr);
    // Initialize the function map
    cp_handling_func_map_[hm_cp_failure_recovery_t::hm_failure_recovery_nothing] =
        std::bind(&HMFailureHandler::handle_cp_failure_nothing, this, std::placeholders::_1);
    cp_handling_func_map_[hm_cp_failure_recovery_t::hm_failure_recovery_reset] =
        std::bind(&HMFailureHandler::handle_cp_failure_reset, this, std::placeholders::_1);
    cp_handling_func_map_[hm_cp_failure_recovery_t::hm_failure_recovery_restart] =
        std::bind(&HMFailureHandler::handle_cp_failure_restart, this, std::placeholders::_1);
    cp_handling_func_map_[hm_cp_failure_recovery_t::hm_failure_recovery_backup] =
        std::bind(&HMFailureHandler::handle_cp_failure_backup, this, std::placeholders::_1);
    // Initialize the restarting and backup count maps
    const auto& ses = HMCfgHandler::get_supervised_entities();
    for (const auto& se : ses)
    {
        se_restarting_count_[se.se_id] = 0u;
        se_backup_count_[se.se_id] = 0u;
        se_names_[se.se_id] = se.se_name;
    }
}

bool HMFailureHandler::handle_failure_recovery()
{
    bool result{false};
    hm_mode_t active_mode_id{0u};
    std::shared_ptr<const HMModeCfgHandlerIntf> active_mode_cfg{nullptr};
    hm_global_status_t current_global_status{hm_global_status_t::hm_global_status_deactivated};
    if (is_valid_supervision_data)
    {
        result = global_supervision_data_->get_active_mode_id(active_mode_id);
        active_mode_cfg = HMCfgHandler::get_mode_cfg_handler(active_mode_id);
        result &= (active_mode_cfg != nullptr);
        result &= global_supervision_data_->get_global_status(current_global_status);
    }
    if (result)
    {
        wdg_trigger_condition_time_ = (active_mode_cfg->get_supervision_cycle_time() * 2) / 3;
        if (current_global_status == hm_global_status_t::hm_global_status_stopped)
        {
            // Handle checkpoint supervision failures
            const auto& cp_failure_recovery_params = active_mode_cfg->get_cp_recovery_params();
            for (const auto& cp_rec_param : cp_failure_recovery_params)
            {
                result &= handle_cp_supervision_failure(cp_rec_param);
            }
            // Handle invalid health status supervisions
            const auto& hs_invalid_recovery_params = active_mode_cfg->get_hs_reaction_params();
            for (const auto& hs_rec_param : hs_invalid_recovery_params)
            {
                result &= handle_hs_supervision_failure(hs_rec_param);
            }
        }
        else
        {
            result &= set_wdg_trigger_condition(HMCfgHandler::get_wdg_device(), wdg_trigger_condition_time_);
        }
    }
    return result;
}

bool HMFailureHandler::set_wdg_trigger_condition(const hm_watchdog_t& wdg, const uint16_t timeout_ms)
{
    bool result{false};
    if (is_valid_supervision_data)
    {
        // [TODO] Set watchdog trigger condition
        result = true;
    }
    return result;
}

bool HMFailureHandler::handle_cp_supervision_failure(const hm_cp_failure_recovery_param_t& cp_rec_param)
{
    bool result{true};
    hm_local_status_t local_status{hm_local_status_t::hm_local_status_deactivated};
    if (supervised_entity_data_->get_local_status(cp_rec_param.se_id, local_status) &&
        (local_status == hm_local_status_t::hm_local_status_expired))
    {
        result = cp_handling_func_map_[cp_rec_param.recovery_action](cp_rec_param);
        // Print out for testing
        print_cp_handling_result(cp_rec_param.se_id);
    }
    return result;
}

bool HMFailureHandler::handle_hs_supervision_failure(const hm_hs_invalid_reaction_param_t& hs_rec_param)
{
    bool result{true};
    // [TODO] Handler invalid health status
    return result;
}

bool HMFailureHandler::handle_cp_failure_nothing(const hm_cp_failure_recovery_param_t& cp_rec_param)
{
    // Do nothing
    return true;
}

bool HMFailureHandler::handle_cp_failure_reset(const hm_cp_failure_recovery_param_t& cp_rec_param)
{
    bool result{true};
    if (HMCfgHandler::get_immediate_reset())
    {
        // [TODO] Send immediate reset request
    }
    return result;
}

bool HMFailureHandler::handle_cp_failure_restart(const hm_cp_failure_recovery_param_t& cp_rec_param)
{
    bool result{true};
    const auto se_id = cp_rec_param.se_id;
    const auto& se_name = se_names_[se_id];
    const auto restarting_count = se_restarting_count_[se_id];
    const auto restarting_limit = cp_rec_param.recovery_restart_limit;
    if ((restarting_limit == 0) || (restarting_count < restarting_limit))
    {
        // Send restart command
        HMTPRecoveryCmd cmd;
        cmd.recovery_type(0u);
        cmd.se_id(se_id);
        cmd.se_name(se_name);
        cmd.backup_se_id(se_id);
        cmd.backup_se_name(se_name);
        auto now_timestamp = std::chrono::system_clock::now();
        auto now_timestamp_ms = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>
                                                      (now_timestamp.time_since_epoch()).count());
        cmd.timestamp_ms(now_timestamp_ms);
        result &= recovery_cmd_sender_->publish(cmd);
        if (result)
        {
            se_restarting_count_[se_id] = restarting_count + 1;
            result &= supervised_entity_data_->increase_restarted_cycle_count(se_id);
            result &= supervised_entity_data_->set_local_status(se_id, hm_local_status_t::hm_local_status_restarted);
            // Set watchdog trigger condition
            result &= set_wdg_trigger_condition(HMCfgHandler::get_wdg_device(), wdg_trigger_condition_time_);
            result &= global_supervision_data_->set_global_status(hm_global_status_t::hm_global_status_ok);
        }
    }
    return result;
}

bool HMFailureHandler::handle_cp_failure_backup(const hm_cp_failure_recovery_param_t& cp_rec_param)
{
    bool result{true};
    const auto se_id = cp_rec_param.se_id;
    const auto backup_se_id = cp_rec_param.backup_id;
    const auto& se_name = se_names_[se_id];
    const auto& backup_se_name = se_names_[backup_se_id];
    const auto backup_count = se_backup_count_[se_id];
    const auto backup_limit = cp_rec_param.recovery_backup_limit;
    if ((backup_limit == 0) || (backup_count < backup_limit))
    {
        // Send backup command
        HMTPRecoveryCmd cmd;
        cmd.recovery_type(1u);
        cmd.se_id(se_id);
        cmd.se_name(se_name);
        cmd.backup_se_id(backup_se_id);
        cmd.backup_se_name(backup_se_name);
        auto now_timestamp = std::chrono::system_clock::now();
        auto now_timestamp_ms = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>
                                                      (now_timestamp.time_since_epoch()).count());
        cmd.timestamp_ms(now_timestamp_ms);
        result &= recovery_cmd_sender_->publish(cmd);
        if (result)
        {
            se_backup_count_[se_id] = backup_count + 1;
            result &= supervised_entity_data_->set_local_status(se_id, hm_local_status_t::hm_local_status_recovered);
            result &= supervised_entity_data_->increase_backup_cycle_count(backup_se_id);
            result &= supervised_entity_data_->set_local_status(backup_se_id, hm_local_status_t::hm_local_status_backup);
            // Set watchdog trigger condition
            result &= set_wdg_trigger_condition(HMCfgHandler::get_wdg_device(), wdg_trigger_condition_time_);
            result &= global_supervision_data_->set_global_status(hm_global_status_t::hm_global_status_ok);
        }
    }
    return result;
}

void HMFailureHandler::print_cp_handling_result(const hm_supervised_entity_id_t se_id)
{
    hm_local_status_t local_status{hm_local_status_t::hm_local_status_expired};
    supervised_entity_data_->get_local_status(se_id, local_status);
    std::cout << "[HMFailurehandler] SE" << se_id
              << " Local status after recovery handling : " << HMLocalStatusProcessor::local_status_to_string(local_status) << std::endl;
}
