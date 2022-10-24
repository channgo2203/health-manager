/**
 * @file HM_Deadline_Supervision_Cfg.h
 * @author Van C. Ngo
 * @brief This file contains the deadline supervision
 * @version 0.1
 * @date 2022-03-10
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include "HM_CP_Supervision_Cfg_Intf.h"


class HMDeadlineSupervisionCfg : public HMCPSupervisionCfgIntf
{
public:
    HMDeadlineSupervisionCfg() = delete;

    virtual ~HMDeadlineSupervisionCfg() = default;

    HMDeadlineSupervisionCfg(const hm_deadline_supervision_param_t& params);

    hm_supervision_id_t get_supervision_id() const override;

    hm_supervised_entity_id_t get_se_id() const override;

    const std::vector<hm_checkpoint_id_t>& get_checkpoint_ids() const override;

    bool is_contained(const hm_supervised_entity_id_t se_id,
                      const hm_checkpoint_id_t cp_id) const override;

    hm_supervision_type_t get_supervision_type() const override;

    hm_cp_type_t get_cp_supervision_type(const hm_supervised_entity_id_t se_id,
                                         const hm_checkpoint_id_t cp_id) const override;

    bool compute_supervision_result(const std::shared_ptr<HMCPDataIntf>& cp_data,
                                    hm_cp_supervision_result_t& result,
                                    const uint32_t cycle_time = 0u) const override;

    const hm_deadline_supervision_param_t& get_deadline_supervision_params() const override;
private:
    hm_deadline_supervision_param_t supervision_params_{};
    std::vector<hm_checkpoint_id_t> cp_ids_{};
};
