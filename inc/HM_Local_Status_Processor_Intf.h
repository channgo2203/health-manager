/**
 * @file HM_Local_Status_Processor_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the local status processor interface
 * @version 0.1
 * @date 2022-03-23
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


class HMLocalStatusProcessorIntf
{
public:
    HMLocalStatusProcessorIntf() = default;

    virtual ~HMLocalStatusProcessorIntf() = default;

    virtual bool process_local_status() = 0;
};
