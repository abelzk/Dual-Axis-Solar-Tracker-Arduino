#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>

const unsigned long eventInterval = 1000;
unsigned long previousTime  = 0; // Corrected type from `unsigned` to `unsigned long`

// WiFi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// API URL and Key (Update with actual values)
const String apiURL = "http://api.openweathermap.org/data/2.5/onecall";
const String apiKey = "YOUR_API_KEY";

// Servo parameters
Servo horizontal; // horizontal servo
Servo vertical;   // vertical servo
int servoh = 180;
int servohLimitHigh = 175;
int servohLimitLow = 0;
int servov = 180;
int servovLimitHigh = 175;
int servovLimitLow = 1;

// LDR pin connections
int ldrlt = A3; // LDR top left
int ldrrt = A2; // LDR top right
int ldrld = A0; // LDR down left
int ldrrd = A1; // LDR down right

// LCD setup
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

float voltage = 0.0;
float temp = 0.0; // variable to store temperature (digital value of the input)
float current;
int power;
float r1 = 10.0;
float r2 = 10.0;
float value = r2 / (r1 + r2);
int analog_value;
int sensor_value = 0; // corrected typo from `sesnor_value`
bool manualOverride = false;
int manualButtonPin = 8; // Pin for manual override button

void setup() {
  // Servo setup
  horizontal.attach(9); // Connect pin 9 to horizontal servo
  vertical.attach(10);  // Connect pin 10 to vertical servo
  horizontal.write(180);
  vertical.write(180);
  delay(500);

  // Setup WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // LCD setup
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(" SMART SOLAR ");
  lcd.setCursor(0, 1);
  lcd.print("  TRACKER  ");

  // Manual Override setup
  pinMode(manualButtonPin, INPUT);
}

void loop() {
  checkManualOverride();
  if (manualOverride) {
    // Implement manual control logic here
  } else {
    fetchSolarData();
    adjustSolarPanel();
  }
  liquid_crystal();
}

void fetchSolarData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = apiURL + "?lat=YOUR_LAT&lon=YOUR_LON&appid=" + apiKey;
    http.begin(url.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      // Parse JSON data and extract solarElevation, solarAzimuth
      // Example: Use ArduinoJson library to parse the JSON
      int solarElevation = 45; // Placeholder value
      int solarAzimuth = 180;  // Placeholder value
      adjustSolarPanel(solarElevation, solarAzimuth);
    } else {
      Serial.println("Error on HTTP request");
      // Fallback to manual control or last known good data
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    // Handle disconnected state
  }
}

void adjustSolarPanel(int solarElevation = -1, int solarAzimuth = -1) {
  // If API data is provided, adjust based on that
  if (solarElevation != -1 && solarAzimuth != -1) {
    int verticalAngle = map(solarElevation, 0, 90, servovLimitLow, servovLimitHigh);
    int horizontalAngle = map(solarAzimuth, 0, 360, servohLimitLow, servohLimitHigh);
    vertical.write(verticalAngle);
    horizontal.write(horizontalAngle);
  } else {
    // Otherwise, use LDRs to adjust position
    int lt = analogRead(ldrlt) * 15;
    int rt = analogRead(ldrrt) * 15;
    int ld = analogRead(ldrld) * 15;
    int rd = analogRead(ldrrd) * 15;
    int tol = 90;
    int avt = (lt + rt);
    int avd = (ld + rd);
    int avl = (lt + ld);
    int avr = (rt + rd);
    int dvert = avt - avd;
    int dhoriz = avl - avr;

    if (abs(dvert) > tol) {
      if (avt > avd) {
        servov = ++servov;
        if (servov > servovLimitHigh) {
          servov = servovLimitHigh;
        }
      } else if (avt < avd) {
        servov = --servov;
        if (servov < servovLimitLow) {
          servov = servovLimitLow;
        }
      }
      vertical.write(servov);
    }

    if (abs(dhoriz) > tol) {
      if (avl > avr) {
        servoh = --servoh;
        if (servoh < servohLimitLow) {
          servoh = servohLimitLow;
        }
      } else if (avl < avr) {
        servoh = ++servoh;
        if (servoh > servohLimitHigh) {
          servoh = servohLimitHigh;
        }
      }
      horizontal.write(servoh);
    }
  }
}

void liquid_crystal() {
  analog_value = analogRead(A4); // Reading the analog value
  voltage = 2 * (analog_value * 5.0) / 1024.0; // Converting the analog value to digital
  current = voltage / (r1 + r2);
  power = sq(voltage) / current;

  unsigned long currentTime = millis();
  if (currentTime - previousTime >= eventInterval) {
    previousTime = currentTime;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("V: ");
    lcd.print(voltage);
    lcd.print("v P:");
    lcd.print(power);
    lcd.print("mW");
    lcd.setCursor(0, 1);
    lcd.print("I: ");
    lcd.print(current);
    lcd.print("A");

    // Serial output
    Serial.print("DATA,TIME,");
    Serial.print(voltage);
    Serial.print(",");
    Serial.print(current);
    Serial.print(",");
    Serial.println(power);
  }
}

void checkManualOverride() {
  if (digitalRead(manualButtonPin) == HIGH) {
    manualOverride = !manualOverride; // Toggle override mode
  }
}
