# FIRE-PowerEfficiency
Main FIRE repository for the Arduino firmware implementing the power efficiency module of LibreRouter. This Arduino firmware will be used to prototype the development of a daughter board that will be plugged onto a LibreRouter board. This daughter board is supposed to 

1) Configure an RTC to switch on/off the router on an RTC alarm (using a GPIO signal to control the router power).

2) Communicate to an Android device where an Android App will use high-level commands to order that on/off switch

3) Communicate to another similar board (through LoRa or 802.15-4 interface)

This repository will contain the following submodules:
- Wireless
- RTC
- UART
- Commands interpreter

## Wireless
The wireless module will be used for the following links:
- Mobile device to router communication
- Router to router communication

Through each link, the daugther board must broadcast high-level commands (to coordinate network on/off messages)

For the wireless interface the options are:
- BLE
- LoRa
- 802.15-4/Zigbee

### BLE
Constraints:
- Bluetooth Low Energy is compatible with Android 4.3+
Pros:
- Low energy
- Very extended technology (most mobile phones interact with it)
Recommended usage:
- Mobile device to router communication

### LoRa
Constraints:
- Privative technology not available on mobile phones
Pros:
- High range with low power transmission (very low sensitivity)
Recommended usage:
- Router to router communication

### 802.15-4
Constraints:
- Usually not available on mobile phones
Pros:
- High range radios available (low sensitivity)

### Repo
From this repository it's configured as a git submodule so if you want to get wireless module please just add --recursive to clone command when cloning this repo. To get this module alone, please go to:

"Link not yet ready"

## RTC
RTC module. The daugther board is supposed to configure alarms to switch on/off the LibreRouter. These configuration will be received either through the UART either through the wireless link in the form of high-level commands, that the commands interpreter will translate. This module need to be able to offer a public an easy API allowing to 
- Possible inputs:
   - Time to reset the clock to (time of day)
   - Single alarm configuration and associated action (ON/OFF)
   - Periodic alarm configuration and associated actions (ON/OFF)
- Outputs
   - ON/OFF logic signal

### Repo
From this repository it's configured as a git submodule so if you want to get wireless module please just add --recursive to clone command when cloning this repo. To get this module alone, please go to:

"Link not yet ready"

## UART
UART communication module. This module will be in charge of communicating to the 4 possible sources of UART communication that the daughter board can receive:
- ATMega to AR9344: This is the communication between the router microcontroller and the daugther board microcontroller
- ATMega to Wireless module 1: The wireless modules talk using the UART with the ATMega. Module 1 will be the one in charge for the mobile device to router communication
- ATMega to Wireless module 2: Module 2 will be the one in charge for the router to router communication
- ATMega to external board communication: Other boards (like Elektra's MPPT board) use this link to transmit high-level commands

### Repo
From this repository it's configured as a git submodule so if you want to get wireless module please just add --recursive to clone command when cloning this repo. To get this module alone, please go to:

"Link not yet ready"

## Commands interpreter
Manage high-level commands accepted for the daughter board 

### Repo
From this repository it's configured as a git submodule so if you want to get wireless module please just add --recursive to clone command when cloning this repo. To get this module alone, please go to:

"Link not yet ready"
