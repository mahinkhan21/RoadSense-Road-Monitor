# 📦 RoadSense Hardware Parts List

This document contains the complete bill of materials (BoM) required to build the RoadSense device for real-time road condition monitoring.

---

## 🔧 Microcontrollers

| Qty | Component            | Specifications             | Purpose              |
|-----|----------------------|----------------------------|----------------------|
| 1   | Arduino Mega 2560    | ATmega2560, 5V logic       | Master controller    |
| 1   | Arduino Pro Mini     | ATmega328P, 3.3V or 5V     | Slave controller     |

---

## 📡 Sensors & Modules

| Qty | Component            | Specifications                                       | Purpose                        |
|-----|----------------------|------------------------------------------------------|--------------------------------|
| 1   | MPU-6050 IMU         | 3-axis Accelerometer + 3-axis Gyroscope (I2C)        | Detect motion and vibration    |
| 1   | NEO-6M GPS Module    | UART, 50 channels, 1–5 Hz update rate                | Track location and speed       |
| 1   | DHT22 Sensor         | Temp: -40–125°C, Humidity: 0–100%, ±2% accuracy      | Measure environmental data     |
| 1   | DS3231 RTC Module    | I2C, battery backup, highly accurate clock           | Timestamp data entries         |
| 1   | SIM800L GSM Module   | Quad-band GSM, UART                                  | Transmit data to the cloud     |
| 1   | MicroSD Card Module  | SPI, FAT32 formatted storage                         | Log data locally               |

---

## 🧩 User Interaction & Output

| Qty | Component        | Specifications                    | Purpose                         |
|-----|------------------|-----------------------------------|---------------------------------|
| 3   | Push Buttons     | Momentary, Normally Open (NO)     | Annotate road conditions        |
| 1   | Buzzer           | Active/passive, 5V                | Alert driver for speed breaches |

---

## 🧰 Additional Components

| Qty | Component                | Specifications                   | Purpose                          |
|-----|--------------------------|----------------------------------|----------------------------------|
| 1   | CR2032 Coin Cell Battery | 3V lithium                       | Backup power for RTC             |
| 1   | Breadboard or PCB        | Standard size                    | Circuit assembly                 |
| -   | Jumper Wires             | Male-Male, Male-Female           | Connections between components   |
| 1   | 5V Power Supply          | USB or Li-ion with 5V regulator  | Power both microcontrollers      |
| 1   | Logic Level Shifter      | Optional                         | For SIM800L/SD (if using 3.3V)   |

---

## 🔗 Reference Datasheets & Links

- [MPU-6050 Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
- [NEO-6M GPS Info](https://www.u-blox.com/en/product/neo-6-series)
- [SIM800L GSM Module](https://simcom.ee/modules/gsm-gprs/sim800l/)
- [DS3231 RTC Datasheet](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf)

---

## ⚠️ Notes

- **I2C Conflict Avoidance**: MPU-6050 and DS3231 share the same I2C address — hence the dual microcontroller setup.
- **Power Considerations**: SIM800L requires stable 3.7–4.2V power with enough current (~2A peak). Use capacitors or a dedicated LDO regulator.
- **Storage**: Ensure SD card is formatted to FAT32 and uses a compatible voltage level (3.3V logic).
