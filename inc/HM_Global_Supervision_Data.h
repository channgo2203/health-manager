/**
 * @file HM_Global_Supervision_Data.h
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

#pragma once

#include "HM_Types.h"
#include "HM_Global_Supervision_Data_Intf.h"
#include <mutex>


class HMGlobalSupervisionData : public HMGlobalSupervisionDataIntf
{
public:
    HMGlobalSupervisionData() = delete;

    HMGlobalSupervisionData(const hm_mode_t mode_id);

    virtual ~HMGlobalSupervisionData() = default;

    bool get_active_mode_id(hm_mode_t& mode_id) override;

    bool set_active_mode_id(const hm_mode_t mode_id) override;

    bool get_num_local_status_failed(uint32_t& failed_local_status) override;

    bool set_num_local_status_failed(const uint32_t failed_local_status) override;

    bool increase_num_local_status_failed() override;

    bool get_num_local_status_expired(uint32_t& expired_local_status) override;

    bool set_num_local_status_expired(const uint32_t expired_local_status) override;

    bool increase_num_local_status_expired() override;

    bool get_global_status_expired_count(uint32_t& count) override;

    bool set_global_status_expired_count(const uint32_t count) override;

    bool increase_global_status_expired_count() override;

    bool get_global_status(hm_global_status_t& status) override;

    bool set_global_status(const hm_global_status_t status) override;
private:
    hm_global_supervision_data_t global_supervision_data_{};
    std::mutex lock_{};
};
