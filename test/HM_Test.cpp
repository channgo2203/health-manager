/**
 * @file HM_Test.cpp
 * @author Van C. Ngo
 * @brief This file contains HM tests
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

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <thread>
#include <chrono>

#include "HM_Types.h"
#include "HM_Alive_Supervision_Cfg.h"
#include "HM_Deadline_Supervision_Cfg.h"
#include "HM_CP_Data.h"
#include "HM_HS_Data.h"
#include "HM_Supervised_Entity_Data.h"
#include "HM_Global_Supervision_Data.h"
#include "HM_Cfg_Handler.h"
#include "HM_CP_Processor.h"


/**
 * @brief Shared supervision data
 *
 */
std::shared_ptr<HMCPDataIntf> cp_data{nullptr};
std::shared_ptr<HMHSDataIntf> hs_data{nullptr};
std::shared_ptr<HMSupervisedEntityDataIntf> se_data{nullptr};
std::shared_ptr<HMGlobalSupervisionDataIntf> global_data{nullptr};
std::shared_ptr<HMCPProcessorIntf> cp_processor{nullptr};

/**
 * @brief Test HM_CP_Data
 *
 */
bool test_cp_data()
{
    bool result{true};

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
 * @brief Test HM_Supervised_Entity_Data
 *
 */
bool test_supervised_entity_data()
{
    bool result{true};

    result &= se_data->is_contained(0u);
    result &= se_data->is_contained(1u);
    hm_local_status_t local_status{hm_local_status_t::hm_local_status_deactivated};
    se_data->get_local_status(0u, local_status);
    result &= (local_status == hm_local_status_t::hm_local_status_ok);

    return result;
}

/**
 * @brief Test HM_Global_Supervision_Data
 *
 */
bool test_global_supervision_data()
{
    bool result{true};

    hm_mode_t active_mode_id{0};
    global_data->get_active_mode_id(active_mode_id);
    result &= (active_mode_id == 0u);
    hm_global_status_t global_status{hm_global_status_t::hm_global_status_ok};
    global_data->get_global_status(global_status);
    result &= (global_status == hm_global_status_t::hm_global_status_deactivated);
    uint32_t num_local_failed{0};
    global_data->get_num_local_status_failed(num_local_failed);
    result &= (num_local_failed == 0u);

    return result;
}

/**
 * @brief Test HM_Alive_Supervision_Cfg and HM_Deadline_Supervision_Cfg
 *
 */
bool test_supervision_cfg()
{
    bool result{true};

    auto cp_supervisions_0 = HMCfgHandler::get_cp_supervisions(0u, 0u);
    result &= (cp_supervisions_0.size() == 2);
    result &= cp_supervisions_0[0]->is_contained(0u, 0u);
    result &= cp_supervisions_0[1]->is_contained(0u, 1u);
    result &= HMCfgHandler::is_cp_contained(0u, 0u, 1u);
    hm_supervised_entity_type_t se_0_type{hm_supervised_entity_type_t::hm_primary_supervised_entity};
    result &= HMCfgHandler::get_se_type(0u, se_0_type);
    result &= (se_0_type == hm_supervised_entity_type_t::hm_normal_supervised_entity);

    auto alive_supervisions = (HMCfgHandler::get_mode_cfg_handler(0u))->get_alive_supervisions();
    result &= (alive_supervisions.size() == 2);
    result &= alive_supervisions[0]->is_contained(0u, 0u);
    result &= alive_supervisions[1]->is_contained(0u, 1u);

    auto cp_supervisions_1 = HMCfgHandler::get_cp_supervisions(0u, 1u);
    result &= cp_supervisions_1[0]->is_contained(1u, 0u);
    result &= cp_supervisions_1[0]->is_contained(1u, 1u);
    result &= cp_supervisions_1[1]->is_contained(1u, 1u);
    result &= cp_supervisions_1[1]->is_contained(1u, 2u);
    result &= cp_supervisions_1[2]->is_contained(1u, 1u);
    result &= cp_supervisions_1[2]->is_contained(1u, 3u);
    result &= HMCfgHandler::is_cp_contained(0u, 1u, 2u);

    result &= (cp_supervisions_0[0]->get_cp_supervision_type(0u, 0u) == hm_cp_type_t::hm_alive_supervision_cp);
    result &= (cp_supervisions_1[0]->get_cp_supervision_type(1u, 0u) == hm_cp_type_t::hm_deadline_supervision_start_cp);
    result &= (cp_supervisions_1[0]->get_cp_supervision_type(1u, 1u) == hm_cp_type_t::hm_deadline_supervision_stop_cp);

    cp_data->set_alive_indications(0u, 0u, 4u);
    cp_data->set_supervision_cycle_total(0u, 0u, 1u);
    cp_data->set_supervision_time_total(0u, 0u, 2700u);
    hm_cp_supervision_result_t sup_result{hm_cp_supervision_result_t::hm_cp_supervision_incorrect};
    result &= cp_supervisions_0[0]->compute_supervision_result(cp_data, sup_result, 2000u);
    result &= (sup_result == hm_cp_supervision_result_t::hm_cp_supervision_correct);

    cp_data->set_alive_indications(0u, 1u, 0u);
    cp_data->set_supervision_cycle_total(0u, 1u, 1u);
    cp_data->set_supervision_time_total(0u, 1u, 2100u);
    sup_result = hm_cp_supervision_result_t::hm_cp_supervision_incorrect;
    result &= cp_supervisions_0[1]->compute_supervision_result(cp_data, sup_result, 2000u);
    result &= (sup_result == hm_cp_supervision_result_t::hm_cp_supervision_incorrect);

    uint64_t start_time = 100u;
    cp_data->set_deadline_timestamp(1u, 0u, start_time);
    auto stop_time = start_time + 500u;
    cp_data->set_deadline_timestamp(1u, 1u, stop_time);
    sup_result = hm_cp_supervision_result_t::hm_cp_supervision_incorrect;
    result &= cp_supervisions_1[0]->compute_supervision_result(cp_data, sup_result);
    result &= (sup_result == hm_cp_supervision_result_t::hm_cp_supervision_correct);

    stop_time = start_time + 700u;
    cp_data->set_deadline_timestamp(1u, 1u, stop_time);
    sup_result = hm_cp_supervision_result_t::hm_cp_supervision_incorrect;
    result &= cp_supervisions_1[0]->compute_supervision_result(cp_data, sup_result);
    result &= (sup_result == hm_cp_supervision_result_t::hm_cp_supervision_incorrect);

    return result;
}

int main(int argc, char** argv)
{
    cp_data = std::make_shared<HMCPData>(HMCfgHandler::get_supervised_entities(), HMCfgHandler::get_cp_supervision_ids());
    hs_data = std::make_shared<HMHSData>();
    se_data = std::make_shared<HMSupervisedEntityData>(HMCfgHandler::get_supervised_entities());
    global_data = std::make_shared<HMGlobalSupervisionData>(HMCfgHandler::get_init_mode_id());
    cp_processor = std::make_shared<HMCPProcessor>(cp_data, hs_data, se_data, global_data);

    const bool valid_system = HMCfgHandler::validate() &&
        (cp_data != nullptr) &&
        (se_data != nullptr) &&
        (global_data != nullptr) &&
        (cp_processor != nullptr);

    if (valid_system)
    {
        if (test_cp_data())
        {
            std::cout << "[TEST] HMCPData tests PASSED" << std::endl;
        }
        else
        {
            std::cout << "[TEST] HMCPData tests FAILED" << std::endl;
        }

        if (test_supervised_entity_data())
        {
            std::cout << "[TEST] HMSupervisedEntityData tests PASSED" << std::endl;
        }
        else
        {
            std::cout << "[TEST] HMSupervisedEntityData tests FAILED" << std::endl;
        }

        if (test_global_supervision_data())
        {
            std::cout << "[TEST] HMGlobalSupervisionData tests PASSED" << std::endl;
        }
        else
        {
            std::cout << "[TEST] HMGlobalSupervisionData tests FAILED" << std::endl;
        }

        if (test_supervision_cfg())
        {
            std::cout << "[TEST] Supervision Cfg tests PASSED" << std::endl;
        }
        else
        {
            std::cout << "[TEST] Supervision Cfg tests FAILED" << std::endl;
        }
    }

    return 0;
}
