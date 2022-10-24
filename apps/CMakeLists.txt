add_executable(Supervised_Entity_0
  Supervised_Entity_0.cpp)

target_link_libraries(Supervised_Entity_0 HMCore fastrtps fastcdr)

add_executable(Supervised_Entity_1
  Supervised_Entity_1.cpp)

target_link_libraries(Supervised_Entity_1 HMCore fastrtps fastcdr)

add_executable(Status_Receiver
  Status_Receiver.cpp)

target_link_libraries(Status_Receiver HMCore fastrtps fastcdr)

add_executable(App_Manager
  AM_RecoveryCmd_Processor.cpp
  App_Manager.cpp)

target_link_libraries(App_Manager HMCore fastrtps fastcdr)
