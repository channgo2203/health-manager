/**
 * @file HM_SupervisionStatus_Processor.cpp
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

#include <iostream>

#include "HM_SupervisionStatus_Processor.h"


bool HMSupervisionStatusProcessor::process_data(const HMTPSupervisionStatus& status)
{
    bool result{true};
    for (const auto& cp_status : status.cp_supervision_status())
    {
        print_tp_local_status(cp_status);
    }
    for (const auto& hs_status : status.hs_supervision_status())
    {
        print_tp_local_status(hs_status);
    }
    std::cout << "[HMSupervisionStatusProcessor] Global status : " << static_cast<unsigned int>(status.global_status()) << std::endl;
    return result;
}

void HMSupervisionStatusProcessor::print_tp_local_status(const HMTPLocalStatus& local_status)
{
    std::cout << "[HMSupervisionStatusProcessor] Local status for SE" << local_status.id() << " : ("
              << local_status.name() << ", " << static_cast<unsigned int>(local_status.local_status()) << ", "
              << static_cast<unsigned int>(local_status.recovery_type()) << ")" << std::endl;
}
