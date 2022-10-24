/**
 * @file HM_Local_Status_Processor.h
 * @author Van C. Ngo
 * @brief This file contains the local status processor
 * @version 0.1
 * @date 2022-03-23
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include "HM_Local_Status_Processor_Intf.h"
#include "HM_Mode_Cfg_Handler_Intf.h"

#include <memory>
#include <functional>
#include <map>

using hm_local_status_func_map = std::map<hm_local_status_t, std::function<bool(const hm_local_status_param_t&)>>;

class HMLocalStatusProcessor : public HMLocalStatusProcessorIntf
{
public:
    HMLocalStatusProcessor() = delete;

    virtual ~HMLocalStatusProcessor() = default;

    HMLocalStatusProcessor(std::shared_ptr<HMCPDataIntf> checkpoint_data,
                           std::shared_ptr<HMHSDataIntf> health_status_data,
                           std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data,
                           std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data);

    bool process_local_status() override;

    static const std::string& local_status_to_string(const hm_local_status_t status);
private:
    bool update_se_local_status_deactivated(const hm_local_status_param_t& se_local_params);
    bool update_se_local_status_ok(const hm_local_status_param_t& se_local_params);
    bool update_se_local_status_failed(const hm_local_status_param_t& se_local_params);
    bool update_se_local_status_expired(const hm_local_status_param_t& se_local_params);
    bool update_se_local_status_restarted(const hm_local_status_param_t& se_local_params);
    bool update_se_local_status_recovered(const hm_local_status_param_t& se_local_params);
    bool update_se_local_status_backup(const hm_local_status_param_t& se_local_params);

    bool compute_se_alive_supervision_results(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_mode_cfg);
    bool update_se_local_status(const hm_local_status_param_t& se_local_params);
    bool update_es_local_health_status(const hm_hs_supervision_param_t& hs_sup_params);

    void print_local_status(const hm_supervised_entity_id_t se_id);

    std::shared_ptr<HMCPDataIntf> checkpoint_data_{nullptr};
    std::shared_ptr<HMHSDataIntf> health_status_data_{nullptr};
    std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data_{nullptr};
    std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data_{nullptr};
    hm_local_status_func_map update_status_func_map_{};
    bool is_valid_supervision_data{false};
};
