/**
 * @file Health_Manager.h
 * @author Van C. Ngo
 * @brief This file contains the main class of HM
 * @version 0.1
 * @date 2022-03-20
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <memory>

#include "Health_Manager_Intf.h"
#include "HM_CP_Data_Intf.h"
#include "HM_HS_Data_Intf.h"
#include "HM_Supervised_Entity_Data_Intf.h"
#include "HM_Global_Supervision_Data_Intf.h"
#include "HM_CP_Processor_Intf.h"
#include "HM_Data_Receiver_Intf.h"
#include "HM_Local_Status_Processor_Intf.h"
#include "HM_Global_Status_Processor_Intf.h"
#include "HM_Failure_Handler_Intf.h"
#include "HM_Status_Reporter_Intf.h"
#include "HM_Component_Factory_Intf.h"


class HealthManager : public HealthManagerIntf
{
public:
    HealthManager() = delete;

    HealthManager(std::unique_ptr<HMComponentFactoryIntf> component_factory);

    virtual ~HealthManager();

    bool init() override;

    bool deinit() override;

    std::string get_version() override;

    bool get_current_local_status(const hm_supervised_entity_id_t se_id,
                                  hm_local_status_t& local_status) override;

    bool get_current_global_status(hm_global_status_t& global_status) override;

    hm_mode_t get_mode() override;

    bool set_mode(const hm_mode_t) override;

    void supervision_thread_function();
private:
    std::shared_ptr<HMCPDataIntf> checkpoint_data_{nullptr};
    std::shared_ptr<HMHSDataIntf> health_status_data_{nullptr};
    std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data_{nullptr};
    std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data_{nullptr};
    std::shared_ptr<HMCPProcessorIntf> cp_processor_{nullptr};
    std::shared_ptr<HMDataReceiverIntf<HMTPCheckpoint, HMCPProcessorIntf>> cp_receiver_{nullptr};
    std::shared_ptr<HMLocalStatusProcessorIntf> local_status_processor_{nullptr};
    std::shared_ptr<HMGlobalStatusProcessorIntf> global_status_processor_{nullptr};
    std::shared_ptr<HMFailureHandlerIntf> failure_handler_{nullptr};
    std::shared_ptr<HMStatusReporterIntf> status_reporter_{nullptr};
    std::unique_ptr<HMComponentFactoryIntf> component_factory_{nullptr};
    std::thread supervision_thread_{};
    std::atomic<bool> exit_flag_{false};
    std::atomic<bool> is_valid_system_{false};
    std::atomic<bool> is_initialized_{false};
    std::atomic<hm_mode_t> active_mode_id_{0u};
    std::atomic<uint32_t> supervision_cycle_time_{0u};
};
