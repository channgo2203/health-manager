/**
 * @file AM_RecoveryCmd_Processor.cpp
 * @author Van C. Ngo
 * @brief This file contains the recovery cmd processor
 * @version 0.1
 * @date 2022-06-09
 *
 * @copyright Copyright (c) NIO USA 2022
 * All rights reserved.
 * This document contains proprietary information belonging to NIO USA. Passing on and copying of
 * this document, and communication of its contents is not permitted without prior written
 * authorization.
 *
 */

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <signal.h>

#include "AM_RecoveryCmd_Processor.h"


#ifndef MAX_BUF
#define MAX_BUF 256
#endif

AMRecoveryCmdProcessor::AMRecoveryCmdProcessor()
{
    // Get the current working directory
    std::string working_dir{""};
    char current_wd[MAX_BUF];
    if (getcwd(current_wd, sizeof(current_wd)) != nullptr)
    {
        working_dir_ = std::string(current_wd);
    }
}

bool AMRecoveryCmdProcessor::process_data(const HMTPRecoveryCmd& recovery_cmd)
{
    bool result{false};
    const auto recovery_type = recovery_cmd.recovery_type();
    switch (recovery_type)
    {
    case 0:
        std::cout << "[AMRecoverycmdprocessor] Received restarting command" << std::endl;
        result = restart_se(recovery_cmd);
        break;
    case 1:
        std::cout << "[AMRecoverycmdprocessor] Received backup command" << std::endl;
        result = backup_se(recovery_cmd);
        break;
    default:
        std::cout << "[AMRecoveryCmdProcessor] Received unsupported recovery command" << std::endl;
        break;
    }
    return result;
}

bool AMRecoveryCmdProcessor::restart_se(const HMTPRecoveryCmd& cmd)
{
    bool result{false};
    const auto& se_name = cmd.se_name();
    if (!working_dir_.empty() && !se_name.empty())
    {
        if (se_pids_.find(se_name) != se_pids_.end())
        {
            kill_app(se_pids_.at(se_name));
        }
        const auto app_exe_path = working_dir_ + "/apps/" + se_name;
        const auto log_file_path = app_exe_path + "_log.txt";
        auto se_pid = launch_app(app_exe_path, log_file_path);
        if (se_pid > 0)
        {
            std::cout << "[AMRecoveryCmdProcessor] Successfully launched " << se_name << std::endl;
            se_pids_[se_name] = se_pid;
            result = true;
        }
    }
    return result;
}

bool AMRecoveryCmdProcessor::backup_se(const HMTPRecoveryCmd& cmd)
{
    bool result{false};
    const auto& se_name = cmd.se_name();
    std::cout << "[AMRecoveryCmdProcessor] Backup is not implemented yet for " << se_name << std::endl;
    return result;
}

pid_t AMRecoveryCmdProcessor::launch_app(const std::string& exe_path, const std::string& log_file_path)
{
    pid_t app_pid{-1};
    auto log_fd = open(log_file_path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (log_fd >= 0)
    {
        app_pid = fork();
        if (app_pid == 0)
        {
            char* app_arg[] = {nullptr};
            char* app_env[] = {nullptr};
            dup2(log_fd, STDOUT_FILENO);
            dup2(log_fd, STDERR_FILENO);
            ::close(log_fd);
            if (execve(exe_path.c_str(), app_arg, app_env) == -1)
            {
                // 0xEE - 238 will indicate exec failure to parent
                exit(0xEE);
            }
        }
        else
        {
            ::close(log_fd);
        }
    }
    return app_pid;
}

void AMRecoveryCmdProcessor::kill_app(const pid_t pid)
{
    if (pid > 0)
    {
        kill(pid, SIGKILL);
    }
}
