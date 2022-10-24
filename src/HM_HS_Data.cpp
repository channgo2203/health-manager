/**
 * @file HM_HS_Data.cpp
 * @author Van C. Ngo
 * @brief This file contains the health status data
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

#include "HM_HS_Data.h"


bool HMHSData::is_contained(const hm_external_supervision_id_t es_id)
{
    return false;
}

bool HMHSData::get_failed_hs_supervision_count(const hm_external_supervision_id_t es_id,
                                               uint32_t& count)
{
    return false;
}

bool HMHSData::set_failed_hs_supervision_count(const hm_external_supervision_id_t es_id,
                                               const uint32_t count)
{
    return false;
}

bool HMHSData::increase_failed_hs_supervision_count(const hm_external_supervision_id_t es_id)
{
    return false;
}

bool HMHSData::get_local_status(const hm_external_supervision_id_t es_id,
                                hm_local_status_t& status)
{
    return false;
}

bool HMHSData::set_local_status(const hm_external_supervision_id_t es_id,
                                const hm_local_status_t status)
{
    return false;
}
