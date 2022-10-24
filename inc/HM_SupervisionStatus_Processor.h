/**
 * @file HM_SupervisionStatus_Processor.h
 * @author Van C. Ngo
 * @brief This file contains the status processor
 * @version 0.1
 * @date 2022-06-02
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include <iostream>
#include <atomic>
#include <vector>
#include <memory>

#include "HM_SupervisionStatus_Processor_Intf.h"


class HMSupervisionStatusProcessor : public HMSupervisionStatusProcessorIntf
{
public:
    HMSupervisionStatusProcessor() = default;

    virtual ~HMSupervisionStatusProcessor() = default;

    bool process_data(const HMTPSupervisionStatus& status) override;
private:
    void print_tp_local_status(const HMTPLocalStatus& local_status);
};
