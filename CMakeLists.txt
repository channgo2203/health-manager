cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(HealthManager)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

# Set C++14
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++14 SUPPORTS_CXX14)
    if(SUPPORTS_CXX14)
        add_compile_options(-std=c++14)
        set(CMAKE_CXX_STANDARD 14)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++14")
    endif()
endif()

# Include headers
include_directories(inc)
include_directories(topic)

message(STATUS "Configuring HealthManager Component...")
file(GLOB DDS_HEALTHMANAGER_SOURCES_CXX "topic/*.cxx")
file(GLOB DDS_HEALTHMANAGER_HEADER_H "topic/*.h")

add_library(HMCore SHARED
  src/HM_Data_FastDDS_Sender.cpp
  src/HM_Data_FastDDS_Receiver.cpp
  src/HM_CP_Data.cpp
  src/HM_HS_Data.cpp
  src/HM_Supervised_Entity_Data.cpp
  src/HM_Global_Supervision_Data.cpp
  src/HM_Alive_Supervision_Cfg.cpp
  src/HM_Deadline_Supervision_Cfg.cpp
  src/HM_Mode_Cfg_Handler.cpp
  src/HM_Cfg_Handler.cpp
  src/HM_CP_Processor.cpp
  src/HM_SupervisionStatus_Processor.cpp
  src/HM_Local_Status_Processor.cpp
  src/HM_Global_Status_Processor.cpp
  src/HM_Failure_Handler.cpp
  src/HM_Status_Reporter.cpp
  ${DDS_HEALTHMANAGER_SOURCES_CXX}
  ${DDS_HEALTHMANAGER_HEADER_H})

target_link_libraries(HMCore fastrtps fastcdr)

add_executable(HealthManager
  src/HM_Component_Factory.cpp
  src/Health_Manager.cpp
  src/HM_Main.cpp)

target_link_libraries(HealthManager HMCore fastrtps fastcdr)

add_subdirectory(apps)

add_subdirectory(test)
