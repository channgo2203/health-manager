#include "HM_CP_Receiver.h"

using namespace eprosima::fastdds::dds;


HMCPReceiver::HMCPReceiver()
    : participant_(nullptr),
    subscriber_(nullptr),
    topic_(nullptr),
    reader_(nullptr),
    type_(new HMTPCheckpointPubSubType())
{
}

HMCPReceiver::~HMCPReceiver()
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

bool HMCPReceiver::init(std::shared_ptr<HMCPProcessorIntf> cp_processor)
{
    listener_.init(std::move(cp_processor));
    DomainParticipantQos participantQos;
    participantQos.name("CP_Participant_Subscriber");
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
    if (participant_ == nullptr)
    {
        return false;
    }
    // Register the Type
    type_.register_type(participant_);
    // Create the subscriptions topic
    topic_ = participant_->create_topic("HM_CP_Topic", "HMTPCheckpoint", TOPIC_QOS_DEFAULT);
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
