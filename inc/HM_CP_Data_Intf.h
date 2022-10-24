/**
 * @file HM_CP_Data_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the checkpoint data interface
 * @version 0.1
 * @date 2022-03-09
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

class HMCPDataIntf
{
public:
    HMCPDataIntf() = default;

    virtual ~HMCPDataIntf() = default;

    virtual bool is_contained(const hm_supervised_entity_id_t se_id,
                              const hm_checkpoint_id_t cp_id) = 0;

    virtual bool get_alive_indications(const hm_supervised_entity_id_t se_id,
                                       const hm_checkpoint_id_t cp_id,
                                       uint32_t& indications) = 0;

    virtual bool set_alive_indications(const hm_supervised_entity_id_t se_id,
                                       const hm_checkpoint_id_t cp_id,
                                       const uint32_t indications) = 0;

    virtual bool increase_alive_indications(const hm_supervised_entity_id_t se_id,
                                            const hm_checkpoint_id_t cp_id) = 0;

    virtual bool get_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                             const hm_checkpoint_id_t cp_id,
                                             uint32_t& cycle_total) = 0;

    virtual bool set_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                             const hm_checkpoint_id_t cp_id,
                                             const uint32_t cycles) = 0;

    virtual bool increase_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                                  const hm_checkpoint_id_t cp_id) = 0;

    virtual bool get_supervision_time_total(const hm_supervised_entity_id_t se_id,
                                            const hm_checkpoint_id_t cp_id,
                                            uint64_t& supervision_time_total) = 0;

    virtual bool set_supervision_time_total(const hm_supervised_entity_id_t se_id,
                                            const hm_checkpoint_id_t cp_id,
                                            const uint64_t supervision_time_total) = 0;

    virtual bool get_deadline_timestamp(const hm_supervised_entity_id_t se_id,
                                        const hm_checkpoint_id_t cp_id,
                                        uint64_t& timestamp) = 0;

    virtual bool set_deadline_timestamp(const hm_supervised_entity_id_t se_id,
                                        const hm_checkpoint_id_t cp_id,
                                        const uint64_t timestamp) = 0;

    virtual bool get_supervision_time_point(const hm_supervised_entity_id_t se_id,
                                            const hm_checkpoint_id_t cp_id,
                                            std::chrono::time_point<std::chrono::steady_clock>& supervision_time) = 0;

    virtual bool set_supervision_time_point(const hm_supervised_entity_id_t se_id,
                                            const hm_checkpoint_id_t cp_id,
                                            const std::chrono::time_point<std::chrono::steady_clock>& supervision_time) = 0;

    virtual bool init_supervision_time_points(const std::chrono::time_point<std::chrono::steady_clock>& supervision_time) = 0;
};
