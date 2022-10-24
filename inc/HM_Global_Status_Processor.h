/**
 * @file HM_Global_Status_Processor.h
 * @author Van C. Ngo
 * @brief This file contains the global status processor
 * @version 0.1
 * @date 2022-04-09
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include "HM_Global_Status_Processor_Intf.h"
#include "HM_Mode_Cfg_Handler_Intf.h"

#include <memory>
#include <functional>
#include <map>

using hm_global_status_func_map = std::map<hm_global_status_t, std::function<bool(
    const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg)>>;


class HMGlobalStatusProcessor : public HMGlobalStatusProcessorIntf
{
public:
    HMGlobalStatusProcessor() = delete;

    virtual ~HMGlobalStatusProcessor() = default;

    HMGlobalStatusProcessor(std::shared_ptr<HMCPDataIntf> checkpoint_data,
                            std::shared_ptr<HMHSDataIntf> health_status_data,
                            std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data,
                            std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data);

    bool process_global_status() override;

    static const std::string& global_status_to_string(const hm_global_status_t status);
private:
    bool update_global_status_deactivated(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg);

    bool update_global_status_ok(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg);

    bool update_global_status_failed(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg);

    bool update_global_status_expired(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg);

    bool update_global_status_stopped(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg);

    void print_global_status();

    std::shared_ptr<HMCPDataIntf> checkpoint_data_{nullptr};
    std::shared_ptr<HMHSDataIntf> health_status_data_{nullptr};
    std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data_{nullptr};
    std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data_{nullptr};
    hm_global_status_func_map update_status_func_map_{};
    bool is_valid_supervision_data{false};
};
