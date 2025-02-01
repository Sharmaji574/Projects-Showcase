#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Pin definitions
#define TEMP_SENSOR_PIN A1  // TEG Peltier module connected to A1 (analog pin)
#define RED_LED_PIN 3       // Red LED connected to D3
#define YELLOW_LED_PIN 4    // Yellow LED connected to D4
#define GREEN_LED_PIN 5     // Green LED connected to D5

// OLED display settings
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialize serial monitor (optional)
  Serial.begin(9600);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  // Initialize LED pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
}

void loop() {
  // Read voltage from TEG Peltier module (Analog input from A1)
  int sensorValue = analogRead(TEMP_SENSOR_PIN);
  
  // Convert the sensor reading (0-1023) to voltage (0-5V)
  float voltage = (sensorValue * 5.0 / 1023.0); 
  
  // Display voltage on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Voltage: ");
  display.print(voltage);
  display.println(" V");

  // Control LED indicators based on TEG voltage range and display messages
  if (voltage >= 0.26 && voltage <= 0.27) {
    // Red LED for Low Heat
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    display.setCursor(0, 10);
    display.print("Low Heat");
    delay(500);  // Keep the red LED on for 500ms
    digitalWrite(RED_LED_PIN, LOW);
    delay(500);  // Off for 500ms
    delay(2000); // Keep the red LED blinking for 3 seconds (2 more cycles)
  } else if (voltage <= 0.28) {
    // Yellow LED for OK Heat
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
    display.setCursor(0, 10);
    display.print("OK Heat");
    delay(500);  // Keep the yellow LED on for 500ms
    digitalWrite(YELLOW_LED_PIN, LOW);
    delay(500);  // Off for 500ms
    delay(2000); // Keep the yellow LED blinking for 3 seconds (2 more cycles)
  } else if (voltage > 0.28 && voltage >= 0.30) {
    // Green LED for GG Heat
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    display.setCursor(0, 10);
    display.print(":) GG Heat");
    delay(500);  // Keep the green LED on for 500ms
    digitalWrite(GREEN_LED_PIN, LOW);
    delay(500);  // Off for 500ms
    delay(2000); // Keep the green LED blinking for 3 seconds (2 more cycles)
  }

  display.display(); // Refresh the display

  // Print voltage to serial monitor (optional for debugging)
  Serial.print("Voltage: ");
  Serial.println(voltage);

  delay(1000); // Wait for 1 second before the next reading
}