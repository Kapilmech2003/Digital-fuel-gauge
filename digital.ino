#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HX711.h"

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

HX711 scale;

// Calibration values (adjust these based on your tank and sensor)
long EMPTY_PRESSURE = 830000; // Pressure reading when the tank is empty
long FULL_PRESSURE = 2500000; // Pressure reading when the tank is full
const float TANK_CAPACITY = 5; // Tank capacity in liters (750 ml)

unsigned long previousMillis = 0;
const long interval = 500; // Update interval in milliseconds (5 seconds)

void setup() {
  Serial.begin(9600);
  
  // Initialize display with the specified I2C address
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Change 0x3C to your OLED address (if different)
  display.display();
  delay(2000);
  display.clearDisplay();
  
  // Define your scale data and clock pins
  scale.begin(3, 4); // Set the data pin to D3 and the SCK pin to D4 (adjust as needed)

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // Read pressure value from the sensor
    long pressureReading = scale.read();
    
    // Print the raw sensor value to the serial monitor
    Serial.print("Raw Pressure Reading: ");
    Serial.println(pressureReading);
    
    // Calculate the ratio of the current pressure to the full pressure
    float pressureRatio = (float)(pressureReading - EMPTY_PRESSURE) / (FULL_PRESSURE - EMPTY_PRESSURE);
    
    // Ensure the ratio is within the valid range [0, 1]
    pressureRatio = constrain(pressureRatio, 0.0, 1.0);
    
    // Calculate the fuel level in liters
    float fuelLevel = pressureRatio * TANK_CAPACITY;
    
    // Print the calculated fuel level to the serial monitor
    Serial.print("Fuel Level (Liters): ");
    Serial.println(fuelLevel, 3); // Display three decimal places for a precise reading
    
    // Update the OLED display
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Fuel Level:");
    display.setCursor(0,16);
    display.print(fuelLevel, 3); // Display three decimal places
    display.print(" Liters");
    display.display();
    
    previousMillis = currentMillis;
  }
}
