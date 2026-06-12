#include <Arduino.h>

#define NTC_PIN 34 
#define SERIES_RESISTOR 10000  
#define NOMINAL_RESISTANCE 10000 
#define NOMINAL_TEMPERATURE 25.0 
#define BETA_COEFFICIENT 2600 
#define ADC_MAX 4095 
#define VREF 3.3  

void setup() {
    Serial.begin(115200);
}

void loop() {
    int adc_value = analogRead(NTC_PIN);  
    float voltage = adc_value * (VREF / ADC_MAX);

    // One-line equation for temperature calculation
    float temperature = (1.0 / ((log(SERIES_RESISTOR * (VREF / voltage - 1) / NOMINAL_RESISTANCE) / BETA_COEFFICIENT) + (1.0 / (NOMINAL_TEMPERATURE + 273.15)))) - 273.15;

    Serial.print("Température: ");
    Serial.print(temperature);
    Serial.println(" °C");

    delay(1000);  
}
