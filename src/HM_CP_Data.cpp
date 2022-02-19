#include "HM_CP_Data.h"

bool HMCPData::init(const std::vector<hm_supervised_entity_t>& ses,
                    const std::vector<hm_supervision_id_t>& supervision_ids)
{
    // init checkpoint data
    for (const auto& se : ses)
    {
        for (const auto& cp_id : se.checkpoints)
        {
            hm_checkpoint_data_t cp_data{0u, 0u, std::chrono::time_point<std::chrono::system_clock>(), 0u};
            checkpoint_data_[std::make_pair(se.se_id, cp_id)] = cp_data;
        }
    }
    // init active flags
    for (const auto& sup_id : supervision_ids)
    {
        logical_supervision_active_flags_[sup_id] = false;
    }
    return true;
}

bool HMCPData::is_contained(const hm_supervised_entity_id_t se_id,
                            const hm_checkpoint_id_t cp_id) const
{
    return false;
}

uint32_t HMCPData::get_alive_indications() const
{
    return 0;
}

bool HMCPData::set_alive_indications(const uint32_t indications)
{
    return false;
}

bool HMCPData::increase_alive_indications()
{
    return false;
}

uint32_t HMCPData::get_supervision_cycle_total() const
{
    return 0;
}

bool HMCPData::set_supervision_cycle_total(const uint32_t cycles)
{
    return false;
}

bool HMCPData::increase_supervision_cycle_total()
{
    return false;
}
