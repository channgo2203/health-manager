#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <vector>


using hm_checkpoint_id_t = uint16_t;
using hm_supervised_entity_id_t = uint16_t;
using hm_mode_t = uint8_t;
using hm_checkpoint_graph = std::map<hm_checkpoint_id_t, std::vector<hm_checkpoint_id_t>>;

enum class hm_local_status_t : uint8_t {
    hm_local_status_ok = 0u,
    hm_local_status_failed = 1u,
    hm_local_status_expired = 2u,
    hm_local_status_restarted = 3u,
    hm_local_status_backup = 4u,
    hm_local_status_recovered = 5u,
    hm_local_status_deactivated = 6u
};

enum class hm_global_status_t : uint8_t {
    hm_global_status_ok = 0u,
    hm_global_status_failed = 1u,
    hm_global_status_expired = 2u,
    hm_global_status_stopped = 3u,
    hm_global_status_deactivated = 4u
};

