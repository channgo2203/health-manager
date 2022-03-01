#include "HM_CP_Data.h"

#include <thread>

bool HMCPData::init(const std::vector<hm_supervised_entity_t>& ses,
                    const std::vector<hm_supervision_id_t>& supervision_ids)
{
    // init checkpoint data
    for (const auto& se : ses)
    {
        for (const auto& cp_id : se.checkpoints)
        {
            hm_checkpoint_data_t cp_data{0u, 0u, std::chrono::time_point<std::chrono::system_clock>{}};
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

bool HMCPData::is_valid_se_cp(const std::pair<hm_supervised_entity_id_t, hm_checkpoint_id_t>& key)
{
    const auto iter = checkpoint_data_.find(key);
    return (iter != checkpoint_data_.end());
}

bool HMCPData::is_contained(const hm_supervised_entity_id_t se_id,
                            const hm_checkpoint_id_t cp_id)
{
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    return is_valid_se_cp(key);
}

bool HMCPData::get_alive_indications(const hm_supervised_entity_id_t se_id,
                                     const hm_checkpoint_id_t cp_id,
                                     uint32_t& indications)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        indications = checkpoint_data_[key].alive_indications;
        result = true;
    }
    return result;
}

bool HMCPData::set_alive_indications(const hm_supervised_entity_id_t se_id,
                                     const hm_checkpoint_id_t cp_id,
                                     const uint32_t indications)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        checkpoint_data_[key].alive_indications = indications;
        result = true;
    }
    return result;
}

bool HMCPData::increase_alive_indications(const hm_supervised_entity_id_t se_id,
                                          const hm_checkpoint_id_t cp_id)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        checkpoint_data_[key].alive_indications++;
        result = true;
    }
    return result;
}

bool HMCPData::get_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                           const hm_checkpoint_id_t cp_id,
                                           uint32_t& cycle_total)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        cycle_total = checkpoint_data_[key].supervision_cycle_total;
        result = true;
    }
    return result;
}

bool HMCPData::set_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                           const hm_checkpoint_id_t cp_id,
                                           const uint32_t cycles)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        checkpoint_data_[key].supervision_cycle_total = cycles;
        result = true;
    }
    return result;
}

bool HMCPData::increase_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                                const hm_checkpoint_id_t cp_id)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        checkpoint_data_[key].supervision_cycle_total++;
        result = true;
    }
    return result;
}

bool HMCPData::get_deadline_timestamp(const hm_supervised_entity_id_t se_id,
                                      const hm_checkpoint_id_t cp_id,
                                      std::chrono::time_point<std::chrono::system_clock>& timestamp)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        timestamp = checkpoint_data_[key].deadline_timestamp;
        result = true;
    }
    return result;
}

bool HMCPData::set_deadline_timestamp(const hm_supervised_entity_id_t se_id,
                                      const hm_checkpoint_id_t cp_id,
                                      const std::chrono::time_point<std::chrono::system_clock>& timestamp)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        checkpoint_data_[key].deadline_timestamp = timestamp;
        result = true;
    }
    return result;
}
