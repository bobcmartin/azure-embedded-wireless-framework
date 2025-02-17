# Azure Small Devices Networking framework examples for Renesas RA
The purpose of these examples is to show the use of EWF with the Renesas RA family of microcontrollers.
Embedded Wireless framework(EWF) is meant to simplify connecting small devices to the internet using different connectivity adapters.
An adapter is the device or software used to connect the application using the EWF API to the internet.
An adapter can be a cellular modem, a WiFi adapter, an Ethernet adapter, a WinSock software layer, a BSD software layer or other type of device or software.
The EWF API is described in the Doxygen documentation generated out of the common code.
The source files of the EWF are located in the \src directory at the root of this repository.
The current set of examples use the EK-RA6M4 evaluation board and a ryz014 modem.

# Getting started - RYZ014
To get started with the examples, please follow these steps:
1. Acquire the necessary hardware and software: the Evaluation Kit EK-RA6M4, a Renesas RYZ014A PMOD, e2-studio with FSP 3.5 or newer, openssl, J-Link RTT viewer, Azure IoT Explorer(recommend to use v0.13.5 as latest version 0.14.x cannot show data correctly).
2. Connect to RYZ014 PMOD to EK-RA6M4 board on PMOD2 connector. Connect Micro USB cable to USB Debug pin (DEBUG 1) and another USB cable to RYZ014 micro USB pin.
3. Review and edit as necessary the files ewf.config.h and ewf_example.config.h in the different examples. Edit these files to match your configuration. Specially edit the variables EWF_CONFIG_SIM_PIN, EWF_CONFIG_IOT_HUB_HOSTNAME, EWF_CONFIG_IOT_HUB_DEVICEID.
4. Register your device with Azure IoT Hub and generate the necessary certificates and keys and corresponding header files. The detailed instructions and scripts are located in this directory \tools\cert-scripts, copy the resulting .h files into the include directory of the "certs" example
5. Start e2 studio and create a workspace in \examples\EK-RA6M4, import all the ryz014 examples into the workspace and build all examples.
6. Open J-Link RTT Viewer for viewing debug information.
7. Run the ewf_info_ryz014 example to verify basic communication with the modem, check the output on the terminal for progress and possible errors.
8. Run the ewf_test_ryz014 example to verify basic functionality, check the output on the terminal for progress and possible errors, not all test may pass, depending on your current configuration.
9. Run the ewf_certs_ryz014 example to provision the certificates into the modem. Verify the output on the terminal, there should be no errors.
10. Run the ewf_telemetry_ryz014 example to see telemetry information being sent to IoT Hub. You can view the telemetry data being posted using the Azure IoT Explorer. Once the telemetry messages are sent, the application will wait one minute listening for cloud to device messages, you can use the Azure IoT Explorer to send messages to the device. You can observe the progress of the application and C2D incoming messages in the terminal.
Note: C2D message processing is not implemented in the adapter code for RYZ014 due to firmware limitation on message size. This will be updated in near furture.
