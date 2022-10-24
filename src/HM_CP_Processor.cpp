/**
 * @file HM_CP_Processor.cpp
 * @author Van C. Ngo
 * @brief This file contains the CP processor
 * @version 0.1
 * @date 2022-03-13
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include <iostream>

#include "HM_CP_Processor.h"
#include "HM_Cfg_Handler.h"


HMCPProcessor::HMCPProcessor(std::shared_ptr<HMCPDataIntf> checkpoint_data,
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
}

bool HMCPProcessor::process_data(const HMTPCheckpoint& checkpoint)
{
    bool result{false};
    hm_mode_t active_mode_id{0};
    if (is_valid_supervision_data && global_supervision_data_->get_active_mode_id(active_mode_id))
    {
        const auto se_id = checkpoint.se_id();
        const auto cp_id = checkpoint.cp_id();
        const auto timestamp_ms = checkpoint.timestamp_ms();
        std::cout << "[HMCPProcessor] Processing checkpoint (" << se_id << ", " << cp_id << ")" << std::endl;
        result = process_cp(se_id, cp_id, timestamp_ms, HMCfgHandler::get_cp_supervisions(active_mode_id, se_id));
    }
    return result;
}

bool HMCPProcessor::process_cp(const hm_supervised_entity_id_t se_id,
                               const hm_checkpoint_id_t cp_id,
                               const uint64_t timestamp_ms,
                               const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>& cp_sups)
{
    bool result{true};
    bool exit_flag{false};
    hm_cp_supervision_result_t sup_result{hm_cp_supervision_result_t::hm_cp_supervision_correct};
    for (const auto& sup : cp_sups)
    {
        if (exit_flag)
        {
            break;
        }

        const auto cp_sup_type = sup->get_cp_supervision_type(se_id, cp_id);
        switch (cp_sup_type)
        {
            case hm_cp_type_t::hm_alive_supervision_cp:
                // Update alive indications
                result &= checkpoint_data_->increase_alive_indications(se_id, cp_id);
                exit_flag = true;
                break;
            case hm_cp_type_t::hm_deadline_supervision_start_cp:
                // Update start time
                result &= checkpoint_data_->set_deadline_timestamp(se_id, cp_id, timestamp_ms);
                break;
            case hm_cp_type_t::hm_deadline_supervision_stop_cp:
                // Update stop time
                result &= checkpoint_data_->set_deadline_timestamp(se_id, cp_id, timestamp_ms);
                result &= sup->compute_supervision_result(checkpoint_data_, sup_result);
                if (sup_result == hm_cp_supervision_result_t::hm_cp_supervision_incorrect)
                {
                   result &= supervised_entity_data_->
                       increase_failed_deadline_supervision_count(se_id);
                }
                break;
            case hm_cp_type_t::hm_logical_supervision_begin_cp:
                std::cout << "[HMCPProcessor] Logical supervision is not implemented yet!"
                          << std::endl;
                break;
            case hm_cp_type_t::hm_logical_supervision_end_cp:
                std::cout << "[HMCPProcessor] Logical supervision is not implemented yet!"
                          << std::endl;
                break;
            case hm_cp_type_t::hm_logical_supervision_other_cp:
                std::cout << "[HMCPProcessor] Logical supervision is not implemented yet!"
                          << std::endl;
                break;
            default:
                break;
        }
    }
    return result;
}
