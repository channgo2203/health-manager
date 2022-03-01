#include "HM_Deadline_Supervision_Cfg.h"


HMDeadlineSupervisionCfg::HMDeadlineSupervisionCfg(const hm_deadline_supervision_param_t& params)
    : supervision_params_(params)
{
}

hm_supervision_id_t HMDeadlineSupervisionCfg::get_supervision_id() const
{
    return supervision_params_.supervision_id;
}

bool HMDeadlineSupervisionCfg::is_contained(const hm_supervised_entity_id_t se_id,
                                            const hm_checkpoint_id_t cp_id) const
{
    const bool result = (supervision_params_.se_id == se_id) &&
                        ((supervision_params_.cp_start_id == cp_id) || 
                         (supervision_params_.cp_stop_id == cp_id));
    return result;
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
                                                          hm_cp_supervision_result_t& result) const
{
    bool status{false};
    // Check that both start and stop timestamps are valid
    const auto start_time_ms = timestamp_in_ms(supervision_params_.se_id, supervision_params_.cp_start_id, cp_data);
    const auto stop_time_ms = timestamp_in_ms(supervision_params_.se_id, supervision_params_.cp_stop_id, cp_data);
    const bool is_valid_timestamps = (start_time_ms > 0) && (stop_time_ms > 0);
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

uint32_t HMDeadlineSupervisionCfg::timestamp_in_ms(const hm_supervised_entity_id_t se_id,
                                                   const hm_checkpoint_id_t cp_id,
                                                   const std::shared_ptr<HMCPDataIntf>& cp_data) const
{
    uint32_t duration_ms{0};
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    if (cp_data->get_deadline_timestamp(se_id, cp_id, timestamp))
    {
        duration_ms = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()).count());
    }
    return duration_ms;
}
