/**
 * @file HM_Supervised_Enity_Data.cpp
 * @author Van C. Ngo
 * @brief This file contains the SE data
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include "HM_Supervised_Entity_Data.h"
#include "HM_Cfg_Handler.h"


HMSupervisedEntityData::HMSupervisedEntityData(const std::vector<hm_supervised_entity_t>& ses)
{
    const auto init_mode_id = HMCfgHandler::get_init_mode_id();
    hm_supervised_entity_type_t se_type{hm_supervised_entity_type_t::hm_normal_supervised_entity};
    hm_supervised_entity_data_t se_data{0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, hm_local_status_t::hm_local_status_deactivated};
    for (const auto& se : ses)
    {
        if (HMCfgHandler::get_se_type(se.se_id, se_type))
        {
            const auto is_under_supervision = HMCfgHandler::is_se_contained(init_mode_id, se.se_id) &&
                (se_type != hm_supervised_entity_type_t::hm_hot_backup_supervised_entity) &&
                (se_type != hm_supervised_entity_type_t::hm_cold_backup_supervised_entity);
            if (is_under_supervision)
            {
                se_data.local_status = hm_local_status_t::hm_local_status_ok;
            }
            else
            {
                se_data.local_status = hm_local_status_t::hm_local_status_deactivated;
            }
        }
        supervised_entity_data_[se.se_id] = se_data;
    }
}

bool HMSupervisedEntityData::is_valid_se(const hm_supervised_entity_id_t se_id)
{
    const auto iter = supervised_entity_data_.find(se_id);
    return (iter != supervised_entity_data_.end());
}

bool HMSupervisedEntityData::is_contained(const hm_supervised_entity_id_t se_id)
{
    std::lock_guard<std::mutex> guard(lock_);
    return is_valid_se(se_id);
}

bool HMSupervisedEntityData::get_failed_alive_supervision_count(const hm_supervised_entity_id_t se_id,
                                                                uint32_t& count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        count = supervised_entity_data_[se_id].failed_alive_supervision_count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::set_failed_alive_supervision_count(const hm_supervised_entity_id_t se_id,
                                                                const uint32_t count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].failed_alive_supervision_count = count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::increase_failed_alive_supervision_count(const hm_supervised_entity_id_t se_id)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].failed_alive_supervision_count++;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::get_failed_alive_supervision_cycles(const hm_supervised_entity_id_t se_id,
                                                                 uint32_t& cycles)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        cycles = supervised_entity_data_[se_id].failed_alive_supervision_cycles;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::set_failed_alive_supervision_cycles(const hm_supervised_entity_id_t se_id,
                                                                 const uint32_t cycles)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].failed_alive_supervision_cycles = cycles;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::increase_failed_alive_supervision_cycles(const hm_supervised_entity_id_t se_id)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].failed_alive_supervision_cycles++;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::get_failed_deadline_supervision_count(const hm_supervised_entity_id_t se_id,
                                                                   uint32_t& count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        count = supervised_entity_data_[se_id].failed_deadline_supervision_count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::set_failed_deadline_supervision_count(const hm_supervised_entity_id_t se_id,
                                                                   const uint32_t count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].failed_deadline_supervision_count = count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::increase_failed_deadline_supervision_count(const hm_supervised_entity_id_t se_id)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].failed_deadline_supervision_count++;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::get_failed_logical_supervision_count(const hm_supervised_entity_id_t se_id,
                                                                  uint32_t& count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        count = supervised_entity_data_[se_id].failed_logical_supervision_count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::set_failed_logical_supervision_count(const hm_supervised_entity_id_t se_id,
                                                                  const uint32_t count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].failed_logical_supervision_count = count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::increase_failed_logical_supervision_count(const hm_supervised_entity_id_t se_id)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].failed_logical_supervision_count++;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::get_restarted_cycle_count(const hm_supervised_entity_id_t se_id,
                               uint32_t& count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        count = supervised_entity_data_[se_id].restarted_cycle_count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::set_restarted_cycle_count(const hm_supervised_entity_id_t se_id,
                               const uint32_t count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].restarted_cycle_count = count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::increase_restarted_cycle_count(const hm_supervised_entity_id_t se_id)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].restarted_cycle_count++;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::get_backup_cycle_count(const hm_supervised_entity_id_t se_id,
                            uint32_t& count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        count = supervised_entity_data_[se_id].backup_cycle_count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::set_backup_cycle_count(const hm_supervised_entity_id_t se_id,
                            const uint32_t count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].backup_cycle_count = count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::increase_backup_cycle_count(const hm_supervised_entity_id_t se_id)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].backup_cycle_count++;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::get_backup_time_count(const hm_supervised_entity_id_t se_id,
                                                   uint32_t& count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        count = supervised_entity_data_[se_id].backup_time_count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::set_backup_time_count(const hm_supervised_entity_id_t se_id,
                                                   const uint32_t count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].backup_time_count = count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::increase_backup_time_count(const hm_supervised_entity_id_t se_id)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].backup_time_count++;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::get_restarted_time_count(const hm_supervised_entity_id_t se_id,
                                                      uint32_t& count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        count = supervised_entity_data_[se_id].restarted_time_count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::set_restarted_time_count(const hm_supervised_entity_id_t se_id,
                                                      const uint32_t count)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].restarted_time_count = count;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::increase_restarted_time_count(const hm_supervised_entity_id_t se_id)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].restarted_time_count++;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::get_local_status(const hm_supervised_entity_id_t se_id,
                                              hm_local_status_t& status)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        status = supervised_entity_data_[se_id].local_status;
        result = true;
    }
    return result;
}

bool HMSupervisedEntityData::set_local_status(const hm_supervised_entity_id_t se_id,
                                              const hm_local_status_t status)
{
    bool result{false};
    std::lock_guard<std::mutex> guard(lock_);
    if (is_valid_se(se_id))
    {
        supervised_entity_data_[se_id].local_status = status;
        result = true;
    }
    return result;
}
