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
                              const hm_checkpoint_id_t cp_id) = 0;

    virtual bool get_alive_indications(const hm_supervised_entity_id_t se_id,
                                       const hm_checkpoint_id_t cp_id,
                                       uint32_t& indications) = 0;

    virtual bool set_alive_indications(const hm_supervised_entity_id_t se_id,
                                       const hm_checkpoint_id_t cp_id,
                                       const uint32_t indications) = 0;

    virtual bool increase_alive_indications(const hm_supervised_entity_id_t se_id,
                                            const hm_checkpoint_id_t cp_id) = 0;

    virtual bool get_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                             const hm_checkpoint_id_t cp_id,
                                             uint32_t& cycle_total) = 0;

    virtual bool set_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                             const hm_checkpoint_id_t cp_id,
                                             const uint32_t cycles) = 0;

    virtual bool increase_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                                  const hm_checkpoint_id_t cp_id) = 0;

    virtual bool get_deadline_timestamp(const hm_supervised_entity_id_t se_id,
                                        const hm_checkpoint_id_t cp_id,
                                        std::chrono::time_point<std::chrono::system_clock>& timestamp) = 0;

    virtual bool set_deadline_timestamp(const hm_supervised_entity_id_t se_id,
                                        const hm_checkpoint_id_t cp_id,
                                        const std::chrono::time_point<std::chrono::system_clock>& timestamp) = 0;
};
