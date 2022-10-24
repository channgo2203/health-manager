/**
 * @file HM_HS_Data_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the health status data interface
 * @version 0.1
 * @date 2022-03-18
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

class HMHSDataIntf
{
public:
    HMHSDataIntf() = default;

    virtual ~HMHSDataIntf() = default;

    virtual bool is_contained(const hm_external_supervision_id_t es_id) = 0;

    virtual bool get_failed_hs_supervision_count(const hm_external_supervision_id_t es_id,
                                                 uint32_t& count) = 0;

    virtual bool set_failed_hs_supervision_count(const hm_external_supervision_id_t es_id,
                                                 const uint32_t count) = 0;

    virtual bool increase_failed_hs_supervision_count(const hm_external_supervision_id_t es_id) = 0;

    virtual bool get_local_status(const hm_external_supervision_id_t es_id,
                                  hm_local_status_t& status) = 0;

    virtual bool set_local_status(const hm_external_supervision_id_t es_id,
                                  const hm_local_status_t status) = 0;
};
