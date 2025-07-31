// master.ino

// 1. Include necessary libraries
#include <Wire.h>         // For I2C communication with MPU-6050
#include <MPU6050.h>      // Library for MPU-6050 IMU
#include <SD.h>           // For SD card module communication
#include <TinyGPS++.h>    // For parsing GPS data from NEO-6M
#include <SoftwareSerial.h> // For UART communication with NEO-6M GPS and Slave Arduino Pro Mini
#include <Arduino.h>      // Standard Arduino core library

// 2. Define Pin Assignments (Example - adjust based on your actual wiring)
// MPU-6050: Uses I2C (SDA, SCL pins on Mega)
#define GPS_RX_PIN 10 // RX pin for GPS (connects to GPS TX)
#define GPS_TX_PIN 11 // TX pin for GPS (connects to GPS RX)

#define SLAVE_RX_PIN 12 // RX pin for Slave (connects to Slave TX)
#define SLAVE_TX_PIN 13 // TX pin for Slave (connects to Slave RX)

#define SD_CS_PIN 4  // Chip Select pin for SD card module

// Push Buttons for Annotation
#define POTHOLE_BUTTON_PIN 22
#define HARDBRAKE_BUTTON_PIN 23
#define SPEEDBREAKER_BUTTON_PIN 24

// Buzzer for Overspeed Notifications
#define BUZZER_PIN 25

// SIM800L GSM Module
#define GSM_RX_PIN 16
#define GSM_TX_PIN 17

// 3. Create instances of sensor/module objects
MPU6050 mpu;               // MPU-6050 object
TinyGPSPlus gps;           // TinyGPS++ object for GPS data
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);    // SoftwareSerial for GPS module
SoftwareSerial slaveSerial(SLAVE_RX_PIN, SLAVE_TX_PIN);  // SoftwareSerial for communication with slave
SoftwareSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN);    // SoftwareSerial for GSM module

File dataFile;             // File object for SD card operations

// 4. Global variables for data storage and flags
int16_t accelX, accelY, accelZ; // 3-axis Accelerometer values
int16_t gyroX, gyroY, gyroZ;    // 3-axis Gyroscope values

double latitude, longitude;     // Latitude and Longitude
float speed_kmph;               // Speed in km/h

String slave_timestamp;         // Time and date stamps
float temperature;              // Temperature
float humidity;                 // Humidity

// Annotation flags for Pothole, Hard Brake, and Speed Breaker
int pothole_flag = 0;
int hardbrake_flag = 0;
int speedbreaker_flag = 0;

// Data logging frequency
const unsigned long DATA_COLLECTION_INTERVAL_MS = 50; // Approximately 20 Hz

// 5. setup() function - Runs once at startup
void setup() {
  // Initialize Serial communication for debugging
  Serial.begin(9600);
  while (!Serial);

  // Initialize I2C communication
  Wire.begin();

  // Initialize MPU-6050
  Serial.println("Initializing MPU-6050...");
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU-6050 connection successful.");
  } else {
    Serial.println("MPU-6050 connection failed!");
  }

  // Initialize GPS SoftwareSerial
  gpsSerial.begin(9600);

  // Initialize Slave SoftwareSerial
  slaveSerial.begin(9600);

  // Initialize GSM SoftwareSerial
  gsmSerial.begin(9600);

  // Initialize SD card module
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    while (true); // Halt execution if SD card fails
  }
  Serial.println("SD card initialized.");

  // Create header for data file if it doesn't exist
  dataFile = SD.open("ROADATA.CSV", FILE_WRITE);
  if (dataFile) {
    if (dataFile.size() == 0) {
      dataFile.println("Accel_x,Accel_y,Accel_z,Gyro_x,Gyro_y,Gyro_z,Pothole,Hand_brake,Speed_breaker,lat,long,speed,hum,temp,Timestamp");
      Serial.println("CSV header written.");
    }
    dataFile.close();
  } else {
    Serial.println("Error opening data file for header!");
  }

  // Configure button pins as INPUT_PULLUP
  pinMode(POTHOLE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(HARDBRAKE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SPEEDBREAKER_BUTTON_PIN, INPUT_PULLUP);

  // Configure buzzer pin as OUTPUT
  pinMode(BUZZER_PIN, OUTPUT);

  // Initial state of buzzer
  digitalWrite(BUZZER_PIN, LOW);
}

// 6. loop() function - Runs repeatedly
void loop() {
  static unsigned long lastDataCollectionTime = 0;

  // Check if it's time to collect data
  if (millis() - lastDataCollectionTime >= DATA_COLLECTION_INTERVAL_MS) {
    lastDataCollectionTime = millis();

    // Read MPU-6050 data
    mpu.getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);

    // Read GPS data
    while (gpsSerial.available()) {
      if (gps.encode(gpsSerial.read())) {
        if (gps.location.isValid()) {
          latitude = gps.location.lat();
          longitude = gps.location.lng();
        }
        if (gps.speed.isValid()) {
          speed_kmph = gps.speed.kmph();
        }
      }
    }

    // Read data from Slave Arduino Pro Mini
    if (slaveSerial.available()) {
      String receivedData = slaveSerial.readStringUntil('\n');
      int firstComma = receivedData.indexOf(',');
      int secondComma = receivedData.indexOf(',', firstComma + 1);

      if (firstComma != -1 && secondComma != -1) {
        slave_timestamp = receivedData.substring(0, firstComma);
        temperature = receivedData.substring(firstComma + 1, secondComma).toFloat();
        humidity = receivedData.substring(secondComma + 1).toFloat();
      }
    }

    // Read push button states (active LOW)
    pothole_flag = !digitalRead(POTHOLE_BUTTON_PIN);
    hardbrake_flag = !digitalRead(HARDBRAKE_BUTTON_PIN);
    speedbreaker_flag = !digitalRead(SPEEDBREAKER_BUTTON_PIN);

    // Log collected data to SD card
    dataFile = SD.open("ROADATA.CSV", FILE_WRITE);
    if (dataFile) {
      String dataRow = "";
      dataRow += String(accelX) + ",";
      dataRow += String(accelY) + ",";
      dataRow += String(accelZ) + ",";
      dataRow += String(gyroX) + ",";
      dataRow += String(gyroY) + ",";
      dataRow += String(gyroZ) + ",";
      dataRow += String(pothole_flag) + ",";
      dataRow += String(hardbrake_flag) + ",";
      dataRow += String(speedbreaker_flag) + ",";
      dataRow += String(latitude, 6) + ",";
      dataRow += String(longitude, 6) + ",";
      dataRow += String(speed_kmph) + ",";
      dataRow += String(humidity) + ",";
      dataRow += String(temperature) + ",";
      dataRow += slave_timestamp;

      dataFile.println(dataRow);
      dataFile.close();
    } else {
      Serial.println("Error opening data file for writing!");
    }

    // Buzzer notification for overspeeding
    const float SPEED_LIMIT_KM_H = 60.0;
    if (speed_kmph > SPEED_LIMIT_KM_H) {
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }

    // GSM transmission placeholder (commented out)
    /*
    static unsigned long lastGsmSendTime = 0;
    const unsigned long GSM_SEND_INTERVAL_MS = 5 * 60 * 1000;
    if (millis() - lastGsmSendTime >= GSM_SEND_INTERVAL_MS) {
      lastGsmSendTime = millis();
      String gsmPayload = "{ \"lat\": " + String(latitude, 6) + ", \"lon\": " + String(longitude, 6) + " }";
      // Send data via GSM using AT commands here
    }
    */
  }

  // Optional: small delay
  // delay(1);
}
