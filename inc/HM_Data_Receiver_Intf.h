/**
 * @file HM_Data_Receiver_Intf.h
 * @author Van C. Ngo
 * @brief This file contains the data receiver interface
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once


template<typename MsgType, typename MsgProcType>
class HMDataReceiverIntf
{
public:
    HMDataReceiverIntf() = default;

    virtual ~HMDataReceiverIntf() = default;

    virtual bool init(std::shared_ptr<MsgProcType> data_processor) = 0;
};
