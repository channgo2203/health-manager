/**
 * @file HM_Data_FastDDS_Receiver.h
 * @author Van C. Ngo
 * @brief This file contains the data receiver using FastDDS
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

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include "HM_Data_Receiver_Intf.h"

using namespace eprosima::fastdds::dds;

template <typename MsgType, typename MsgProcType, typename MsgPubSubType>
class HMDataFastDDSReceiver : public HMDataReceiverIntf<MsgType, MsgProcType>
{
public:
    HMDataFastDDSReceiver() = delete;

    HMDataFastDDSReceiver(const std::string& participant_qos_name,
                          const std::string& msg_type_name,
                          const std::string& topic_name);

    virtual ~HMDataFastDDSReceiver();

    bool init(std::shared_ptr<MsgProcType> data_processor) override;
private:
    DomainParticipant* participant_;
    Subscriber* subscriber_;
    DataReader* reader_;
    Topic* topic_;
    TypeSupport type_;
    std::string participant_qos_name_;
    std::string msg_type_name_;
    std::string topic_name_;
    class SubListener : public DataReaderListener
    {
    public:
        SubListener() = default;

        ~SubListener() override = default;

        /* When publisher and subscriber are bind */
        void on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                std::cout << "[HMDataFastDDSReceiver] Data subscriber matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                std::cout << "[HMDataFastDDSReceiver] Data subscriber unmatched." << std::endl;
            }
            else
            {
                std::cout << "[HMDataFastDDSReceiver] " << info.current_count_change
                          << " is not a valid value for SubscriptionMatchedStatus" << std::endl;
            }
        }

        /* When data arrives */
        void on_data_available(DataReader* reader) override
        {
            SampleInfo info;
            // Keep taking data until there is nothing to take
            while (reader->take_next_sample(&data_, &info) == ReturnCode_t::RETCODE_OK)
            {
                // Process the received data
                if (info.valid_data && (data_processor_ != nullptr))
                {
                    data_processor_->process_data(data_);
                }
            }
        }

        void init(std::shared_ptr<MsgProcType> processor)
        {
            data_processor_ = std::move(processor);
        }

    private:
        std::shared_ptr<MsgProcType> data_processor_{nullptr};
        MsgType data_{};
    } listener_;
};
