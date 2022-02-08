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

enum class hm_health_status_reaction_t : uint8_t {
    hm_health_status_nothing = 0u,
    hm_health_status_reset = 1u
};

struct hm_supervised_entity_t {
    hm_supervised_entity_id_t se_id;
    std::vector<hm_checkpoint_id_t> checkpoints;
};

struct hm_alive_supervision_t {
    uint16_t expected_alive_indicators;
    uint32_t min_margin;
    uint32_t max_margin;
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
struct hm_health_status_reaction_param_t {
    hm_external_supervision_id_t es_id;
    hm_health_status_reaction_t health_status_action;
};

struct hm_wdg_t {
    std::string wdg_name;
    uint8_t wdg_device_index;
};

struct hm_mode_config_t {
    uint16_t expired_supervision_cycle_tolerance;
    uint8_t mode_id;
    uint16_t supervision_cycle;
    std::map<hm_supervised_entity_t, std::vector<hm_alive_supervision_t>> se_alive_supervisions;
    std::map<hm_supervised_entity_t, std::vector<hm_deadline_supervision_t>> se_deadline_supervisions;
    std::map<hm_supervised_entity_t, std::vector<hm_logical_supervision_t>> se_logical_supervisions;
    std::vector<hm_local_status_param_t> local_status_params;
};
