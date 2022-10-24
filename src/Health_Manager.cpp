/**
 * @file Health_Manager.cpp
 * @author Van C. Ngo
 * @brief This file contains the main class of HM
 * @version 0.1
 * @date 2022-03-20
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include "Health_Manager.h"
#include "HM_Types.h"
#include "HM_Cfg_Handler.h"
#include "HM_Component_Factory.h"

#include <chrono>
#include <thread>
#include <atomic>
#include <map>


const std::string hm_version_info = "0.1";

HealthManager::HealthManager(std::unique_ptr<HMComponentFactoryIntf> component_factory)
    : component_factory_(std::move(component_factory))
{
    if (component_factory_ != nullptr)
    {
        // Get the active mode
        active_mode_id_ = HMCfgHandler::get_init_mode_id();

        // Initialize all composed components
        checkpoint_data_ = component_factory_->create_checkpoint_data();
        health_status_data_ = component_factory_->create_hs_data();
        supervised_entity_data_ = component_factory_->create_se_data();
        global_supervision_data_ = component_factory_->create_global_sup_data(active_mode_id_);
        cp_processor_ = component_factory_->create_cp_processor(checkpoint_data_,
                                                                supervised_entity_data_,
                                                                health_status_data_,
                                                                global_supervision_data_);
        cp_receiver_ = component_factory_->create_cp_receiver();
        is_valid_system_ = (cp_receiver_ != nullptr) && cp_receiver_->init(cp_processor_);
        local_status_processor_ = component_factory_->create_local_status_processor(checkpoint_data_,
                                                                                    supervised_entity_data_,
                                                                                    health_status_data_,
                                                                                    global_supervision_data_);
        global_status_processor_ = component_factory_->create_global_status_processor(checkpoint_data_,
                                                                                      supervised_entity_data_,
                                                                                      health_status_data_,
                                                                                      global_supervision_data_);
        auto recovery_cmd_sender = component_factory_->create_recovery_cmd_sender();
        is_valid_system_ = is_valid_system_ && (recovery_cmd_sender != nullptr) && recovery_cmd_sender->init();
        failure_handler_ = component_factory_->create_failure_handler(checkpoint_data_,
                                                                      supervised_entity_data_,
                                                                      health_status_data_,
                                                                      global_supervision_data_,
                                                                      std::move(recovery_cmd_sender));
        auto status_sender = component_factory_->create_supervision_status_sender();
        is_valid_system_ = is_valid_system_ && (status_sender != nullptr) && status_sender->init();
        status_reporter_ = component_factory_->create_status_reporter(checkpoint_data_,
                                                                      supervised_entity_data_,
                                                                      health_status_data_,
                                                                      global_supervision_data_,
                                                                      std::move(status_sender));
        is_valid_system_ = is_valid_system_ &&
            HMCfgHandler::validate() &&
            (checkpoint_data_ != nullptr) &&
            (health_status_data_ != nullptr) &&
            (supervised_entity_data_ != nullptr) &&
            (global_supervision_data_ != nullptr) &&
            (cp_processor_ != nullptr) &&
            (local_status_processor_ != nullptr) &&
            (global_status_processor_ != nullptr) &&
            (failure_handler_ != nullptr) &&
            (status_reporter_ != nullptr);
    }
}

HealthManager::~HealthManager()
{
    if (is_initialized_ && supervision_thread_.joinable())
    {
        supervision_thread_.join();
    }
}

bool HealthManager::init()
{
    if (!is_initialized_ && is_valid_system_)
    {
        global_supervision_data_->set_global_status(hm_global_status_t::hm_global_status_deactivated);
        // Get the supervision cycle time
        const auto active_mode_cfg = HMCfgHandler::get_mode_cfg_handler(active_mode_id_);
        if (active_mode_cfg != nullptr)
        {
            supervision_cycle_time_ = active_mode_cfg->get_supervision_cycle_time();
        }
        // [TODO] Create and start the watchdog feeding timer
        // [TODO] Initialize and set watchdog driver mode
        // Spawn the supervision thread
        supervision_thread_ = std::thread(&HealthManager::supervision_thread_function, this);
        is_initialized_ = supervision_thread_.joinable();
        if (is_initialized_)
        {
            global_supervision_data_->set_global_status(hm_global_status_t::hm_global_status_ok);
        }
    }
    return is_initialized_;
}

bool HealthManager::deinit()
{
    bool result{false};
    if (is_initialized_ && supervision_thread_.joinable())
    {
        global_supervision_data_->set_global_status(hm_global_status_t::hm_global_status_deactivated);
        exit_flag_ = true;
        supervision_thread_.join();
        is_initialized_ = false;
        // [TODO] Stop the watchdog
        result = true;
    }
    return result;
}

std::string HealthManager::get_version()
{
    return hm_version_info;
}

bool HealthManager::get_current_local_status(const hm_supervised_entity_id_t se_id,
                                             hm_local_status_t& local_status)
{
    bool result{false};
    if (is_initialized_ && (supervised_entity_data_ != nullptr))
    {
        result = supervised_entity_data_->get_local_status(se_id, local_status);
    }
    return result;
}

bool HealthManager::get_current_global_status(hm_global_status_t& global_status)
{
    bool result{false};
    if (is_initialized_ && (global_supervision_data_ != nullptr))
    {
        result = global_supervision_data_->get_global_status(global_status);
    }
    return result;
}

hm_mode_t HealthManager::get_mode()
{
    return active_mode_id_;
}

bool HealthManager::set_mode(const hm_mode_t)
{
    // [TODO] Set the local status for all SEs and global status
    return true;
}

void HealthManager::supervision_thread_function()
{
    std::chrono::time_point<std::chrono::steady_clock> last_cycle_time = std::chrono::steady_clock::now();
    checkpoint_data_->init_supervision_time_points(last_cycle_time);
    uint64_t supervision_exe_time_us{0u};
    hm_global_status_t global_status{hm_global_status_t::hm_global_status_deactivated};
    while (!exit_flag_)
    {
        // Sleep until next cycle time
        std::this_thread::sleep_until(last_cycle_time + std::chrono::milliseconds(supervision_cycle_time_));
        last_cycle_time = std::chrono::steady_clock::now();
        global_supervision_data_->get_global_status(global_status);
        // Perform the supervision if it is not in deactivated state
        if (global_status != hm_global_status_t::hm_global_status_deactivated)
        {
            // Process local status
            if (!local_status_processor_->process_local_status())
            {
                std::cout << "[HealthManager] Failed to process local status" << std::endl;
            }
            // Process global status
            if (!global_status_processor_->process_global_status())
            {
                std::cout << "[HealthManager] Failed to process global status" << std::endl;
            }
            // Supervision status report
            if (!status_reporter_->report_supervision_failure())
            {
                std::cout << "[Healthmanager] Failed to report supervision status" << std::endl;
            }
            // Failure handling including watchdog trigger condition setting
            if (!failure_handler_->handle_failure_recovery())
            {
                std::cout << "[HealthManager] Failed to handle supervision failures" << std::endl;
            }
            supervision_exe_time_us = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - last_cycle_time).count();
            std::cout << "[HealthManager] Supervision cycle run-time : " << supervision_exe_time_us
                      << "us" << std::endl;
        }
    }
}
