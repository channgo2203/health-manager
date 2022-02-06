#pragma once

#include "HM_Cfg_Types.h"

class HMModeConfig {
public:
    HMModeConfig() = delete;
    HMModeConfig(const HMModeConfig&) = delete;
    HMModeConfig& operator=(const HMModeConfig&) = delete;
    virtual ~HMModeConfig() = default;
    
    static uint16_t get_count();
private:
    static const uint16_t count_;
};