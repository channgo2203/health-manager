#include "HM_Alive_Supervision_Cfg.h"


HMAliveSupervisionCfg::HMAliveSupervisionCfg(const hm_alive_supervision_param_t& params)
    : supervision_params_(params)
{
}

hm_supervision_id_t HMAliveSupervisionCfg::get_supervision_id() const
{
    return supervision_params_.supervision_id;
}

bool HMAliveSupervisionCfg::is_contained(const hm_supervised_entity_id_t se_id,
                                         const hm_checkpoint_id_t cp_id) const
{
    const bool result = (supervision_params_.se_id == se_id) &&
                        (supervision_params_.cp_id == cp_id);
    return result;
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
                                                       hm_cp_supervision_result_t& result) const
{
    bool status{false};
    uint32_t cycle_total{0};
    if ((cp_data->get_supervision_cycle_total(supervision_params_.se_id, supervision_params_.cp_id, cycle_total)) && 
        (cycle_total >= supervision_params_.supervision_ref_cycles))
    {
        const auto supervised_indications = static_cast<uint32_t>(
                                            static_cast<double>(cycle_total * supervision_params_.expected_alive_indications) / 
                                            static_cast<double>(supervision_params_.supervision_ref_cycles));
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
