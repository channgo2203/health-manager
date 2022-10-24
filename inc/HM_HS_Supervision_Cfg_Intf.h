/**
 * @file HM_HS_Supervision_Cfg_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the health status supervision interface
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

#pragma once

#include "HM_Types.h"


class HMHSSupervisionCfgIntf
{
public:
    HMHSSupervisionCfgIntf() = default;

    virtual ~HMHSSupervisionCfgIntf() = default;

    virtual hm_supervision_id_t get_supervision_id() const = 0;

    virtual bool is_contained(const hm_external_supervision_id_t es_id) const = 0;

    virtual hm_supervision_type_t get_supervision_type() const = 0;

    virtual bool compute_supervision_result(const hm_hs_type_t hs_status,
                                            hm_hs_supervision_result_t& result) const = 0;
};
