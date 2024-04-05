#define potPin 6            // Define pin on MCU
float potVoltage,adcVal;    // Define variables

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // Bit resultion for readings
}

void loop() {
  adcVal = analogRead(potPin);  // Read ADC values on the pin. Ranges between 0 - 40955
  potVoltage = adcVal / 4095 * 3.1; // Convert the ADC values to a voltage. Max voltage reading for MCU is 3.1V
  Serial.print("Voltage reading:"); // Print results.
  Serial.print("\t");
  Serial.println(potVoltage);
  delay(20);
}
