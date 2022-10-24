/**
 * @file HM_Mode_Cfg_Handler.h
 * @author Van C. Ngo
 * @brief This file contains the mode configuration
 * @version 0.1
 * @date 2022-03-16
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include "HM_Mode_Cfg_Handler_Intf.h"


class HMModeCfgHandler : public HMModeCfgHandlerIntf
{
public:
    HMModeCfgHandler() = delete;

    HMModeCfgHandler(const uint32_t expired_supervision_cycle_tolerance,
                     const hm_mode_t mode_id,
                     const uint32_t supervision_cycle_time,
                     const std::vector<hm_local_status_param_t>& local_supervision_params,
                     const std::vector<hm_hs_supervision_param_t>& hs_supervision_params,
                     const std::vector<hm_cp_failure_recovery_param_t>& cp_recovery_params,
                     const std::vector<hm_hs_invalid_reaction_param_t>& hs_reaction_params,
                     const std::map<hm_supervised_entity_id_t,
                     std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>>& cp_supervisions,
                     const std::map<hm_external_supervision_id_t,
                     std::shared_ptr<const HMHSSupervisionCfgIntf>>& hs_supervisions);


    virtual ~HMModeCfgHandler() = default;

    bool validate() const override;

    bool is_se_contained(const hm_supervised_entity_id_t se_id) const override;

    bool is_cp_contained(const hm_supervised_entity_id_t se_id,
                         const hm_checkpoint_id_t cp_id) const override;

    bool is_hs_contained(const hm_external_supervision_id_t es_id) const override;

    const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
        get_cp_supervisions(const hm_supervised_entity_id_t se_id) const override;

    std::shared_ptr<const HMHSSupervisionCfgIntf>
        get_hs_supervision(const hm_external_supervision_id_t es_id) const override;

    uint32_t get_expired_supervision_cycle_tolerance() const override;

    hm_mode_t get_mode_id() const override;

    uint32_t get_supervision_cycle_time() const override;

    const std::vector<hm_local_status_param_t>&
        get_local_supervision_params() const override;

    const std::vector<hm_hs_supervision_param_t>&
        get_hs_supervision_params() const override;

    const std::vector<hm_cp_failure_recovery_param_t>&
        get_cp_recovery_params() const override;

    const std::vector<hm_hs_invalid_reaction_param_t>&
        get_hs_reaction_params() const override;

    const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
        get_alive_supervisions() const override;

    const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
        get_deadline_supervisions() const override;

    const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
        get_logical_supervisions() const override;

    const std::vector<std::shared_ptr<const HMHSSupervisionCfgIntf>>&
        get_hs_supervisions() const override;
private:
    const static std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>> empty_cp_supervision_;
    const uint32_t expired_supervision_cycle_tolerance_;
    const hm_mode_t hm_mode_id_;
    const uint32_t supervision_cycle_time_;
    const std::vector<hm_local_status_param_t> local_supervision_params_;
    const std::vector<hm_hs_supervision_param_t> hs_supervision_params_;
    const std::vector<hm_cp_failure_recovery_param_t> cp_recovery_params_;
    const std::vector<hm_hs_invalid_reaction_param_t> hs_reaction_params_;
    const std::map<hm_supervised_entity_id_t,
        std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>> cp_supervisions_;
    const std::map<hm_external_supervision_id_t,
        std::shared_ptr<const HMHSSupervisionCfgIntf>> hs_supervisions_;
    std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>> alive_supervisions_;
    std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>> deadline_supervisions_;
    std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>> logical_supervisions_;
    std::vector<std::shared_ptr<const HMHSSupervisionCfgIntf>> health_status_supervisions_;
};
