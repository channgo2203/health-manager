#pragma once

#include "HM_Types.h"

class HMCPProcessorIntf
{
public:
    HMCPProcessorIntf() = default;
    virtual ~HMCPProcessorIntf() = default;
    virtual bool process_reached_cp(const hm_supervised_entity_id_t, const hm_checkpoint_id_t) = 0;
};
