/**
 * @file HM_CP_Supervision_Cfg_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the CP supervision interface
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include <vector>

#include "HM_Types.h"
#include "HM_CP_Data_Intf.h"


class HMCPSupervisionCfgIntf
{
public:
    HMCPSupervisionCfgIntf() = default;

    virtual ~HMCPSupervisionCfgIntf() = default;

    virtual hm_supervision_id_t get_supervision_id() const = 0;

    virtual hm_supervised_entity_id_t get_se_id() const = 0;

    virtual const std::vector<hm_checkpoint_id_t>& get_checkpoint_ids() const = 0;

    virtual bool is_contained(const hm_supervised_entity_id_t se_id,
                              const hm_checkpoint_id_t cp_id) const = 0;

    virtual hm_supervision_type_t get_supervision_type() const = 0;

    virtual hm_cp_type_t get_cp_supervision_type(const hm_supervised_entity_id_t se_id,
                                                 const hm_checkpoint_id_t cp_id) const = 0;

    virtual bool compute_supervision_result(const std::shared_ptr<HMCPDataIntf>& cp_data,
                                            hm_cp_supervision_result_t& result,
                                            const uint32_t cycle_time = 0u) const = 0;

    virtual const hm_alive_supervision_param_t& get_alive_supervision_params() const
    {
        static hm_alive_supervision_param_t default_params{};
        return default_params;
    }

    virtual const hm_deadline_supervision_param_t& get_deadline_supervision_params() const
    {
        static hm_deadline_supervision_param_t default_params{};
        return default_params;
    }

    virtual const hm_logical_supervision_param_t& get_logical_supervision_params() const
    {
        static hm_logical_supervision_param_t default_params{};
        return default_params;
    }
};
