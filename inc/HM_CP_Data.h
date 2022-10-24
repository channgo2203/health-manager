/**
 * @file HM_CP_Data.h
 * @author Van C. Ngo
 * @brief This file contains the checkpoint data
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

#include "HM_CP_Data_Intf.h"
#include <mutex>
#include <tuple>

class HMCPData : public HMCPDataIntf
{
public:
    HMCPData() = delete;

    virtual ~HMCPData() = default;

    HMCPData(const std::vector<hm_supervised_entity_t>& ses,
             const std::vector<hm_supervision_id_t>& supervision_ids);

    bool is_contained(const hm_supervised_entity_id_t se_id,
                      const hm_checkpoint_id_t cp_id) override;

    bool get_alive_indications(const hm_supervised_entity_id_t se_id,
                               const hm_checkpoint_id_t cp_id,
                               uint32_t& indications) override;

    bool set_alive_indications(const hm_supervised_entity_id_t se_id,
                               const hm_checkpoint_id_t cp_id,
                               const uint32_t indications) override;

    bool increase_alive_indications(const hm_supervised_entity_id_t se_id,
                                    const hm_checkpoint_id_t cp_id) override;

    bool get_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                     const hm_checkpoint_id_t cp_id,
                                     uint32_t& cycle_total) override;

    bool set_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                     const hm_checkpoint_id_t cp_id,
                                     const uint32_t cycles) override;

    bool increase_supervision_cycle_total(const hm_supervised_entity_id_t se_id,
                                          const hm_checkpoint_id_t cp_id) override;

    bool get_supervision_time_total(const hm_supervised_entity_id_t se_id,
                                    const hm_checkpoint_id_t cp_id,
                                    uint64_t& supervision_time_total) override;

    bool set_supervision_time_total(const hm_supervised_entity_id_t se_id,
                                    const hm_checkpoint_id_t cp_id,
                                    const uint64_t supervision_time_total) override;

    bool get_deadline_timestamp(const hm_supervised_entity_id_t se_id,
                                const hm_checkpoint_id_t cp_id,
                                uint64_t& timestamp) override;

    bool set_deadline_timestamp(const hm_supervised_entity_id_t se_id,
                                const hm_checkpoint_id_t cp_id,
                                const uint64_t timestamp) override;

    bool get_supervision_time_point(const hm_supervised_entity_id_t se_id,
                                    const hm_checkpoint_id_t cp_id,
                                    std::chrono::time_point<std::chrono::steady_clock>& supervision_time) override;

    bool set_supervision_time_point(const hm_supervised_entity_id_t se_id,
                                    const hm_checkpoint_id_t cp_id,
                                    const std::chrono::time_point<std::chrono::steady_clock>& supervision_time) override;

    bool init_supervision_time_points(const std::chrono::time_point<std::chrono::steady_clock>& supervision_time) override;
private:
    bool is_valid_se_cp(const std::pair<hm_supervised_entity_id_t, hm_checkpoint_id_t>& key);
    std::map<std::pair<hm_supervised_entity_id_t, hm_checkpoint_id_t>, hm_checkpoint_data_t> checkpoint_data_{};
    std::map<hm_supervision_id_t, bool> logical_supervision_active_flags_{};
    std::map<hm_supervision_id_t, hm_checkpoint_id_t> logical_supervision_pre_visited_cp_{};
    std::mutex lock_{};
};
