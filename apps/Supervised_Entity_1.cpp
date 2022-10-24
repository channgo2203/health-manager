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


#define SUPERVISED_ENTITY_1_ID   1u
#define CHECKPOINT_ID_1_0        0u
#define CHECKPOINT_ID_1_1        1u
#define CHECKPOINT_ID_1_2        2u
#define CHECKPOINT_ID_1_3        3u
#define ABNORMAL_CYCLES          60u

void run(const uint32_t abnormal_cycle,
         const std::shared_ptr<HMDataSenderIntf<HMTPCheckpoint>>& p_sender)
{
    uint32_t cycle_count{0u};
    HMTPCheckpoint cp;
    cp.se_id(SUPERVISED_ENTITY_1_ID);
    while (cycle_count < abnormal_cycle)
    {
        bool pub_result_0{false};
        bool pub_result_1{false};
        bool pub_result_2{false};
        bool pub_result_3{false};
        uint64_t duration_between_cps{0u};
        cp.cp_id(CHECKPOINT_ID_1_0);
        auto now_timestamp = std::chrono::system_clock::now();
        auto now_timestamp_ms = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>
                                                      (now_timestamp.time_since_epoch()).count());
        cp.timestamp_ms(now_timestamp_ms);
        // Report the checkpoint (1, 0)
        if (p_sender->publish(cp))
        {
            pub_result_0 = true;
            std::cout << "[Supervised_Entity_1] Reported checkpoint ("
                      << cp.se_id() << ", " << cp.cp_id() << ")"
                      << " at timestamp "<< now_timestamp_ms << std::endl;
        }
        // Simulate some work within around 500ms
        duration_between_cps = (rand() % 20 + 500);
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_between_cps));
        // Report the checkpoint (1, 1)
        cp.cp_id(CHECKPOINT_ID_1_1);
        now_timestamp_ms += duration_between_cps;
        cp.timestamp_ms(now_timestamp_ms);
        if (p_sender->publish(cp))
        {
            pub_result_1 = true;
            std::cout << "[Supervised_Entity_1] Reported checkpoint ("
                      << cp.se_id() << ", " << cp.cp_id() << ")"
                      << " after "<< duration_between_cps << "ms" << std::endl;
        }
        // Simulate some work within around 500ms
        duration_between_cps = (rand() % 20 + 500);
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_between_cps));
        // Report the checkpoint (1, 2)
        cp.cp_id(CHECKPOINT_ID_1_2);
        now_timestamp_ms += duration_between_cps;
        cp.timestamp_ms(now_timestamp_ms);
        if (p_sender->publish(cp))
        {
            pub_result_2 = true;
            std::cout << "[Supervised_Entity_1] Reported checkpoint ("
                      << cp.se_id() << ", " << cp.cp_id() << ")"
                      << " after "<< duration_between_cps << "ms" << std::endl;
        }
        // Report the checkpoint (1, 3)
        if (cycle_count == abnormal_cycle - 1)
        {
            // Simulate a long delay, then crash
            duration_between_cps = 1000u;
            std::this_thread::sleep_for(std::chrono::milliseconds(duration_between_cps));
        }
        else
        {
            duration_between_cps = (rand() % 20 + 500);;
            std::this_thread::sleep_for(std::chrono::milliseconds(duration_between_cps));
        }
        cp.cp_id(CHECKPOINT_ID_1_3);
        now_timestamp_ms += duration_between_cps;
        cp.timestamp_ms(now_timestamp_ms);
        if (p_sender->publish(cp))
        {
            pub_result_3 = true;
            std::cout << "[Supervised_Entity_1] Reported checkpoint ("
                      << cp.se_id() << ", " << cp.cp_id() << ")"
                      << " after "<< duration_between_cps << "ms" << std::endl;
        }
        if (pub_result_0 && pub_result_1 && pub_result_2 && pub_result_3)
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
        std::cout << "[Supervised_Entity_1] Successfully initialized CP sender" << std::endl;
        // Simulate some initialization
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        run(ABNORMAL_CYCLES, p_sender);
        return 0;
    }
    return 1;
}
