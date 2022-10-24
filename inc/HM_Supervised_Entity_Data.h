/**
 * @file HM_Supervised_Enity_Data.h
 * @author Van C. Ngo
 * @brief This file contains the SE data
 * @version 0.1
 * @date 2022-03-15
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
#include "HM_Supervised_Entity_Data_Intf.h"
#include <mutex>

class HMSupervisedEntityData : public HMSupervisedEntityDataIntf
{
public:
    HMSupervisedEntityData() = delete;

    virtual ~HMSupervisedEntityData() = default;

    HMSupervisedEntityData(const std::vector<hm_supervised_entity_t>& ses);

    bool is_contained(const hm_supervised_entity_id_t se_id) override;

    bool get_failed_alive_supervision_count(const hm_supervised_entity_id_t se_id,
                                            uint32_t& count) override;

    bool set_failed_alive_supervision_count(const hm_supervised_entity_id_t se_id,
                                            const uint32_t count) override;

    bool increase_failed_alive_supervision_count(const hm_supervised_entity_id_t se_id) override;

    bool get_failed_alive_supervision_cycles(const hm_supervised_entity_id_t se_id,
                                             uint32_t& cycles) override;

    bool set_failed_alive_supervision_cycles(const hm_supervised_entity_id_t se_id,
                                             const uint32_t cycles) override;

    bool increase_failed_alive_supervision_cycles(const hm_supervised_entity_id_t se_id) override;

    bool get_failed_deadline_supervision_count(const hm_supervised_entity_id_t se_id,
                                               uint32_t& count) override;

    bool set_failed_deadline_supervision_count(const hm_supervised_entity_id_t se_id,
                                               const uint32_t count) override;

    bool increase_failed_deadline_supervision_count(const hm_supervised_entity_id_t se_id) override;

    bool get_failed_logical_supervision_count(const hm_supervised_entity_id_t se_id,
                                              uint32_t& count) override;

    bool set_failed_logical_supervision_count(const hm_supervised_entity_id_t se_id,
                                              const uint32_t count) override;

    bool increase_failed_logical_supervision_count(const hm_supervised_entity_id_t se_id) override;

    bool get_restarted_cycle_count(const hm_supervised_entity_id_t se_id,
                                   uint32_t& count) override;

    bool set_restarted_cycle_count(const hm_supervised_entity_id_t se_id,
                                   const uint32_t count) override;

    bool increase_restarted_cycle_count(const hm_supervised_entity_id_t se_id) override;

    bool get_backup_cycle_count(const hm_supervised_entity_id_t se_id,
                                uint32_t& count) override;

    bool set_backup_cycle_count(const hm_supervised_entity_id_t se_id,
                                const uint32_t count) override;

    bool increase_backup_cycle_count(const hm_supervised_entity_id_t se_id) override;

    bool get_backup_time_count(const hm_supervised_entity_id_t se_id,
                               uint32_t& count) override;

    bool set_backup_time_count(const hm_supervised_entity_id_t se_id,
                               const uint32_t count) override;

    bool increase_backup_time_count(const hm_supervised_entity_id_t se_id) override;

    bool get_restarted_time_count(const hm_supervised_entity_id_t se_id,
                                  uint32_t& count) override;

    bool set_restarted_time_count(const hm_supervised_entity_id_t se_id,
                                  const uint32_t count) override;

    bool increase_restarted_time_count(const hm_supervised_entity_id_t se_id) override;

    bool get_local_status(const hm_supervised_entity_id_t se_id,
                          hm_local_status_t& status) override;

    bool set_local_status(const hm_supervised_entity_id_t se_id,
                          const hm_local_status_t status) override;
private:
    bool is_valid_se(const hm_supervised_entity_id_t se_id);
    std::map<hm_supervised_entity_id_t, hm_supervised_entity_data_t> supervised_entity_data_{};
    std::mutex lock_{};
};
