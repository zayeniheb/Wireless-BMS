wifi.c/wifi.h:
This folder handles Wi-Fi network management. It initializes the Wi-Fi module, connects to Wi-Fi networks, and manages the connection status. 
It offers easy functions to scan, connect, and disconnect from Wi-Fi. It uses the ES Wi-Fi driver to send commands to the module(eswifi.h/eswifi.c).

es_wifi.c/es_wifi.h:
This drivers talks directly to the ESP Wi-Fi module using commands (like AT commands). 
It translates the Wi-Fi module’s needs into low-level commands and reads responses. 
It provides the interface for the Wi-Fi module to perform actions like connecting to an access point or sending data using communication protocols like SPI.

es_wifi_io.h/es_wifi_io.c:
This part manages the actual data transfer between the STM32 and the ESP Wi-Fi module. 
It sends and receives bytes over UART or SPI. The ESP Wi-Fi driver uses this to communicate physically with the hardware
It's the communication betwene the ES module and the MCU

stm32l475e_iot01.h/stm32l475e_iot01.c:
This file controls the basic hardware on the STM32L475E IOT01 board. It manages LEDs, the user button, UART, and I2C pins. 
It provides simple functions to turn LEDs on or off, read the button state, and set up communication ports.

es_wifi_conf.h:
This file sets the configuration options for the ESP Wi-Fi driver and IO interface. 
It defines settings like whether to use SPI or UART for communication, buffer sizes, timeouts, and whether to enable features like ping or WPS. 
It also handles locking mechanisms for multi-threading if the system uses an RTOS

description:
In this code, we receive and send data from ESP32-based equipment, store it in a buffer, and transmit it via UART to an AI model. 
Additionally, we use external interrupts triggered by buttons to select the operating mode.
