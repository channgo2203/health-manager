#pragma once

#include "HM_CP_Data_Intf.h"
#include <mutex>
#include <tuple>

class HMCPData : public HMCPDataIntf {
public:
    HMCPData() = default;

    virtual ~HMCPData() = default;

    bool init(const std::vector<hm_supervised_entity_t>& ses,
              const std::vector<hm_supervision_id_t>& supervision_ids) override;

    bool is_contained(const hm_supervised_entity_id_t se_id,
                      const hm_checkpoint_id_t cp_id) const = 0;

    uint32_t get_alive_indications() const override;

    bool set_alive_indications(const uint32_t indications) override;

    bool increase_alive_indications() override;

    uint32_t get_supervision_cycle_total() const override;

    bool set_supervision_cycle_total(const uint32_t cycles) override;

    bool increase_supervision_cycle_total() override;
private:
    std::map<std::pair<hm_supervised_entity_id_t, hm_checkpoint_id_t>, hm_checkpoint_data_t> checkpoint_data_{};
    std::map<hm_supervision_id_t, bool> logical_supervision_active_flags_{};
    std::map<hm_supervision_id_t, hm_checkpoint_id_t> logical_supervision_pre_visited_cp_{};
    std::mutex lock_{};
};
