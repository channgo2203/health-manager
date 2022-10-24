/**
 * @file HM_Mode_Cfg_Handler.cpp
 * @author Van C. Ngo
 * @brief This file contains the mode configuration
 * @version 0.1
 * @date 2022-03-16
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include "HM_Mode_Cfg_Handler.h"

#include <algorithm>
#include <iterator>
#include <numeric>


const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>> HMModeCfgHandler::empty_cp_supervision_ = {};

HMModeCfgHandler::HMModeCfgHandler(const uint32_t expired_supervision_cycle_tolerance,
                                   const hm_mode_t mode_id,
                                   const uint32_t supervision_cycle_time,
                                   const std::vector<hm_local_status_param_t>& local_supervision_params,
                                   const std::vector<hm_hs_supervision_param_t>& hs_supervision_params,
                                   const std::vector<hm_cp_failure_recovery_param_t>& cp_recovery_params,
                                   const std::vector<hm_hs_invalid_reaction_param_t>& hs_reaction_params,
                                   const std::map<hm_supervised_entity_id_t,
                                   std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>>& cp_supervisions,
                                   const std::map<hm_external_supervision_id_t,
                                   std::shared_ptr<const HMHSSupervisionCfgIntf>>& hs_supervisions)
: expired_supervision_cycle_tolerance_(expired_supervision_cycle_tolerance),
  hm_mode_id_(mode_id),
  supervision_cycle_time_(supervision_cycle_time),
  local_supervision_params_(local_supervision_params),
  hs_supervision_params_(hs_supervision_params),
  cp_recovery_params_(cp_recovery_params),
  hs_reaction_params_(hs_reaction_params),
  cp_supervisions_(cp_supervisions),
  hs_supervisions_(hs_supervisions)
{
    for (const auto& cp_sup : cp_supervisions_)
    {
        std::copy_if(cp_sup.second.begin(), cp_sup.second.end(),
                     std::back_inserter(alive_supervisions_),
                     [](std::shared_ptr<const HMCPSupervisionCfgIntf> sup) {
                         return (sup->get_supervision_type() ==
                                 hm_supervision_type_t::hm_alive_supervision); });

        std::copy_if(cp_sup.second.begin(), cp_sup.second.end(),
                     std::back_inserter(deadline_supervisions_),
                     [](std::shared_ptr<const HMCPSupervisionCfgIntf> sup) {
                         return (sup->get_supervision_type() ==
                                 hm_supervision_type_t::hm_deadline_supervision); });

        std::copy_if(cp_sup.second.begin(), cp_sup.second.end(),
                     std::back_inserter(logical_supervisions_),
                     [](std::shared_ptr<const HMCPSupervisionCfgIntf> sup) {
                         return (sup->get_supervision_type() ==
                                 hm_supervision_type_t::hm_logical_supervision); });
    }

    for (const auto& hs_sup : hs_supervisions_)
    {
        health_status_supervisions_.push_back(hs_sup.second);
    }
}

bool HMModeCfgHandler::validate() const
{
    bool result{true};
    result &= (local_supervision_params_.size() == cp_recovery_params_.size());
    result &= (local_supervision_params_.size() == cp_supervisions_.size());
    result &= (hs_reaction_params_.size() == hs_supervisions_.size());

    return result;
}

bool HMModeCfgHandler::is_se_contained(const hm_supervised_entity_id_t se_id) const
{
    return (cp_supervisions_.find(se_id) != cp_supervisions_.end());
}

bool HMModeCfgHandler::is_cp_contained(const hm_supervised_entity_id_t se_id,
                                       const hm_checkpoint_id_t cp_id) const
{
    bool result{false};
    if (cp_supervisions_.find(se_id) != cp_supervisions_.end())
    {
        for (const auto& sup : cp_supervisions_.at(se_id))
        {
            if (sup->is_contained(se_id, cp_id))
            {
                result = true;
                break;
            }
        }
    }
    return result;
}

bool HMModeCfgHandler::is_hs_contained(const hm_external_supervision_id_t es_id) const
{
    bool result{false};
    if (hs_supervisions_.find(es_id) != hs_supervisions_.end())
    {
        result = true;
    }
    return result;
}

const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
    HMModeCfgHandler::get_cp_supervisions(const hm_supervised_entity_id_t se_id) const
{
    if (cp_supervisions_.find(se_id) != cp_supervisions_.end())
    {
        return cp_supervisions_.at(se_id);
    }

    return empty_cp_supervision_;
}

std::shared_ptr<const HMHSSupervisionCfgIntf>
    HMModeCfgHandler::get_hs_supervision(const hm_external_supervision_id_t es_id) const
{
    if (hs_supervisions_.find(es_id) != hs_supervisions_.end())
    {
        return hs_supervisions_.at(es_id);
    }

    return nullptr;
}

uint32_t HMModeCfgHandler::get_expired_supervision_cycle_tolerance() const
{
    return expired_supervision_cycle_tolerance_;
}

hm_mode_t HMModeCfgHandler::get_mode_id() const
{
    return hm_mode_id_;
}

uint32_t HMModeCfgHandler::get_supervision_cycle_time() const
{
    return supervision_cycle_time_;
}

const std::vector<hm_local_status_param_t>&
    HMModeCfgHandler::get_local_supervision_params() const
{
    return local_supervision_params_;
}

const std::vector<hm_hs_supervision_param_t>&
    HMModeCfgHandler::get_hs_supervision_params() const
{
    return hs_supervision_params_;
}

const std::vector<hm_cp_failure_recovery_param_t>&
    HMModeCfgHandler::get_cp_recovery_params() const
{
    return cp_recovery_params_;
}

const std::vector<hm_hs_invalid_reaction_param_t>&
    HMModeCfgHandler::get_hs_reaction_params() const
{
    return hs_reaction_params_;
}

const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
    HMModeCfgHandler::get_alive_supervisions() const
{
    return alive_supervisions_;
}

const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
    HMModeCfgHandler::get_deadline_supervisions() const
{
    return deadline_supervisions_;
}

const std::vector<std::shared_ptr<const HMCPSupervisionCfgIntf>>&
    HMModeCfgHandler::get_logical_supervisions() const
{
    return logical_supervisions_;
}

const std::vector<std::shared_ptr<const HMHSSupervisionCfgIntf>>&
    HMModeCfgHandler::get_hs_supervisions() const
{
    return health_status_supervisions_;
}
