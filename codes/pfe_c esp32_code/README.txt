description:
we have two separate ESP32-based equipment units, each with its own dedicated hardware components, as illustrated in the cabling flowchart. 
Each unit acquires temperature, current, and voltage data using an NTC thermistor, an ACS712 current sensor, and an ADS1115 ADC. The collected data is sent to the STM32 via Wi-Fi sockets.
 Additionally, each unit receives commands from the STM32, allowing it to take appropriate actions on the battery.