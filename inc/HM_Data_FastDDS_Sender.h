/**
 * @file HM_Data_FastDDS_Sender.h
 * @author Van C. Ngo
 * @brief This file contains the data sender using FastDDS
 * @version 0.1
 * @date 2022-03-13
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include <string>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>


#include "HM_Data_Sender_Intf.h"


using namespace eprosima::fastdds::dds;

template <typename MsgType, typename MsgPubSubType>
class HMDataFastDDSSender : public HMDataSenderIntf<MsgType>
{
public:
    HMDataFastDDSSender() = delete;

    HMDataFastDDSSender(const std::string& participant_qos_name,
                        const std::string& msg_type_name,
                        const std::string& topic_name);

    virtual ~HMDataFastDDSSender();

    bool init() override;

    bool publish(const MsgType& data) override;
private:
    DomainParticipant* participant_;
    Publisher* publisher_;
    DataWriter* writer_;
    Topic* topic_;
    TypeSupport type_;
    MsgType current_data_;
    std::string participant_qos_name_;
    std::string msg_type_name_;
    std::string topic_name_;
    class PubListener : public DataWriterListener
    {
    public:
        PubListener() : matched_(0)
        {
        }

        ~PubListener() override
        {
        }

        /* When publisher and subscriber are bind */
        void on_publication_matched(DataWriter*, const PublicationMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                matched_ = info.total_count;
                std::cout << "[HMDataFastDDSSender] Data publisher matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                matched_ = info.total_count;
                std::cout << "[HMDataFastDDSSender] Data publisher unmatched." << std::endl;
            }
            else
            {
                std::cout << "[HMDataFastDDSSender] " << info.current_count_change
                          << " is not a valid value for PublicationMatchedStatus" << std::endl;
            }
        }
        std::atomic_int matched_;
    } listener_;
};
