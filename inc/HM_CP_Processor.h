/**
 * @file HM_CP_Processor.h
 * @author Van C. Ngo
 * @brief This file contains the CP processor
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

#include <iostream>
#include <atomic>
#include <vector>
#include <memory>

#include "HM_CP_Processor_Intf.h"
#include "HM_CP_Data_Intf.h"
#include "HM_HS_Data_Intf.h"
#include "HM_Supervised_Entity_Data_Intf.h"
#include "HM_Global_Supervision_Data_Intf.h"
#include "HM_CP_Supervision_Cfg_Intf.h"


class HMCPProcessor : public HMCPProcessorIntf
{
public:
    HMCPProcessor() = delete;

    virtual ~HMCPProcessor() = default;

    HMCPProcessor(std::shared_ptr<HMCPDataIntf> checkpoint_data,
                  std::shared_ptr<HMHSDataIntf> health_status_data,
                  std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data,
                  std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data);

    bool process_data(const HMTPCheckpoint& checkpoint) override;
private:
    bool process_cp(const hm_supervised_entity_id_t se_id,
                    const hm_checkpoint_id_t cp_id,
                    const uint64_t timestamp_ms,
                    const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>& cp_sups);
    std::shared_ptr<HMCPDataIntf> checkpoint_data_{nullptr};
    std::shared_ptr<HMHSDataIntf> health_status_data_{nullptr};
    std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data_{nullptr};
    std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data_{nullptr};
    bool is_valid_supervision_data{false};
};
