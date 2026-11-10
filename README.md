POC of Health Manager for MPU with FastDDS
 
Requirements
============

- Install the library FastDDS by following this [https://fast-dds.docs.eprosima.com/en/latest/installation/binaries/binaries_linux.html]


Features
========

This POC covers all features specified in the Adaptive AUTOSAR except
the following:
- Features that are related to health status with external supervisions

Build
=====

- mkdir build
- cd build
- cmake .. 
- make -j6

Run
===

In the first terminal, from the build directory, run the SE ID 0 with the command

- ./apps/Supervised_Entity_0

In the second terminal, from the build directory, run the SE ID 1 with the command

- ./apps/Supervised_Entity_1

In the third terminal, from the build directory, run the simple example of application
manager with the command

- ./apps/App_Manager

In the 4th terminal, from the build directory, for testing status reporting,
run the command

- ./apps/Status_Receiver

In the 5th terminal, from the build directory, run the HM with the command

- ./HealthManager

Test
===

For some unit tests, run the following command from the build directory

- ./test/HM_Test

