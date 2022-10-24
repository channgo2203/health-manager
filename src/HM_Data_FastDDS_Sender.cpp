/**
 * @file HM_Data_FastDDS_Sender.cpp
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

#include "HM_Data_FastDDS_Sender.h"

#include "HM_TP_CheckpointPubSubTypes.h"
#include "HM_TP_RecoveryCmdPubSubTypes.h"
#include "HM_TP_SupervisionStatusPubSubTypes.h"


using namespace eprosima::fastdds::dds;

template <typename MsgType, typename MsgPubSubType>
HMDataFastDDSSender<MsgType, MsgPubSubType>::HMDataFastDDSSender(
    const std::string& participant_qos_name,
    const std::string& msg_type_name,
    const std::string& topic_name)
    : participant_(nullptr),
      publisher_(nullptr),
      topic_(nullptr),
      writer_(nullptr),
      type_(new MsgPubSubType()),
      participant_qos_name_(participant_qos_name),
      msg_type_name_(msg_type_name),
      topic_name_(topic_name)
{
}

template <typename MsgType, typename MsgPubSubType>
HMDataFastDDSSender<MsgType, MsgPubSubType>::~HMDataFastDDSSender()
{
    if (writer_ != nullptr)
    {
        publisher_->delete_datawriter(writer_);
    }
    if (publisher_ != nullptr)
    {
        participant_->delete_publisher(publisher_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

template <typename MsgType, typename MsgPubSubType>
bool HMDataFastDDSSender<MsgType, MsgPubSubType>::init()
{
    DomainParticipantQos participantQos;
    participantQos.name(participant_qos_name_);
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
    if (participant_ == nullptr)
    {
        return false;
    }
    // Register the Type
    type_.register_type(participant_);
    // Create the publication topic
    topic_ = participant_->create_topic(topic_name_, msg_type_name_, TOPIC_QOS_DEFAULT);
    if (topic_ == nullptr)
    {
        return false;
    }
    // Create the publisher
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
    if (publisher_ == nullptr)
    {
        return false;
    }
    // Create the datawriter
    writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);
    if (writer_ == nullptr)
    {
        return false;
    }
    return true;
}

template <typename MsgType, typename MsgPubSubType>
bool HMDataFastDDSSender<MsgType, MsgPubSubType>::publish(const MsgType& data)
{
    // Pubish only when there are subcribers binding
    if (listener_.matched_ > 0)
    {
        current_data_ = data;
        writer_->write(&current_data_);
        return true;
    }
    return false;
}

template class HMDataFastDDSSender<HMTPCheckpoint, HMTPCheckpointPubSubType>;
template class HMDataFastDDSSender<HMTPRecoveryCmd, HMTPRecoveryCmdPubSubType>;
template class HMDataFastDDSSender<HMTPSupervisionStatus, HMTPSupervisionStatusPubSubType>;
