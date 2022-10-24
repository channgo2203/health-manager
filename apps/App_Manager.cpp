/**
 * @file App_Manager.cpp
 * @author Van C. Ngo
 * @brief This file contains a fake application manager
 * @version 0.1
 * @date 2022-05-22
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
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <map>
#include <mutex>

#include "HM_Cfg_Handler.h"
#include "HM_Data_Receiver_Intf.h"
#include "HM_Data_FastDDS_Receiver.h"
#include "HM_RecoveryCmd_Processor_Intf.h"
#include "AM_RecoveryCmd_Processor.h"


const std::vector<int> term_signals = {SIGINT, SIGQUIT, SIGTERM};

int main(int argc, char** argv)
{
    sigset_t term_signal_set;
    sigemptyset(&term_signal_set);
    // Add terminating signals into the set
    for (const auto& sig : term_signals)
    {
        sigaddset(&term_signal_set, sig);
    }
    sigprocmask(SIG_BLOCK, &term_signal_set, nullptr);
    // Create recovery cmd receiver
    std::unique_ptr<HMDataReceiverIntf<HMTPRecoveryCmd, HMRecoveryCmdProcessorIntf>> p_receiver =
        std::make_unique<HMDataFastDDSReceiver<HMTPRecoveryCmd,
                                               HMRecoveryCmdProcessorIntf,
                                               HMTPRecoveryCmdPubSubType>>(HMCfgHandler::get_recovery_cmd_subscriber_qos_name(),
                                                                           HMCfgHandler::get_recovery_cmd_msg_type_name(),
                                                                           HMCfgHandler::get_recovery_cmd_topic_name());
    // Create recovery cmd processor
    std::shared_ptr<HMRecoveryCmdProcessorIntf> p_cmd_processor = std::make_shared<AMRecoveryCmdProcessor>();
    if ((p_receiver == nullptr) || (p_cmd_processor == nullptr))
    {
        std::cout << "[AppManager] Failed to create cmd receiver" << std::endl;
        return 1;
    }
    if (!p_receiver->init(p_cmd_processor))
    {
        std::cout << "[AppManager] Failed to initialize recovery cmd receiver" << std::endl;
        return 1;
    }
    std::cout << "[AppManager] Successfully run app manager" << std::endl;
    // Wait for a terminating signal
    int term_signal;
    const auto waiting_result = sigwait(&term_signal_set, &term_signal);
    if (waiting_result == -1)
    {
        std::cout << "[AppManager] Failed to wait for terminating signals" << std::endl;
    }
    else
    {
        std::cout << "[AppManager] Received signal number " <<  term_signal << std::endl;
    }
    return 0;
}
