#pragma once

#include "HM_Types.h"

class HMCPDataIntf
{
public:
    HMCPDataIntf() = default;
    
    virtual ~HMCPDataIntf() = default;
    
    virtual bool init(const std::vector<hm_supervised_entity_t>& ses,
                      const std::vector<hm_supervision_id_t>& supervision_ids) = 0;

    virtual bool is_contained(const hm_supervised_entity_id_t se_id,
                              const hm_checkpoint_id_t cp_id) const = 0;

    virtual uint32_t get_alive_indications() const = 0;

    virtual bool set_alive_indications(const uint32_t indications) = 0;

    virtual bool increase_alive_indications() = 0;

    virtual uint32_t get_supervision_cycle_total() const = 0;

    virtual bool set_supervision_cycle_total(const uint32_t cycles) = 0;

    virtual bool increase_supervision_cycle_total() = 0;
};
