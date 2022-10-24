/**
 * @file HM_Status_Reporter_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the supervision failure status reporter interface
 * @version 0.1
 * @date 2022-05-31
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


class HMStatusReporterIntf
{
public:
    HMStatusReporterIntf() = default;

    virtual ~HMStatusReporterIntf() = default;

    virtual bool report_supervision_failure() = 0;
};
