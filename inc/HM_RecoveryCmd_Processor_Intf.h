/**
 * @file HM_RecoveryCmd_Processor_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the recovey cmd processor interface
 * @version 0.1
 * @date 2022-05-24
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

#include "HM_TP_RecoveryCmdPubSubTypes.h"


class HMRecoveryCmdProcessorIntf
{
public:
    HMRecoveryCmdProcessorIntf() = default;

    virtual ~HMRecoveryCmdProcessorIntf() = default;

    virtual bool process_data(const HMTPRecoveryCmd& recovery_cmd) = 0;
};
