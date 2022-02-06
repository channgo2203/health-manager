#pragma once

#include <string>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "HM_Types.h"

enum class hm_failure_recovery_t : uint8_t {
    hm_failure_recovery_nothing = 0u,
    hm_failure_recovery_reset = 1u,
    hm_alive_failure_restart = 2u,
    hm_alive_failure_backup = 3u,
    hm_deadline_failure_restart = 4u,
    hm_deadline_failure_backup = 5u,
    hm_logical_failure_restart = 6u,
    hm_logical_failure_backup = 7u
};

struct hm_supervised_entity_t {
    hm_supervised_entity_id_t se_id;
    std::vector<hm_checkpoint_id_t> checkpoints;
};

struct hm_alive_supervision_t {
    uint16_t expected_alive_indicators;
    uint8_t min_margin;
    uint8_t max_margin;
    uint16_t supervision_ref_cycles;
    hm_supervised_entity_id_t se_id;
    hm_checkpoint_id_t cp_id;
};

struct hm_deadline_supervision_t {
    uint32_t deadline_min;
    uint32_t deadline_max;
    hm_supervised_entity_id_t se_id;
    hm_checkpoint_id_t deadline_cp_start_id;
    hm_checkpoint_id_t deadline_cp_stop_id;
};

struct hm_logical_supervision_t {
    bool active_flag;
    hm_supervised_entity_id_t se_id;
    hm_checkpoint_graph logical_cp_ids;
    std::vector<hm_checkpoint_id_t> logical_begin_cp_ids;
    std::vector<hm_checkpoint_id_t> logical_end_cp_ids;
};

struct hm_local_status_param_t {
    hm_supervised_entity_id_t se_id;
    uint8_t failed_alive_supervision_cycle_tolerance;
    uint8_t restarted_supervision_cycle_tolerance;
    uint8_t backup_supervision_cycle_tolerance;
};

struct hm_failure_recovery_param_t {
    hm_supervised_entity_id_t se_id;
    hm_failure_recovery_t recovery_action;
};

struct hm_wdg_t {
    std::string wdg_name;
    uint8_t wdg_device_index;
};
