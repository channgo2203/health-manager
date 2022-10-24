/**
 * @file HM_Failure_Handler.h
 * @author Van C. Ngo
 * @brief This file contains the failure handler header
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

#pragma once

#include "HM_Types.h"
#include "HM_Failure_Handler_Intf.h"
#include "HM_TP_RecoveryCmdPubSubTypes.h"
#include "HM_Data_Sender_Intf.h"

#include <memory>
#include <functional>
#include <map>

using hm_cp_failure_func_map = std::map<hm_cp_failure_recovery_t, std::function<bool(const hm_cp_failure_recovery_param_t&)>>;

class HMFailureHandler : public HMFailureHandlerIntf
{
public:
    HMFailureHandler() = delete;

    virtual ~HMFailureHandler() = default;

    HMFailureHandler(std::shared_ptr<HMCPDataIntf> checkpoint_data,
                     std::shared_ptr<HMHSDataIntf> health_status_data,
                     std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data,
                     std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data,
                     std::unique_ptr<HMDataSenderIntf<HMTPRecoveryCmd>> recovery_cmd_sender);

    bool handle_failure_recovery() override;

    bool set_wdg_trigger_condition(const hm_watchdog_t& wdg, const uint16_t timeout_ms) override;
private:
    bool handle_cp_supervision_failure(const hm_cp_failure_recovery_param_t& cp_rec_param);
    bool handle_hs_supervision_failure(const hm_hs_invalid_reaction_param_t& hs_rec_param);

    bool handle_cp_failure_nothing(const hm_cp_failure_recovery_param_t& cp_rec_param);
    bool handle_cp_failure_reset(const hm_cp_failure_recovery_param_t& cp_rec_param);
    bool handle_cp_failure_restart(const hm_cp_failure_recovery_param_t& cp_rec_param);
    bool handle_cp_failure_backup(const hm_cp_failure_recovery_param_t& cp_rec_param);

    void print_cp_handling_result(const hm_supervised_entity_id_t se_id);

    std::shared_ptr<HMCPDataIntf> checkpoint_data_{nullptr};
    std::shared_ptr<HMHSDataIntf> health_status_data_{nullptr};
    std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data_{nullptr};
    std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data_{nullptr};
    std::unique_ptr<HMDataSenderIntf<HMTPRecoveryCmd>> recovery_cmd_sender_{nullptr};
    bool is_valid_supervision_data{false};
    uint16_t wdg_trigger_condition_time_{0u};
    std::map<hm_supervised_entity_id_t, uint32_t> se_restarting_count_{};
    std::map<hm_supervised_entity_id_t, uint32_t> se_backup_count_{};
    std::map<hm_supervised_entity_id_t, std::string> se_names_{};
    hm_cp_failure_func_map cp_handling_func_map_{};
};
