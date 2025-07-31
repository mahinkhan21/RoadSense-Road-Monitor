# 🚗 RoadSense: Real-Time Road Condition Monitoring Device

**RoadSense** is a low-cost, real-time road condition monitoring system that uses a dual-microcontroller setup and a combination of motion, environmental, and location sensors to detect road anomalies such as potholes, speed breakers, and hard braking events.

> 📄 **Published in:** IEEE Transactions on Intelligent Vehicles  
> 📌 **DOI:** [10.1109/TIV.2024.3486020](https://doi.org/10.1109/TIV.2024.3486020)  
> 🧠 **Authors:** Insan Arafat Jahan, Armana Sabiha Huq, Mahin Khan Mahadi, Insan Arafat Jamil, Md. Zidan Shahriar

---

## 📦 Repository Contents

| Folder/File           | Description                                          |
|-----------------------|------------------------------------------------------|
| `code/`               | Arduino code for master (Mega) and slave (Pro Mini) |
| `hardware/`           | Circuit diagrams, Proteus simulation files          |
| `docs/`               | System architecture diagrams and flowcharts         |
| `sample_data/`        | Example CSV dataset (accelerometer, GPS, etc.)      |
| `parts_list.md`       | Full hardware bill of materials                     |
| `PAPER_LINK.md`       | Link and citation for the published research paper  |

---

## 🛠️ System Overview

The RoadSense device uses:
- **MPU-6050** IMU for motion detection (3-axis Accelerometer + 3-axis Gyroscope)  
- **NEO-6M GPS** for velocity and location  
- **DHT22** for temperature and humidity  
- **DS3231** RTC for accurate timestamping  
- **SIM800L GSM** for cloud upload  
- **MicroSD module** for local data storage  
- **3 Push buttons** for manual data annotation  
- **Buzzer** for overspeed warnings  

A dual-microcontroller setup (Arduino Mega 2560 as master and Arduino Pro Mini as slave) is employed to manage communication challenges, such as shared I2C addresses, and ensure reliable data acquisition.

---

## 🔧 Setup Instructions

### 1. Wiring & Hardware
Refer to the `hardware/` folder for detailed wiring diagrams and the Proteus `.pdsprj` simulation file. The `parts_list.md` also contains a comprehensive list of all components.

### 2. Code Upload
- **Master Controller** (Arduino Mega 2560):  
  Upload the code found in `code/master_controller/master.ino`.
- **Slave Controller** (Arduino Pro Mini):  
  Upload the code found in `code/slave_controller/slave.ino`.

### 3. Required Libraries (Arduino IDE)
Ensure you install the following libraries in your Arduino IDE before compiling:
- `Wire.h`
- `RTClib`
- `DHT.h`
- `SoftwareSerial.h`
- `TinyGPS++.h`
- `SD.h`
- `MPU6050.h` (typically found in the `i2cdevlib` library by Jeff Rowberg)

### 4. Data Logging Format
Data is logged in CSV format and includes the following columns:
- `Accel_x`, `Accel_y`, `Accel_z` (3-axis Accelerometer values)  
- `Gyro_x`, `Gyro_y`, `Gyro_z` (3-axis Gyroscope values)  
- `Pothole`, `Hard_brake`, `Speed_breaker` (Annotation flags: `1` for occurrence, `0` for absence)  
- `latt`, `long` (Latitude, Longitude from GPS)  
- `speed` (Vehicle speed in km/h)  
- `hum`, `temp` (Humidity and Temperature)  

---

## 📡 Cloud Communication (Optional)

The SIM800L GSM module facilitates data transfer to a cloud server using the MQTT protocol.  
All communications are secured by TLS Version 1.3 and AES-based cryptographic algorithms.  
The cloud server only accepts requests from registered MAC addresses to minimize external attacks.

---

## 📊 Dataset Access

The dataset collected during the RoadSense project test runs is available **upon request** for academic or research purposes.

If you are using our dataset or building upon this work, please feel free to cite our research paper:

> **RoadSense: A Framework for Road Condition Monitoring using Sensors and Machine Learning**  
> *IEEE Transactions on Intelligent Vehicles, 2024*  
> [DOI: 10.1109/TIV.2024.3486020](https://doi.org/10.1109/TIV.2024.3486020)

### 📚 Suggested Citation (BibTeX)

```bibtex
@article{jahan2024roadsense,
  title={RoadSense: A Framework for Road Condition Monitoring using Sensors and Machine Learning},
  author={Jahan, Insan Arafat and Huq, Armana Sabiha and Mahadi, Mahin Khan and Jamil, Insan Arafat and Shahriar, Md Zidan},
  journal={IEEE Transactions on Intelligent Vehicles},
  year={2024},
  publisher={IEEE}
}
