/**
 * @file HM_Global_Supervision_Data.cpp
 * @author Van C. Ngo
 * @brief This file contains global supervision data
 * @version 0.1
 * @date 2022-03-16
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include "HM_Global_Supervision_Data.h"

#include <thread>


HMGlobalSupervisionData::HMGlobalSupervisionData(const hm_mode_t mode_id)
{
    global_supervision_data_.active_mode_id = mode_id;
    global_supervision_data_.global_status = hm_global_status_t::hm_global_status_deactivated;
    global_supervision_data_.num_local_status_failed = 0u;
    global_supervision_data_.num_local_status_expired = 0u;
    global_supervision_data_.global_status_expired_count = 0u;
}

bool HMGlobalSupervisionData::get_active_mode_id(hm_mode_t& mode_id)
{
    std::lock_guard<std::mutex> guard(lock_);
    mode_id = global_supervision_data_.active_mode_id;
    return true;
}

bool HMGlobalSupervisionData::set_active_mode_id(const hm_mode_t mode_id)
{
    std::lock_guard<std::mutex> guard(lock_);
    global_supervision_data_.active_mode_id = mode_id;
    return true;
}

bool HMGlobalSupervisionData::get_num_local_status_failed(uint32_t& failed_local_status)
{
    std::lock_guard<std::mutex> guard(lock_);
    failed_local_status = global_supervision_data_.num_local_status_failed;
    return true;
}

bool HMGlobalSupervisionData::set_num_local_status_failed(const uint32_t failed_local_status)
{
    std::lock_guard<std::mutex> guard(lock_);
    global_supervision_data_.num_local_status_failed = failed_local_status;
    return true;
}

bool HMGlobalSupervisionData::increase_num_local_status_failed()
{
    std::lock_guard<std::mutex> guard(lock_);
    global_supervision_data_.num_local_status_failed++;
    return true;
}

bool HMGlobalSupervisionData::get_num_local_status_expired(uint32_t& expired_local_status)
{
    std::lock_guard<std::mutex> guard(lock_);
    expired_local_status = global_supervision_data_.num_local_status_expired;
    return true;
}

bool HMGlobalSupervisionData::set_num_local_status_expired(const uint32_t expired_local_status)
{
    std::lock_guard<std::mutex> guard(lock_);
    global_supervision_data_.num_local_status_expired = expired_local_status;
    return true;
}

bool HMGlobalSupervisionData::increase_num_local_status_expired()
{
    std::lock_guard<std::mutex> guard(lock_);
    global_supervision_data_.num_local_status_expired++;
    return true;
}

bool HMGlobalSupervisionData::get_global_status_expired_count(uint32_t& count)
{
    std::lock_guard<std::mutex> guard(lock_);
    count = global_supervision_data_.global_status_expired_count;
    return true;
}

bool HMGlobalSupervisionData::set_global_status_expired_count(const uint32_t count)
{
    std::lock_guard<std::mutex> guard(lock_);
    global_supervision_data_.global_status_expired_count = count;
    return true;
}

bool HMGlobalSupervisionData::increase_global_status_expired_count()
{
    std::lock_guard<std::mutex> guard(lock_);
    global_supervision_data_.global_status_expired_count++;
    return true;
}

bool HMGlobalSupervisionData::get_global_status(hm_global_status_t& status)
{
    std::lock_guard<std::mutex> guard(lock_);
    status = global_supervision_data_.global_status;
    return true;
}

bool HMGlobalSupervisionData::set_global_status(const hm_global_status_t status)
{
    std::lock_guard<std::mutex> guard(lock_);
    global_supervision_data_.global_status = status;
    return true;
}
