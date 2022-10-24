/**
 * @file Status_Receiver.cpp
 * @author Van C. Ngo
 * @brief This file contains a fake application that receives supervision status
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
#include <string>
#include <memory>
#include <thread>
#include <cstdint>
#include <chrono>
#include <cstdlib>
#include <csignal>

#include "HM_Cfg_Handler.h"
#include "HM_Data_Receiver_Intf.h"
#include "HM_Data_FastDDS_Receiver.h"
#include "HM_SupervisionStatus_Processor_Intf.h"
#include "HM_SupervisionStatus_Processor.h"


const std::vector<int> term_signals = {SIGINT, SIGQUIT, SIGTERM};

int main(int argc, char** argv)
{
    int ret_val{0};
    sigset_t term_signal_set;
    sigemptyset(&term_signal_set);
    // Add terminating signals into the set
    for (const auto& sig : term_signals)
    {
        sigaddset(&term_signal_set, sig);
    }
    sigprocmask(SIG_BLOCK, &term_signal_set, nullptr);
    // Create supervision status receiver
    std::unique_ptr<HMDataReceiverIntf<HMTPSupervisionStatus, HMSupervisionStatusProcessorIntf>> p_receiver =
        std::make_unique<HMDataFastDDSReceiver<HMTPSupervisionStatus,
                                               HMSupervisionStatusProcessorIntf,
                                               HMTPSupervisionStatusPubSubType>>(HMCfgHandler::get_sup_status_subscriber_qos_name(),
                                                                                 HMCfgHandler::get_sup_status_msg_type_name(),
                                                                                 HMCfgHandler::get_sup_status_topic_name());
    // Create supervision status processor
    std::shared_ptr<HMSupervisionStatusProcessorIntf> p_status_processor = std::make_shared<HMSupervisionStatusProcessor>();
    if ((p_receiver != nullptr) && (p_status_processor != nullptr) && p_receiver->init(p_status_processor))
    {
        std::cout << "[Status_Receiver] Successfully initialized supervision status receiver" << std::endl;
        // Wait for a terminating signal
        int term_signal;
        const auto waiting_result = sigwait(&term_signal_set, &term_signal);
        if (waiting_result == -1)
        {
            std::cout << "[Status_Receiver] Failed to wait for terminating signals" << std::endl;
            ret_val = 1;
        }
        else
        {
            std::cout << "[Status_Receiver] Received signal number " <<  term_signal << std::endl;
        }
    }
    return 0;
}
