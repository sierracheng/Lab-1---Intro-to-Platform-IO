#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_VEML7700.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Sensor and display objects
Adafruit_BME280 bme;
Adafruit_VEML7700 veml;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin and thresholds
const int lightPin = 1;
const float tempThreshold = 30.0;
const float lightLowThreshold = 50.0;
const float lightHighThreshold = 100.0;

void setup() {
  Serial.begin(9600);

  if (!bme.begin(0x76) || !veml.begin() || !display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Sensor initialization failed. Check wiring!");
    while (1);
  }

  pinMode(lightPin, OUTPUT);
  display.clearDisplay();
  display.display();
  delay(2000);
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;
  float light = veml.readLux();

  displayDataOnOLED(temperature, humidity, pressure, light);
  controlLightBasedOnIntensity(light);

  delay(2000);
}

void displayDataOnOLED(float temp, float hum, float press, float lightIntensity) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temp: "); display.print(temp); display.println(" C");
  display.print("Hum: "); display.print(hum); display.println(" %");
  display.print("Pres: "); display.print(press); display.println(" hPa");
  display.print("Light: "); display.print(lightIntensity); display.println(" Lux");
  display.display();
}

void controlLightBasedOnIntensity(float lightIntensity) {
  if (lightIntensity < lightLowThreshold) {
    digitalWrite(lightPin, HIGH);
  } else if (lightIntensity > lightHighThreshold) {
    digitalWrite(lightPin, LOW);
  }
}