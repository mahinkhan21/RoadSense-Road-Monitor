// slave.ino

#include <Arduino.h>
#include <DHT.h>

// Define sensor pin and type
#define DHT_PIN 2
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

// Variables
String timestamp = "00:00:00"; // You can update this if you have a real-time clock or other time source

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Read temperature and humidity
  float temperature = dht.readTemperature(); // Celsius
  float humidity = dht.readHumidity();

  // Check if reads failed and handle
  if (isnan(temperature) || isnan(humidity)) {
    // Could not read sensor, send empty or error values
    temperature = -999.0;
    humidity = -999.0;
  }

  // Update timestamp if you have RTC or other method here
  // For now, just use millis() as a simple placeholder timestamp in seconds
  unsigned long seconds = millis() / 1000;
  unsigned long hrs = (seconds / 3600) % 24;
  unsigned long mins = (seconds / 60) % 60;
  unsigned long secs = seconds % 60;
  char timeBuffer[9];
  snprintf(timeBuffer, sizeof(timeBuffer), "%02lu:%02lu:%02lu", hrs, mins, secs);
  timestamp = String(timeBuffer);

  // Prepare data string: "timestamp,temperature,humidity"
  String dataToSend = timestamp + "," + String(temperature, 1) + "," + String(humidity, 1);

  // Send data over Serial to master
  Serial.println(dataToSend);

  // Send data every 1 second (adjust as needed)
  delay(1000);
}
