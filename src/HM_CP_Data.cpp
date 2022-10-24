/**
 * @file HM_CP_Data.cpp
 * @author Van C. Ngo
 * @brief This file contains the checkpoint data
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

#include "HM_CP_Data.h"

#include <thread>

HMCPData::HMCPData(const std::vector<hm_supervised_entity_t>& ses,
                   const std::vector<hm_supervision_id_t>& supervision_ids)
{
    // init checkpoint data
    for (const auto& se : ses)
    {
        for (const auto& cp_id : se.checkpoints)
        {
            hm_checkpoint_data_t cp_data{0u, 0u, 0u, 0u, {}};
            checkpoint_data_[std::make_pair(se.se_id, cp_id)] = cp_data;
        }
    }
    // init active flags
    for (const auto& sup_id : supervision_ids)
    {
        logical_supervision_active_flags_[sup_id] = false;
    }
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

bool HMCPData::get_supervision_time_total(const hm_supervised_entity_id_t se_id,
                                          const hm_checkpoint_id_t cp_id,
                                          uint64_t& supervision_time_total)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        supervision_time_total = checkpoint_data_[key].supervision_time_total;
        result = true;
    }
    return result;
}

bool HMCPData::set_supervision_time_total(const hm_supervised_entity_id_t se_id,
                                          const hm_checkpoint_id_t cp_id,
                                          const uint64_t supervision_time_total)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        checkpoint_data_[key].supervision_time_total = supervision_time_total;
        result = true;
    }
    return result;
}

bool HMCPData::get_deadline_timestamp(const hm_supervised_entity_id_t se_id,
                                      const hm_checkpoint_id_t cp_id,
                                      uint64_t& timestamp)
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
                                      const uint64_t timestamp)
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

bool HMCPData::get_supervision_time_point(const hm_supervised_entity_id_t se_id,
                                          const hm_checkpoint_id_t cp_id,
                                          std::chrono::time_point<std::chrono::steady_clock>& supervision_time)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        supervision_time = checkpoint_data_[key].supervision_time_point;
        result = true;
    }
    return result;
}

bool HMCPData::set_supervision_time_point(const hm_supervised_entity_id_t se_id,
                                          const hm_checkpoint_id_t cp_id,
                                          const std::chrono::time_point<std::chrono::steady_clock>& supervision_time)
{
    bool result{false};
    const auto key = std::make_pair(se_id, cp_id);
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se_cp(key))
    {
        checkpoint_data_[key].supervision_time_point = supervision_time;
        result = true;
    }
    return result;
}

bool HMCPData::init_supervision_time_points(const std::chrono::time_point<std::chrono::steady_clock>& supervision_time)
{
    std::lock_guard<std::mutex> guard(lock_);
    for (auto& cp_data : checkpoint_data_)
    {
        cp_data.second.supervision_time_point = supervision_time;
    }
    return true;
}
