/**
 * @file HM_Data_FastDDS_Receiver.cpp
 * @author Van C. Ngo
 * @brief This file contains the CP receiver using FastDDS
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

#include "HM_Data_FastDDS_Receiver.h"

#include "HM_CP_Processor_Intf.h"
#include "HM_RecoveryCmd_Processor_Intf.h"
#include "HM_SupervisionStatus_Processor_Intf.h"
#include "HM_TP_CheckpointPubSubTypes.h"
#include "HM_TP_RecoveryCmdPubSubTypes.h"
#include "HM_TP_SupervisionStatusPubSubTypes.h"


using namespace eprosima::fastdds::dds;


template <typename MsgType, typename MsgProcType, typename MsgPubSubType>
HMDataFastDDSReceiver<MsgType, MsgProcType, MsgPubSubType>::HMDataFastDDSReceiver(
    const std::string& participant_qos_name,
    const std::string& msg_type_name,
    const std::string& topic_name)
    : participant_(nullptr),
      subscriber_(nullptr),
      topic_(nullptr),
      reader_(nullptr),
      type_(new MsgPubSubType()),
      participant_qos_name_(participant_qos_name),
      msg_type_name_(msg_type_name),
      topic_name_(topic_name)
{
}

template <typename MsgType, typename MsgProcType, typename MsgPubSubType>
HMDataFastDDSReceiver<MsgType, MsgProcType, MsgPubSubType>::~HMDataFastDDSReceiver()
{
    if (reader_ != nullptr)
    {
        subscriber_->delete_datareader(reader_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }
    if (subscriber_ != nullptr)
    {
        participant_->delete_subscriber(subscriber_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

template <typename MsgType, typename MsgProcType, typename MsgPubSubType>
bool HMDataFastDDSReceiver<MsgType, MsgProcType, MsgPubSubType>::init(
    std::shared_ptr<MsgProcType> data_processor)
{
    listener_.init(std::move(data_processor));
    DomainParticipantQos participantQos;
    participantQos.name(participant_qos_name_);
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
    if (participant_ == nullptr)
    {
        return false;
    }
    // Register the Type
    type_.register_type(participant_);
    // Create the subscriptions topic
    topic_ = participant_->create_topic(topic_name_, msg_type_name_, TOPIC_QOS_DEFAULT);
    if (topic_ == nullptr)
    {
        return false;
    }
    // Create the subscriber
    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
    if (subscriber_ == nullptr)
    {
        return false;
    }
    // Create the datareader
    reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);
    if (reader_ == nullptr)
    {
        return false;
    }
    return true;
}

template class HMDataFastDDSReceiver<HMTPCheckpoint,
                                     HMCPProcessorIntf,
                                     HMTPCheckpointPubSubType>;
template class HMDataFastDDSReceiver<HMTPRecoveryCmd,
                                     HMRecoveryCmdProcessorIntf,
                                     HMTPRecoveryCmdPubSubType>;
template class HMDataFastDDSReceiver<HMTPSupervisionStatus,
                                     HMSupervisionStatusProcessorIntf,
                                     HMTPSupervisionStatusPubSubType>;
