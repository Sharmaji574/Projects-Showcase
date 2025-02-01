#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Pin configuration
const int batteryPin = A0;    // Pin to read battery voltage
const int redLED = 3;         // Red LED pin (D3)
const int yellowLED = 4;      // Yellow LED pin (D4)
const int greenLED = 5;       // Green LED pin (D5)

// Battery voltage thresholds
const float lowThreshold = 4.95;  // Voltage below which Red LED will blink (low battery)
const float medThreshold = 4.99;  // Voltage between 4.95V - 4.99V to blink Yellow LED
const float highThreshold = 5.00; // Above 5.00V to blink Green LED

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Set LED pins as OUTPUT
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);  // Wait for 2 seconds to show the logo
  
  // Clear the display
  display.clearDisplay();
}

void loop() {
  // Read battery voltage from analog pin
  int rawValue = analogRead(batteryPin);
  
  // Convert raw analog value (0-1023) to battery voltage (0-5V)
  float batteryVoltage = (rawValue / 1023.0) * 5.0; // Correct voltage scaling
  
  // Print battery voltage to serial monitor for debugging
  Serial.print("Battery Voltage: ");
  Serial.println(batteryVoltage);
  
  // Display energy transfer status
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Energy Transfer: ");
  
  // Assuming solar energy is being transferred
  display.setCursor(0, 10);
  display.print("Energy Transfer Active");
  
  // LED Control Logic
  if (batteryVoltage < lowThreshold) {
    blinkLED(redLED);   // Blink red if battery is low
    display.setCursor(0, 20);
    display.print("Current: Low");
  } else if (batteryVoltage >= lowThreshold && batteryVoltage < medThreshold) {
    blinkLED(yellowLED); // Blink yellow for medium battery
    display.setCursor(0, 20);
    display.print("Current: Medium");
  } else if (batteryVoltage >= medThreshold) {
    blinkLED(greenLED);  // Blink green for high battery
    display.setCursor(0, 20);
    display.print("Current: High");
  }

  // Display the voltage
  display.setCursor(0, 30);
  display.print("Voltage: ");
  display.print(batteryVoltage);
  display.println(" V");
  
  // Display updated information
  display.display();
  
  // Small delay to reduce power consumption
  delay(1000);
}

// Function to blink an LED
void blinkLED(int ledPin) {
  digitalWrite(ledPin, HIGH);
  delay(500); // LED ON for 500 ms
  digitalWrite(ledPin, LOW);
  delay(500); // LED OFF for 500 ms
}