/**
 * @file HM_Component_Factory_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the HM component abstract factory
 * @version 0.1
 * @date 2022-03-21
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include <memory>

#include "HM_Types.h"
#include "Health_Manager_Intf.h"
#include "HM_CP_Data_Intf.h"
#include "HM_HS_Data_Intf.h"
#include "HM_Supervised_Entity_Data_Intf.h"
#include "HM_Global_Supervision_Data_Intf.h"
#include "HM_CP_Processor_Intf.h"
#include "HM_Data_Receiver_Intf.h"
#include "HM_TP_CheckpointPubSubTypes.h"
#include "HM_Data_Sender_Intf.h"
#include "HM_TP_RecoveryCmdPubSubTypes.h"
#include "HM_TP_SupervisionStatusPubSubTypes.h"
#include "HM_Local_Status_Processor_Intf.h"
#include "HM_Global_Status_Processor_Intf.h"
#include "HM_Failure_Handler_Intf.h"
#include "HM_Status_Reporter_Intf.h"


class HMComponentFactoryIntf
{
public:
    HMComponentFactoryIntf() = default;

    virtual ~HMComponentFactoryIntf() = default;

    virtual std::shared_ptr<HMCPDataIntf> create_checkpoint_data() = 0;

    virtual std::shared_ptr<HMSupervisedEntityDataIntf> create_se_data() = 0;

    virtual std::shared_ptr<HMHSDataIntf> create_hs_data() = 0;

    virtual std::shared_ptr<HMGlobalSupervisionDataIntf> create_global_sup_data(const hm_mode_t mode_id) = 0;

    virtual std::shared_ptr<HMCPProcessorIntf> create_cp_processor(std::shared_ptr<HMCPDataIntf> cp_data,
                                                                   std::shared_ptr<HMSupervisedEntityDataIntf> se_data,
                                                                   std::shared_ptr<HMHSDataIntf> hs_data,
                                                                   std::shared_ptr<HMGlobalSupervisionDataIntf> gs_data) = 0;

    virtual std::shared_ptr<HMDataReceiverIntf<HMTPCheckpoint, HMCPProcessorIntf>> create_cp_receiver() = 0;

    virtual std::shared_ptr<HMLocalStatusProcessorIntf>
        create_local_status_processor(std::shared_ptr<HMCPDataIntf> cp_data,
                                      std::shared_ptr<HMSupervisedEntityDataIntf> se_data,
                                      std::shared_ptr<HMHSDataIntf> hs_data,
                                      std::shared_ptr<HMGlobalSupervisionDataIntf> gs_data) = 0;

    virtual std::shared_ptr<HMGlobalStatusProcessorIntf>
        create_global_status_processor(std::shared_ptr<HMCPDataIntf> cp_data,
                                       std::shared_ptr<HMSupervisedEntityDataIntf> se_data,
                                       std::shared_ptr<HMHSDataIntf> hs_data,
                                       std::shared_ptr<HMGlobalSupervisionDataIntf> gs_data) = 0;

    virtual std::unique_ptr<HMDataSenderIntf<HMTPRecoveryCmd>> create_recovery_cmd_sender() = 0;

    virtual std::shared_ptr<HMFailureHandlerIntf>
        create_failure_handler(std::shared_ptr<HMCPDataIntf> cp_data,
                               std::shared_ptr<HMSupervisedEntityDataIntf> se_data,
                               std::shared_ptr<HMHSDataIntf> hs_data,
                               std::shared_ptr<HMGlobalSupervisionDataIntf> gs_data,
                               std::unique_ptr<HMDataSenderIntf<HMTPRecoveryCmd>> recovery_cmd_sender) = 0;

    virtual std::unique_ptr<HMDataSenderIntf<HMTPSupervisionStatus>> create_supervision_status_sender() = 0;

    virtual std::shared_ptr<HMStatusReporterIntf>
        create_status_reporter(std::shared_ptr<HMCPDataIntf> cp_data,
                               std::shared_ptr<HMSupervisedEntityDataIntf> se_data,
                               std::shared_ptr<HMHSDataIntf> hs_data,
                               std::shared_ptr<HMGlobalSupervisionDataIntf> gs_data,
                               std::unique_ptr<HMDataSenderIntf<HMTPSupervisionStatus>> status_sender) = 0;
};
