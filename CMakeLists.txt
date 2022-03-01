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

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(SUPPORTS_CXX11)
        add_compile_options(-std=c++11)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

# Include headers
include_directories(inc)
include_directories(topic)

message(STATUS "Configuring HealthManager Component...")
file(GLOB DDS_HEALTHMANAGER_SOURCES_CXX "topic/*.cxx")

add_executable(HealthManager
               src/HM_CP_Data.cpp
               src/HM_Alive_Supervision_Cfg.cpp
               src/HM_Deadline_Supervision_Cfg.cpp
               src/HM_Main.cpp
               ${DDS_HEALTHMANAGER_SOURCES_CXX})

target_link_libraries(HealthManager fastrtps fastcdr)
