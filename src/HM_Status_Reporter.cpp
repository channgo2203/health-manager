/**
 * @file HM_Status_Reporter.cpp
 * @author Van C. Ngo
 * @brief This file contains the supervision failure status reporter
 * @version 0.1
 * @date 2022-05-31
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include "HM_Status_Reporter.h"
#include "HM_Cfg_Handler.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>


HMStatusReporter::HMStatusReporter(std::shared_ptr<HMCPDataIntf> checkpoint_data,
                                   std::shared_ptr<HMHSDataIntf> health_status_data,
                                   std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data,
                                   std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data,
                                   std::unique_ptr<HMDataSenderIntf<HMTPSupervisionStatus>> status_sender)
{
    checkpoint_data_ = std::move(checkpoint_data);
    health_status_data_ = std::move(health_status_data);
    supervised_entity_data_ = std::move(supervised_entity_data);
    global_supervision_data_ = std::move(global_supervision_data);
    status_sender_ = std::move(status_sender);
    is_valid_supervision_data = (checkpoint_data_ != nullptr) &&
        (health_status_data_ != nullptr) &&
        (supervised_entity_data_ != nullptr) &&
        (global_supervision_data_ != nullptr) &&
        (status_sender_ != nullptr);
    // Initialize the restarting and backup count maps
    const auto& ses = HMCfgHandler::get_supervised_entities();
    for (const auto& se : ses)
    {
        se_names_[se.se_id] = se.se_name;
    }
    const auto& ess = HMCfgHandler::get_external_supervision_entities();
    for (const auto& es : ess)
    {
        es_names_[es.es_id] = es.es_name;
    }
}

bool HMStatusReporter::report_supervision_failure()
{
    bool result{false};
    hm_mode_t active_mode_id{0u};
    std::shared_ptr<const HMModeCfgHandlerIntf> active_mode_cfg{nullptr};
    hm_global_status_t global_status{hm_global_status_t::hm_global_status_deactivated};
    HMTPSupervisionStatus status;
    if (is_valid_supervision_data && global_supervision_data_->get_active_mode_id(active_mode_id))
    {
        active_mode_cfg = HMCfgHandler::get_mode_cfg_handler(active_mode_id);
        result = (active_mode_cfg != nullptr);
        result &= global_supervision_data_->get_global_status(global_status);
    }
    if (result)
    {
        status.global_status(static_cast<uint8_t>(global_status));
        // Report cp supervisions
        result = report_cp_supervision_failures(active_mode_cfg, status);
        // Report hs supervisions
        result &= report_hs_supervision_failures(active_mode_cfg, status);
    }
    if (result)
    {
        const auto num_cp_failures = status.cp_supervision_status().size();
        const auto num_hs_failures = status.hs_supervision_status().size();
        print_reporting_result(status);
        if ((num_cp_failures > 0) || (num_hs_failures > 0))
        {
            // Send the status
            result &= status_sender_->publish(status);
        }
    }
    return result;
}

bool HMStatusReporter::report_cp_supervision_failures(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_cfg,
                                                      HMTPSupervisionStatus& status)
{
    bool result{true};
    std::vector<HMTPLocalStatus> cp_local_status{};
    const auto& cp_failure_recovery_params = active_cfg->get_cp_recovery_params();
    for (const auto& cp_rec_param : cp_failure_recovery_params)
    {
        hm_local_status_t local_status{hm_local_status_t::hm_local_status_deactivated};
        if (!supervised_entity_data_->get_local_status(cp_rec_param.se_id, local_status))
        {
            result &= false;
        }
        else if (local_status == hm_local_status_t::hm_local_status_expired)
        {
            HMTPLocalStatus tp_local_status;
            tp_local_status.id(cp_rec_param.se_id);
            tp_local_status.name(se_names_[cp_rec_param.se_id]);
            tp_local_status.recovery_type(static_cast<uint8_t>(cp_rec_param.recovery_action));
            tp_local_status.local_status(static_cast<uint8_t>(local_status));
            cp_local_status.push_back(tp_local_status);
        }
    }
    status.cp_supervision_status(cp_local_status);
    return result;
}

bool HMStatusReporter::report_hs_supervision_failures(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_cfg,
                                                      HMTPSupervisionStatus& status)
{
    bool result{true};
    std::vector<HMTPLocalStatus> hs_local_status{};
    const auto& hs_invalid_recovery_params = active_cfg->get_hs_reaction_params();
    for (const auto& hs_rec_param : hs_invalid_recovery_params)
    {
        hm_local_status_t local_status{hm_local_status_t::hm_local_status_deactivated};
        if (!health_status_data_->get_local_status(hs_rec_param.es_id, local_status))
        {
            result &= false;
        }
        else if (local_status == hm_local_status_t::hm_local_status_expired)
        {
            HMTPLocalStatus tp_local_status;
            tp_local_status.id(hs_rec_param.es_id);
            tp_local_status.name(es_names_[hs_rec_param.es_id]);
            tp_local_status.recovery_type(static_cast<uint8_t>(hs_rec_param.hs_action));
            tp_local_status.local_status(static_cast<uint8_t>(local_status));
            hs_local_status.push_back(tp_local_status);
        }
    }
    status.hs_supervision_status(hs_local_status);
    return result;
}

void HMStatusReporter::print_reporting_result(const HMTPSupervisionStatus& status)
{
    const auto num_cp_failures = status.cp_supervision_status().size();
    const auto num_hs_failures = status.hs_supervision_status().size();
    std::cout << "[HMStatusreporter] Number of expired supervised entities : "
              << num_cp_failures
              << " Number of invalid health status : "
              << num_hs_failures << std::endl;
}
