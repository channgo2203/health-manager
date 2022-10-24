/**
 * @file HM_CP_Processor_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the CP processor interface
 * @version 0.1
 * @date 2022-03-12
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

#include "HM_TP_CheckpointPubSubTypes.h"


class HMCPProcessorIntf
{
public:
    HMCPProcessorIntf() = default;

    virtual ~HMCPProcessorIntf() = default;

    virtual bool process_data(const HMTPCheckpoint& checkpoint) = 0;
};
