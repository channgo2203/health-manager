/**
 * @file HM_Types.h
 * @author Van C. Ngo
 * @brief This file contains the HM types
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

#include <cstdint>
#include <string>
#include <ctime>
#include <chrono>
#include <map>
#include <memory>
#include <vector>


using hm_checkpoint_id_t = uint16_t;

using hm_supervised_entity_id_t = uint16_t;

using hm_mode_t = uint8_t;

using hm_external_supervision_id_t = uint16_t;

using hm_supervision_id_t = uint16_t;

using hm_checkpoint_graph_t = std::map<hm_checkpoint_id_t, std::vector<hm_checkpoint_id_t>>;

enum class hm_cp_supervision_result_t : uint8_t
{
    hm_cp_supervision_incorrect = 0u,
    hm_cp_supervision_correct = 1u
};

enum class hm_hs_supervision_result_t : uint8_t
{
    hm_hs_supervision_invalid = 0u,
    hm_hs_supervision_valid = 1u
};

enum class hm_cp_type_t : uint8_t
{
    hm_alive_supervision_cp = 0u,
    hm_deadline_supervision_start_cp = 1u,
    hm_deadline_supervision_stop_cp = 2u,
    hm_logical_supervision_begin_cp = 3u,
    hm_logical_supervision_end_cp = 4u,
    hm_logical_supervision_other_cp = 5u,
    hm_unknown_supervision_cp = 6u
};

enum class hm_supervision_type_t : uint8_t
{
    hm_alive_supervision = 0u,
    hm_deadline_supervision = 1u,
    hm_logical_supervision = 2u,
    hm_health_status_supervision = 3u
};

enum class hm_hs_type_t : uint8_t
{
    hm_health_status_low = 0u,
    hm_health_status_high = 1u,
    hm_health_status_ok = 2u,
    hm_health_status_very_low = 3u,
    hm_health_status_very_high = 4u
};

enum class hm_cp_failure_recovery_t : uint8_t
{
    hm_failure_recovery_nothing = 0u,
    hm_failure_recovery_reset = 1u,
    hm_failure_recovery_restart = 2u,
    hm_failure_recovery_backup = 3u
};

enum class hm_hs_invalid_reaction_t : uint8_t
{
    hm_hs_invalid_nothing = 0u,
    hm_hs_invalid_reset = 1u
};

enum class hm_supervised_entity_type_t : uint8_t
{
    hm_normal_supervised_entity = 0u,
    hm_primary_supervised_entity = 1u,
    hm_hot_backup_supervised_entity = 2u,
    hm_cold_backup_supervised_entity = 3u
};

enum class hm_local_status_t : uint8_t
{
    hm_local_status_ok = 0u,
    hm_local_status_failed = 1u,
    hm_local_status_expired = 2u,
    hm_local_status_restarted = 3u,
    hm_local_status_backup = 4u,
    hm_local_status_recovered = 5u,
    hm_local_status_deactivated = 6u
};

enum class hm_global_status_t : uint8_t
{
    hm_global_status_ok = 0u,
    hm_global_status_failed = 1u,
    hm_global_status_expired = 2u,
    hm_global_status_stopped = 3u,
    hm_global_status_deactivated = 4u
};

struct hm_cp_failure_recovery_param_t
{
    hm_supervised_entity_id_t se_id;
    hm_cp_failure_recovery_t recovery_action;
    uint32_t recovery_restart_limit;
    uint32_t recovery_backup_limit;
    hm_supervised_entity_id_t primary_id;
    hm_supervised_entity_id_t backup_id;
};

struct hm_hs_invalid_reaction_param_t
{
    hm_external_supervision_id_t es_id;
    hm_hs_invalid_reaction_t hs_action;
};

struct hm_supervised_entity_t
{
    hm_supervised_entity_id_t se_id;
    std::string se_name;
    hm_supervised_entity_type_t type;
    hm_supervised_entity_id_t primary_se_id;
    hm_supervised_entity_id_t backup_se_id;
    std::vector<hm_checkpoint_id_t> checkpoints;
};

struct hm_external_supervision_t
{
    hm_external_supervision_id_t es_id;
    std::string es_name;
};

struct hm_local_status_param_t
{
    hm_supervised_entity_id_t se_id;
    uint32_t failure_alive_supervision_cycle_tolerance;
    uint32_t restarted_supervision_cycle_tolerance;
    uint32_t backup_supervision_cycle_tolerance;
};

struct hm_checkpoint_data_t
{
    uint32_t alive_indications;
    uint32_t supervision_cycle_total;
    uint64_t supervision_time_total;
    uint64_t deadline_timestamp;
    std::chrono::time_point<std::chrono::steady_clock> supervision_time_point;
};

struct hm_supervised_entity_data_t
{
    uint32_t failed_alive_supervision_cycles;
    uint32_t failed_alive_supervision_count;
    uint32_t failed_deadline_supervision_count;
    uint32_t failed_logical_supervision_count;
    uint32_t restarted_cycle_count;
    uint32_t restarted_time_count;
    uint32_t backup_cycle_count;
    uint32_t backup_time_count;
    hm_local_status_t local_status;
};

struct hm_hs_external_supervision_data_t
{
    uint32_t failed_hs_supervision_count;
    hm_local_status_t local_status;
};

struct hm_watchdog_t
{
    std::string wdg_name;
    uint8_t wdg_index;
};

struct hm_global_supervision_data_t
{
    hm_mode_t active_mode_id;
    hm_global_status_t global_status;
    uint32_t num_local_status_failed;
    uint32_t num_local_status_expired;
    uint32_t global_status_expired_count;
};

struct hm_alive_supervision_param_t
{
    uint32_t expected_alive_indications;
    uint32_t min_margin;
    uint32_t max_margin;
    uint32_t supervision_ref_cycles;
    hm_supervised_entity_id_t se_id;
    hm_checkpoint_id_t cp_id;
    hm_supervision_id_t supervision_id;
};

struct hm_deadline_supervision_param_t
{
    uint32_t deadline_min;
    uint32_t deadline_max;
    hm_supervised_entity_id_t se_id;
    hm_checkpoint_id_t cp_start_id;
    hm_checkpoint_id_t cp_stop_id;
    hm_supervision_id_t supervision_id;
};

struct hm_logical_supervision_param_t
{
    hm_checkpoint_graph_t cp_graph;
    std::vector<hm_checkpoint_id_t> begin_cp_ids;
    std::vector<hm_checkpoint_id_t> end_cp_ids;
    hm_supervised_entity_id_t se_id;
    hm_supervision_id_t supervision_id;
};

struct hm_hs_supervision_param_t
{
    std::vector<hm_hs_type_t> valid_status;
    hm_external_supervision_id_t es_id;
};
