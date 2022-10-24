/**
 * @file HM_Failure_Handler_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the failure handler interface
 * @version 0.1
 * @date 2022-05-23
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
#include "HM_CP_Data_Intf.h"
#include "HM_HS_Data_Intf.h"
#include "HM_Supervised_Entity_Data_Intf.h"
#include "HM_Global_Supervision_Data_Intf.h"


class HMFailureHandlerIntf
{
public:
    HMFailureHandlerIntf() = default;

    virtual ~HMFailureHandlerIntf() = default;

    virtual bool handle_failure_recovery() = 0;

    virtual bool set_wdg_trigger_condition(const hm_watchdog_t& wdg, const uint16_t timeout_ms) = 0;
};
