/**
 * @file HM_Main.cpp
 * @author Van C. Ngo
 * @brief This file contains the entry point of HM module
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

#include <iostream>
#include <memory>
#include <vector>
#include <csignal>

#include "HM_Component_Factory_Intf.h"
#include "HM_Component_Factory.h"
#include "Health_Manager_Intf.h"
#include "Health_Manager.h"


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

    // Create a component factory
    std::unique_ptr<HMComponentFactoryIntf> p_hm_factory = std::make_unique<HMComponentFactory>();
    // Create an HM object
    std::shared_ptr<HealthManagerIntf> p_hm = std::make_shared<HealthManager>(std::move(p_hm_factory));
    if ((p_hm != nullptr) && p_hm->init())
    {
        // Wait for a terminating signal
        int term_signal;
        const auto waiting_result = sigwait(&term_signal_set, &term_signal);
        if (waiting_result == -1)
        {
            std::cout << "[HMMain] Failed to wait for terminating signals" << std::endl;
            ret_val = 1;
        }
        else
        {
            std::cout <<"[HMMain] Received signal number " <<  term_signal << std::endl;
        }
        p_hm->deinit();
    }
    return ret_val;
}
