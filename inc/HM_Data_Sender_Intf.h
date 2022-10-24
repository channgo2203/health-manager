/**
 * @file HM_Data_Sender_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the data sender interface
 * @version 0.1
 * @date 2022-03-10
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once


template <typename MsgType>
class HMDataSenderIntf
{
public:
    HMDataSenderIntf() = default;

    virtual ~HMDataSenderIntf() = default;

    virtual bool init() = 0;

    virtual bool publish(const MsgType& data) = 0;
};
