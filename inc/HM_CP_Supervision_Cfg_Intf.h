#pragma once

#include "HM_Types.h"
#include "HM_CP_Data_Intf.h"


class HMCPSupervisionCfgIntf
{
public:
    HMCPSupervisionCfgIntf() = default;
    
    virtual ~HMCPSupervisionCfgIntf() = default;
    
    virtual hm_supervision_id_t get_supervision_id() const = 0;

    virtual bool is_contained(const hm_supervised_entity_id_t se_id,
                              const hm_checkpoint_id_t cp_id) const = 0;

    virtual hm_cp_type_t get_cp_supervision_type(const hm_supervised_entity_id_t se_id,
                                                 const hm_checkpoint_id_t cp_id) const = 0;

    virtual bool compute_supervision_result(const std::shared_ptr<HMCPDataIntf>& cp_data,
                                            hm_cp_supervision_result_t& result) const = 0;
};
