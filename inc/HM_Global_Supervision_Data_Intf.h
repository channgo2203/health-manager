/**
 * @file HM_Global_Supervision_Data_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the global supervision data interface
 * @version 0.1
 * @date 2022-03-17
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

class HMGlobalSupervisionDataIntf
{
public:
    HMGlobalSupervisionDataIntf() = default;

    virtual ~HMGlobalSupervisionDataIntf() = default;

    virtual bool get_active_mode_id(hm_mode_t& mode_id) = 0;

    virtual bool set_active_mode_id(const hm_mode_t mode_id) = 0;

    virtual bool get_num_local_status_failed(uint32_t& failed_local_status) = 0;

    virtual bool set_num_local_status_failed(const uint32_t failed_local_status) = 0;

    virtual bool increase_num_local_status_failed() = 0;

    virtual bool get_num_local_status_expired(uint32_t& expired_local_status) = 0;

    virtual bool set_num_local_status_expired(const uint32_t expired_local_status) = 0;

    virtual bool increase_num_local_status_expired() = 0;

    virtual bool get_global_status_expired_count(uint32_t& count) = 0;

    virtual bool set_global_status_expired_count(const uint32_t count) = 0;

    virtual bool increase_global_status_expired_count() = 0;

    virtual bool get_global_status(hm_global_status_t& status) = 0;

    virtual bool set_global_status(const hm_global_status_t status) = 0;
};
