#pragma once

#include <iostream>

#include "HM_CP_Processor_Intf.h"

class HMCPProcessor : public HMCPProcessorIntf
{
public:
    HMCPProcessor() = default;
    virtual ~HMCPProcessor() = default;
    bool process_reached_cp(const hm_supervised_entity_id_t, const hm_checkpoint_id_t) override;
private:
};
