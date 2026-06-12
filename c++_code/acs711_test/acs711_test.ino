const int acs771Pin = A0;  // Pin where the ACS771 output is connected
float voltage = 0.0;
float current = 0.0;
const float sensitivity = 0.066;  // Sensitivity of ACS771 (66 mV/A for the 20A version)
const float offsetVoltage = 2.5; // Quiescent voltage (0A) for 5V supply

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read the analog value from ACS771 (0-4096 range)
  int sensorValue = analogRead(acs771Pin);

  // Convert the sensor value (0-4096) to a voltage (0-3.3V)
  voltage = (sensorValue * (3.3 / 4096))-2.30;// -2.30 is a calibrage for voltage 

  // Calculate the current based on the voltage
  // Subtract the offset voltage (2.5V at 0A) and divide by the sensitivity
  current = ((voltage - offsetVoltage) / sensitivity)+38;// +38 IS A CALIBRAGE FOR CURRENT

  // Output the voltage and current values to the serial monitor
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" V, ");
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");

  delay(500);  // Wait for half a second before taking the next reading
}
