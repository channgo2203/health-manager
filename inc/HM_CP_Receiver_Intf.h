#pragma once

#include "HM_Types.h"

#include "HM_TP_CheckpointPubSubTypes.h"

#include "HM_CP_Processor_Intf.h"

class HMCPReceiverIntf
{
public:
    HMCPReceiverIntf() = default;
    
    virtual ~HMCPReceiverIntf() = default;
    
    virtual bool init(std::shared_ptr<HMCPProcessorIntf> cp_processor) = 0;
};
