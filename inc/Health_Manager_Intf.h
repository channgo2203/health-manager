/**
 * @file Health_Manager_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the interface of main class of HM
 * @version 0.1
 * @date 2022-03-20
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include <string>

#include "HM_Types.h"

class HealthManagerIntf
{
public:
    HealthManagerIntf() = default;

    virtual ~HealthManagerIntf() = default;

    virtual bool init() = 0;

    virtual bool deinit() = 0;

    virtual std::string get_version() = 0;

    virtual bool get_current_local_status(const hm_supervised_entity_id_t se_id,
                                          hm_local_status_t& local_status) = 0;

    virtual bool get_current_global_status(hm_global_status_t& global_status) = 0;

    virtual hm_mode_t get_mode() = 0;

    virtual bool set_mode(const hm_mode_t) = 0;
};
