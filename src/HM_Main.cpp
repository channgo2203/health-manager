#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "HM_Types.h"
#include "HM_Alive_Supervision_Cfg.h"
#include "HM_Deadline_Supervision_Cfg.h"
#include "HM_CP_Data.h"


std::vector<hm_supervised_entity_t> supervised_entities{};
std::vector<hm_supervision_id_t> supervision_ids{0u, 1u, 2u, 3u, 4u};
std::map<hm_supervised_entity_id_t, std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>> cp_supervisions{};
std::shared_ptr<HMCPDataIntf> cp_data{nullptr};

/**
 * @brief Create supervised entities
 * 
 */
void create_supervised_entities()
{
    hm_supervised_entity_t se_0{0u, hm_supervised_entity_type_t::hm_normal_supervised_entity, 0u, 0u, {0u, 1u}};
    supervised_entities.push_back(se_0);
    se_0.se_id = 1u;
    se_0.checkpoints.push_back(2u);
    se_0.checkpoints.push_back(3u);
    supervised_entities.push_back(se_0);
}

/**
 * @brief Create test cp supervisions
 * 
 */
void create_cp_supervisions()
{
    hm_alive_supervision_param_t alive_sup_params{10u, 3u, 3u, 1u, 0u, 0u, 0u};
    std::shared_ptr<HMCPSupervisionCfgIntf> p_as_0 = std::make_shared<HMAliveSupervisionCfg>(alive_sup_params);
    alive_sup_params.expected_alive_indications = 5u;
    alive_sup_params.min_margin = 1u;
    alive_sup_params.max_margin = 1u;
    alive_sup_params.cp_id = 1u;
    alive_sup_params.supervision_id = 1u;
    std::shared_ptr<HMCPSupervisionCfgIntf> p_as_1 = std::make_shared<HMAliveSupervisionCfg>(alive_sup_params);
    cp_supervisions[0u] = {p_as_0, p_as_1};
    hm_deadline_supervision_param_t deadline_sup_params{50u, 120u, 1u, 0u, 1u, 2u};
    std::shared_ptr<HMCPSupervisionCfgIntf> p_ds_2 = std::make_shared<HMDeadlineSupervisionCfg>(deadline_sup_params);
    deadline_sup_params.cp_start_id = 1u;
    deadline_sup_params.cp_stop_id = 2u;
    deadline_sup_params.supervision_id = 3u;
    std::shared_ptr<HMCPSupervisionCfgIntf> p_ds_3 = std::make_shared<HMDeadlineSupervisionCfg>(deadline_sup_params);
    deadline_sup_params.deadline_min = 100u;
    deadline_sup_params.deadline_max = 220u;
    deadline_sup_params.cp_start_id = 1u;
    deadline_sup_params.cp_stop_id = 3u;
    deadline_sup_params.supervision_id = 4u;
    std::shared_ptr<HMCPSupervisionCfgIntf> p_ds_4 = std::make_shared<HMDeadlineSupervisionCfg>(deadline_sup_params);
    cp_supervisions[1u] = {p_ds_2, p_ds_3, p_ds_4};
}

/**
 * @brief Test HM_CP_Data
 * 
 */
bool test_cp_data()
{
    bool result{true};
    create_supervised_entities();
    cp_data = std::make_shared<HMCPData>();
    cp_data->init(supervised_entities, supervision_ids);

    result &= cp_data->is_contained(0u, 0u);
    result &= cp_data->is_contained(0u, 1u);
    result &= !(cp_data->is_contained(0u, 2u));
    result &= cp_data->is_contained(1u, 0u);
    result &= cp_data->is_contained(1u, 1u);
    result &= cp_data->is_contained(1u, 2u);
    result &= cp_data->is_contained(1u, 3u);
    result &= !(cp_data->is_contained(1u, 4u));

    return result;
}

/**
 * @brief Test HM_Alive_Supervision_Cfg and HM_Deadline_Supervision_Cfg
 * 
 */
bool test_supervision_cfg()
{
    bool result{true};
    create_cp_supervisions();
    result &= (cp_supervisions.size() == 2);
    result &= cp_supervisions[0][0]->is_contained(0u, 0u);
    result &= cp_supervisions[0][1]->is_contained(0u, 1u);
    result &= cp_supervisions[1][0]->is_contained(1u, 0u);
    result &= cp_supervisions[1][0]->is_contained(1u, 1u);
    result &= cp_supervisions[1][1]->is_contained(1u, 1u);
    result &= cp_supervisions[1][1]->is_contained(1u, 2u);
    result &= cp_supervisions[1][2]->is_contained(1u, 1u);
    result &= cp_supervisions[1][2]->is_contained(1u, 3u);
    result &= (cp_supervisions[0][0]->get_cp_supervision_type(0u, 0u) == hm_cp_type_t::hm_alive_supervision_cp);
    result &= (cp_supervisions[1][0]->get_cp_supervision_type(1u, 0u) == hm_cp_type_t::hm_deadline_supervision_start_cp);
    result &= (cp_supervisions[1][0]->get_cp_supervision_type(1u, 1u) == hm_cp_type_t::hm_deadline_supervision_stop_cp);
    
    cp_data->set_alive_indications(0u, 0u, 12u);
    cp_data->set_supervision_cycle_total(0u, 0u, 1u);
    hm_cp_supervision_result_t sup_result{hm_cp_supervision_result_t::hm_cp_supervision_incorrect};
    result &= cp_supervisions[0][0]->compute_supervision_result(cp_data, sup_result);
    result &= (sup_result == hm_cp_supervision_result_t::hm_cp_supervision_correct);

    cp_data->set_alive_indications(0u, 1u, 3u);
    cp_data->set_supervision_cycle_total(0u, 1u, 1u);
    sup_result = hm_cp_supervision_result_t::hm_cp_supervision_incorrect;
    result &= cp_supervisions[0][1]->compute_supervision_result(cp_data, sup_result);
    result &= (sup_result == hm_cp_supervision_result_t::hm_cp_supervision_incorrect);

    const auto start_time = std::chrono::system_clock::now();
    cp_data->set_deadline_timestamp(1u, 0u, start_time);
    auto stop_time = start_time + std::chrono::milliseconds(100);
    cp_data->set_deadline_timestamp(1u, 1u, stop_time);
    sup_result = hm_cp_supervision_result_t::hm_cp_supervision_incorrect;
    result &= cp_supervisions[1][0]->compute_supervision_result(cp_data, sup_result);
    result &= (sup_result == hm_cp_supervision_result_t::hm_cp_supervision_correct);

    stop_time = start_time + std::chrono::milliseconds(130);
    cp_data->set_deadline_timestamp(1u, 1u, stop_time);
    sup_result = hm_cp_supervision_result_t::hm_cp_supervision_incorrect;
    result &= cp_supervisions[1][0]->compute_supervision_result(cp_data, sup_result);
    result &= (sup_result == hm_cp_supervision_result_t::hm_cp_supervision_incorrect);

    return result;
}

int main(int argc, char** argv)
{
    if (test_cp_data())
    {
        std::cout << "HMCPData tests PASSED" << std::endl;
    }
    else
    {
        std::cout << "HMCPData tests FAILED" << std::endl;
    }

    if (test_supervision_cfg())
    {
        std::cout << "Supervision Cfg tests PASSED" << std::endl;
    }
    else
    {
        std::cout << "Supervision Cfg tests FAILED" << std::endl;
    }

    return 0;
}
