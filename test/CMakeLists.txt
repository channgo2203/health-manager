add_executable(HM_Test
  ../src/HM_CP_Data.cpp
  ../src/HM_HS_Data.cpp
  ../src/HM_Supervised_Entity_Data.cpp
  ../src/HM_Global_Supervision_Data.cpp
  ../src/HM_Alive_Supervision_Cfg.cpp
  ../src/HM_Deadline_Supervision_Cfg.cpp
  ../src/HM_Mode_Cfg_Handler.cpp
  ../src/HM_Cfg_Handler.cpp
  ../src/HM_CP_Processor.cpp
  ${DDS_HEALTHMANAGER_SOURCES_CXX}
  ${DDS_HEALTHMANAGER_HEADER_H}
  HM_Test.cpp)

target_link_libraries(HM_Test fastrtps fastcdr)
