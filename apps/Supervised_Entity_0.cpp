/**
 * @file Supervised_Entity_0.cpp
 * @author Van C. Ngo
 * @brief This file contains an example of SE
 * @version 0.1
 * @date 2022-03-20
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

#include "HM_TP_CheckpointPubSubTypes.h"
#include "HM_TP_RecoveryCmdPubSubTypes.h"
#include "HM_Data_Sender_Intf.h"
#include "HM_Data_FastDDS_Sender.h"
#include "HM_Cfg_Handler.h"

#define SUPERVISED_ENTITY_0_ID   0u
#define CHECKPOINT_ID_0_0        0u
#define CHECKPOINT_ID_0_1        1u
#define TOTAL_NORMAL_CYCLES_0    100u

void run(const uint32_t cycles,
         const std::shared_ptr<HMDataSenderIntf<HMTPCheckpoint>>& p_sender)
{
    uint32_t cycle_count{0u};
    HMTPCheckpoint cp;
    cp.se_id(SUPERVISED_ENTITY_0_ID);
    while (cycle_count < cycles)
    {
        bool pub_result_0{false};
        bool pub_result_1{false};
        // Simulate some work within around 200ms
        auto work_time = rand() % 30 + 180;
        std::this_thread::sleep_for(std::chrono::milliseconds(work_time));
        // Report the checkpoint
        cp.cp_id(CHECKPOINT_ID_0_0);
        auto now_timestamp = std::chrono::system_clock::now();
        auto now_timestamp_ms = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>
                                                      (now_timestamp.time_since_epoch()).count());
        cp.timestamp_ms(now_timestamp_ms);
        if (p_sender->publish(cp))
        {
            pub_result_0 = true;
            std::cout << "[Supervised_Entity_0] Reported checkpoint ("
                      << cp.se_id() << ", " << cp.cp_id() << ")"
                      << " after " << work_time << "ms" << std::endl;
        }
        // Simulate some work within around 400ms
        work_time += (rand() % 30 + 180);
        std::this_thread::sleep_for(std::chrono::milliseconds(work_time));
        // Report the checkpoint
        cp.cp_id(CHECKPOINT_ID_0_1);
        now_timestamp = std::chrono::system_clock::now();
        now_timestamp_ms = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>
                                                 (now_timestamp.time_since_epoch()).count());
        cp.timestamp_ms(now_timestamp_ms);
        if (p_sender->publish(cp))
        {
            pub_result_1 = true;
            std::cout << "[Supervised_Entity_0] Reported checkpoint ("
                      << cp.se_id() << ", " << cp.cp_id() << ")"
                      << " after " << work_time << "ms" << std::endl;
        }
        if (pub_result_0 && pub_result_1)
        {
            cycle_count++;
        }
    }
};

int main(int argc, char** argv)
{
    std::shared_ptr<HMDataSenderIntf<HMTPCheckpoint>> p_sender =
        std::make_shared<HMDataFastDDSSender<HMTPCheckpoint, HMTPCheckpointPubSubType>>(
            HMCfgHandler::get_cp_publisher_qos_name(),
            HMCfgHandler::get_cp_msg_type_name(),
            HMCfgHandler::get_cp_topic_name());
    if ((p_sender != nullptr) && p_sender->init())
    {
        std::cout << "[Supervised_Entity_0] Successfully initialized CP sender" << std::endl;
        // Simulate some initialization
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        run(TOTAL_NORMAL_CYCLES_0, p_sender);
        return 0;
    }
    return 1;
}
