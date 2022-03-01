#pragma once

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include "HM_CP_Receiver_Intf.h"
#include "HM_CP_Processor_Intf.h"

using namespace eprosima::fastdds::dds;

class HMCPReceiver : public HMCPReceiverIntf
{
public:
    HMCPReceiver();
    
    virtual ~HMCPReceiver();
    
    bool init(std::shared_ptr<HMCPProcessorIntf> cp_processor) override;
private:
    DomainParticipant* participant_;
    Subscriber* subscriber_;
    DataReader* reader_;
    Topic* topic_;
    TypeSupport type_;
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
                std::cout << "Subscriber matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                std::cout << "Subscriber unmatched." << std::endl;
            }
            else
            {
                std::cout << info.current_count_change << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
            }
        }

        /* When data arrives */
        void on_data_available(DataReader* reader) override
        {
            SampleInfo info;
            if (reader->take_next_sample(&checkpoint_, &info) == ReturnCode_t::RETCODE_OK)
            {
                if (info.valid_data)
                {
                    /* Process the received data */
                    const auto se_id = static_cast<hm_supervised_entity_id_t>(checkpoint_.se_id());
                    const auto cp_id = static_cast<hm_checkpoint_id_t>(checkpoint_.cp_id());
                    std::cout << "Received checkpoint(" << se_id << ", " << cp_id << ")" << std::endl;
                    cp_processor_->process_reached_cp(se_id, cp_id);
                }
            }
        }

        void init(std::shared_ptr<HMCPProcessorIntf> processor)
        {
            cp_processor_ = std::move(processor);
        }

    private:
        std::shared_ptr<HMCPProcessorIntf> cp_processor_{nullptr};
        HMTPCheckpoint checkpoint_{};
    } listener_;
};
