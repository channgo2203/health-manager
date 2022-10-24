/**
 * @file HM_Alive_Supervision_Cfg.cpp
 * @author Van C. Ngo
 * @brief This file contains the alive supervision
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

#include <iostream>

#include "HM_Alive_Supervision_Cfg.h"


HMAliveSupervisionCfg::HMAliveSupervisionCfg(const hm_alive_supervision_param_t& params)
    : supervision_params_(params)
{
    cp_ids_.push_back(supervision_params_.cp_id);
}

hm_supervision_id_t HMAliveSupervisionCfg::get_supervision_id() const
{
    return supervision_params_.supervision_id;
}

hm_supervised_entity_id_t HMAliveSupervisionCfg::get_se_id() const
{
    return supervision_params_.se_id;
}

const std::vector<hm_checkpoint_id_t>& HMAliveSupervisionCfg::get_checkpoint_ids() const
{
    return cp_ids_;
}

bool HMAliveSupervisionCfg::is_contained(const hm_supervised_entity_id_t se_id,
                                         const hm_checkpoint_id_t cp_id) const
{
    const bool result = (supervision_params_.se_id == se_id) &&
                        (supervision_params_.cp_id == cp_id);
    return result;
}

hm_supervision_type_t HMAliveSupervisionCfg::get_supervision_type() const
{
    return hm_supervision_type_t::hm_alive_supervision;
}

hm_cp_type_t HMAliveSupervisionCfg::get_cp_supervision_type(const hm_supervised_entity_id_t se_id,
                                                            const hm_checkpoint_id_t cp_id) const
{
    hm_cp_type_t result{hm_cp_type_t::hm_unknown_supervision_cp};
    if (is_contained(se_id, cp_id))
    {
        result = hm_cp_type_t::hm_alive_supervision_cp;
    }
    return result;
}

bool HMAliveSupervisionCfg::compute_supervision_result(const std::shared_ptr<HMCPDataIntf>& cp_data,
                                                       hm_cp_supervision_result_t& result,
                                                       const uint32_t cycle_time) const
{
    bool status{false};
    uint32_t cycle_total{0u};
    uint64_t supervision_time_total{0u};
    const auto get_cycle_total_result = cp_data->get_supervision_cycle_total(supervision_params_.se_id,
                                                                             supervision_params_.cp_id,
                                                                             cycle_total);
    const auto get_supervision_time_total_result = cp_data->get_supervision_time_total(supervision_params_.se_id,
                                                                                  supervision_params_.cp_id,
                                                                                  supervision_time_total);
    if (cycle_time > 0u &&
        get_cycle_total_result &&
        get_supervision_time_total_result &&
        (cycle_total >= supervision_params_.supervision_ref_cycles))
    {
        const auto supervised_indications = static_cast<uint32_t>(
            static_cast<double>(supervision_time_total * supervision_params_.expected_alive_indications) /
            static_cast<double>(supervision_params_.supervision_ref_cycles * cycle_time));
        uint32_t alive_indications{0};
        if (cp_data->get_alive_indications(supervision_params_.se_id, supervision_params_.cp_id, alive_indications))
        {
            const auto sup_result = (alive_indications >= supervised_indications - supervision_params_.min_margin) &&
                                    (alive_indications <= supervised_indications + supervision_params_.max_margin);
            result = sup_result ? hm_cp_supervision_result_t::hm_cp_supervision_correct
                                : hm_cp_supervision_result_t::hm_cp_supervision_incorrect;
            status = true;
        }
    }
    return status;
}

const hm_alive_supervision_param_t& HMAliveSupervisionCfg::get_alive_supervision_params() const
{
    return supervision_params_;
}
