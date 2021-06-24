/**
 * Copyright (c) 2021 Artem Hlumov
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <dht_nonblocking.h>
#include <LiquidCrystal.h>

// Define pins for LCD display.
#define LCD_RS_PIN 7
#define LCD_E_PIN 8
#define LCD_DB4_PIN 9
#define LCD_DB5_PIN 10
#define LCD_DB6_PIN 11
#define LCD_DB7_PIN 12
// Define a data pin for temperature-humidity sensor.
#define DHT_DATA_PIN 2
// Pin for an LED that should blink when we take measurements.
#define MEASUREMENTS_LED 13

// Delay in milliseconds that defins how often measurements should be taken.
#define MEASUREMENTS_UPDATE_DELAY (10*1000)

// Initialize temperature-humidity sensor.
DHT_nonblocking dhtSensor(DHT_DATA_PIN, DHT_TYPE_11);

// Initiaize LCD display.
LiquidCrystal lcd(LCD_RS_PIN, LCD_E_PIN, LCD_DB4_PIN, LCD_DB5_PIN, LCD_DB6_PIN, LCD_DB7_PIN);

// Set up function.
void setup() {
  // Initialize LCD display with amount of rows and columns.
  lcd.begin(16, 2);
  // Input from the temperature-humidity sensor.
  pinMode(DHT_DATA_PIN, INPUT);
}

// Last saved value of temperature.
float temperature;
// Last saved value of humidity.
float humidity;
// Timestamp of last taken measurements.
// Initially it is set to -MEASUREMENTS_UPDATE_DELAY in order to initiate the first measurement immediately.
long lastMeasurementsTaken = -MEASUREMENTS_UPDATE_DELAY;

void loop() {
  // Take measurements if it is time to do that.
  if (millis() - lastMeasurementsTaken > MEASUREMENTS_UPDATE_DELAY) {
    // Blink with built-in LED.
    digitalWrite(MEASUREMENTS_LED, HIGH);
    // Take measurements. This may take several attempt to be done.
    while(!dhtSensor.measure(&temperature, &humidity));
    digitalWrite(MEASUREMENTS_LED, LOW);
    // Print out current temperature.
    lcd.setCursor(0, 0);
    lcd.print("T = ");
    lcd.print(temperature, 1);
    lcd.print(" C");
    // Print out current humidity value.
    lcd.setCursor(0, 1);
    lcd.print("H = ");
    lcd.print(humidity, 1);
    lcd.print(" %");
    // Save the last measurements timestamp.
    lastMeasurementsTaken = millis();
  }
}
