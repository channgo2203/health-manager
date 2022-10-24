/**
 * @file HM_Status_Reporter.h
 * @author Van C. Ngo
 * @brief This file contains the supervision failure status reporter
 * @version 0.1
 * @date 2022-05-31
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include "HM_Types.h"
#include "HM_Status_Reporter_Intf.h"
#include "HM_TP_SupervisionStatusPubSubTypes.h"
#include "HM_Data_Sender_Intf.h"
#include "HM_Mode_Cfg_Handler_Intf.h"

#include <memory>
#include <functional>
#include <map>


class HMStatusReporter : public HMStatusReporterIntf
{
public:
    HMStatusReporter() = delete;

    virtual ~HMStatusReporter() = default;

    HMStatusReporter(std::shared_ptr<HMCPDataIntf> checkpoint_data,
                     std::shared_ptr<HMHSDataIntf> health_status_data,
                     std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data,
                     std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data,
                     std::unique_ptr<HMDataSenderIntf<HMTPSupervisionStatus>> status_sender);

    bool report_supervision_failure() override;
private:
    bool report_cp_supervision_failures(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_cfg,
                                        HMTPSupervisionStatus& status);
    bool report_hs_supervision_failures(const std::shared_ptr<const HMModeCfgHandlerIntf>& active_cfg,
                                        HMTPSupervisionStatus& status);
    void print_reporting_result(const HMTPSupervisionStatus& status);

    std::shared_ptr<HMCPDataIntf> checkpoint_data_{nullptr};
    std::shared_ptr<HMHSDataIntf> health_status_data_{nullptr};
    std::shared_ptr<HMSupervisedEntityDataIntf> supervised_entity_data_{nullptr};
    std::shared_ptr<HMGlobalSupervisionDataIntf> global_supervision_data_{nullptr};
    std::unique_ptr<HMDataSenderIntf<HMTPSupervisionStatus>> status_sender_{nullptr};
    bool is_valid_supervision_data{false};
    std::map<hm_supervised_entity_id_t, std::string> se_names_{};
    std::map<hm_external_supervision_id_t, std::string> es_names_{};
};
