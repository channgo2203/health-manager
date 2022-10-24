/**
 * @file HM_Mode_Cfg_Handler_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the mode configuration interface
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

#include "HM_Types.h"
#include "HM_CP_Supervision_Cfg_Intf.h"
#include "HM_HS_Supervision_Cfg_Intf.h"


class HMModeCfgHandlerIntf
{
public:
    HMModeCfgHandlerIntf() = default;

    virtual ~HMModeCfgHandlerIntf() = default;

    virtual bool validate() const = 0;

    virtual bool is_se_contained(const hm_supervised_entity_id_t se_id) const = 0;

    virtual bool is_cp_contained(const hm_supervised_entity_id_t se_id,
                                 const hm_checkpoint_id_t cp_id) const = 0;

    virtual bool is_hs_contained(const hm_external_supervision_id_t es_id) const = 0;

    virtual const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
        get_cp_supervisions(const hm_supervised_entity_id_t se_id) const = 0;

    virtual std::shared_ptr<const HMHSSupervisionCfgIntf>
        get_hs_supervision(const hm_external_supervision_id_t es_id) const = 0;

    virtual uint32_t get_expired_supervision_cycle_tolerance() const = 0;

    virtual hm_mode_t get_mode_id() const = 0;

    virtual uint32_t get_supervision_cycle_time() const = 0;

    virtual const std::vector<hm_local_status_param_t>&
        get_local_supervision_params() const = 0;

    virtual const std::vector<hm_hs_supervision_param_t>&
        get_hs_supervision_params() const = 0;

    virtual const std::vector<hm_cp_failure_recovery_param_t>&
        get_cp_recovery_params() const = 0;

    virtual const std::vector<hm_hs_invalid_reaction_param_t>&
        get_hs_reaction_params() const = 0;

    virtual const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
        get_alive_supervisions() const = 0;

    virtual const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
        get_deadline_supervisions() const = 0;

    virtual const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
        get_logical_supervisions() const = 0;

    virtual const std::vector<std::shared_ptr<const HMHSSupervisionCfgIntf>>&
        get_hs_supervisions() const = 0;
};
