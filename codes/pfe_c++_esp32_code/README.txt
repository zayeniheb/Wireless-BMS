description:
we started with the first version of the ESP32-based equipment developed in C++, where each functionality—such as temperature measurement, current sensing, voltage reading, data transmission, and command reception—was implemented and tested individually.
 Each sensor was handled separately: the NTC thermistor for temperature, the ACS712 for current, and the ADS1115 ADC for accurate analog signal conversion.
This modular and functional testing approach served as the foundation for developing the current version using the C language and the Espressif ESP-IDF framework.