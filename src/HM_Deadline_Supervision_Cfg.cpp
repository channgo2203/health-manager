/**
 * @file HM_Deadline_Supervision_Cfg.cpp
 * @author Van C. Ngo
 * @brief This file contains the deadline supervision
 * @version 0.1
 * @date 2022-03-10
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include "HM_Deadline_Supervision_Cfg.h"


HMDeadlineSupervisionCfg::HMDeadlineSupervisionCfg(const hm_deadline_supervision_param_t& params)
    : supervision_params_(params)
{
    cp_ids_.push_back(supervision_params_.cp_start_id);
    cp_ids_.push_back(supervision_params_.cp_stop_id);
}

hm_supervision_id_t HMDeadlineSupervisionCfg::get_supervision_id() const
{
    return supervision_params_.supervision_id;
}

hm_supervised_entity_id_t HMDeadlineSupervisionCfg::get_se_id() const
{
    return supervision_params_.se_id;
}

const std::vector<hm_checkpoint_id_t>& HMDeadlineSupervisionCfg::get_checkpoint_ids() const
{
    return cp_ids_;
}

bool HMDeadlineSupervisionCfg::is_contained(const hm_supervised_entity_id_t se_id,
                                            const hm_checkpoint_id_t cp_id) const
{
    const bool result = (supervision_params_.se_id == se_id) &&
                        ((supervision_params_.cp_start_id == cp_id) ||
                         (supervision_params_.cp_stop_id == cp_id));
    return result;
}

hm_supervision_type_t HMDeadlineSupervisionCfg::get_supervision_type() const
{
    return hm_supervision_type_t::hm_deadline_supervision;
}

hm_cp_type_t HMDeadlineSupervisionCfg::get_cp_supervision_type(const hm_supervised_entity_id_t se_id,
                                                               const hm_checkpoint_id_t cp_id) const
{
    hm_cp_type_t result{hm_cp_type_t::hm_unknown_supervision_cp};
    if (supervision_params_.se_id == se_id)
    {
        if (supervision_params_.cp_start_id == cp_id)
        {
            result = hm_cp_type_t::hm_deadline_supervision_start_cp;
        }
        else if (supervision_params_.cp_stop_id == cp_id)
        {
            result = hm_cp_type_t::hm_deadline_supervision_stop_cp;
        }
    }
    return result;
}

bool HMDeadlineSupervisionCfg::compute_supervision_result(const std::shared_ptr<HMCPDataIntf>& cp_data,
                                                          hm_cp_supervision_result_t& result,
                                                          const uint32_t cycle_time) const
{
    bool status{false};
    // Check that both start and stop timestamps are valid
    uint64_t start_time_ms{0u};
    cp_data->get_deadline_timestamp(supervision_params_.se_id, supervision_params_.cp_start_id, start_time_ms);
    uint64_t stop_time_ms{0u};
    cp_data->get_deadline_timestamp(supervision_params_.se_id, supervision_params_.cp_stop_id, stop_time_ms);
    const bool is_valid_timestamps = (start_time_ms > 0) && (stop_time_ms > 0) && (stop_time_ms >= start_time_ms);
    if (is_valid_timestamps)
    {
        const auto exe_time_ms = stop_time_ms - start_time_ms;
        const auto sup_result = (exe_time_ms >= supervision_params_.deadline_min) &&
                                (exe_time_ms <= supervision_params_.deadline_max);
        result = sup_result ? hm_cp_supervision_result_t::hm_cp_supervision_correct
                            : hm_cp_supervision_result_t::hm_cp_supervision_incorrect;
        status = true;
    }
    return status;
}

const hm_deadline_supervision_param_t& HMDeadlineSupervisionCfg::get_deadline_supervision_params() const
{
    return supervision_params_;
}
