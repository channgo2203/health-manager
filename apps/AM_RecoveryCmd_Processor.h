/**
 * @file HM_RecoveryCmd_Processor.h
 * @author Van C. Ngo
 * @brief This file contains the CP processor
 * @version 0.1
 * @date 2022-05-24
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#pragma once

#include <string>
#include <atomic>
#include <cstdint>
#include <map>

#include "HM_RecoveryCmd_Processor_Intf.h"


class AMRecoveryCmdProcessor : public HMRecoveryCmdProcessorIntf
{
public:
    AMRecoveryCmdProcessor();

    virtual ~AMRecoveryCmdProcessor() = default;

    bool process_data(const HMTPRecoveryCmd& recovery_cmd) override;

private:
    bool restart_se(const HMTPRecoveryCmd& cmd);
    bool backup_se(const HMTPRecoveryCmd& cmd);
    pid_t launch_app(const std::string& exe_path, const std::string& log_file_path);
    void kill_app(const pid_t pid);

    std::string working_dir_{""};
    std::map<std::string, pid_t> se_pids_{};
};
