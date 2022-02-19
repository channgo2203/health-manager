#pragma once

#include "HM_CP_Supervision_Cfg_Intf.h"


class HMAliveSupervisionCfg : public HMCPSupervisionCfgIntf
{
public:
    HMAliveSupervisionCfg() = delete;
    
    virtual ~HMAliveSupervisionCfg() = default;
    
    HMAliveSupervisionCfg(const hm_alive_supervision_param_t& params);

    hm_supervision_id_t get_supervision_id() const override;

    bool is_contained(const hm_supervised_entity_id_t se_id,
                      const hm_checkpoint_id_t cp_id) const override;

    hm_cp_type_t get_cp_supervision_type(const hm_supervised_entity_id_t se_id,
                                         const hm_checkpoint_id_t cp_id) const override;

    bool compute_supervision_result(const std::shared_ptr<HMCPDataIntf>& cp_data,
                                    hm_cp_supervision_result_t& result) const override;

private:
    hm_alive_supervision_param_t supervision_params_{};
};
