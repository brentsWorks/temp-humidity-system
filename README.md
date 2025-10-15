# 🌡️ Raspberry Pi Environmental Monitoring System

A lightweight embedded system built with a Raspberry Pi to collect, 
display, and log temperature and humidity data in real time. Sensor data 
is stored in a MySQL database and can be queried through a simple 
command-line C program.

---

## 🚀 Features

- Real-time temperature and humidity collection using a DHT11 sensor  
- LCD display output via I2C for on-device visualization  
- MySQL database integration for persistent data logging  
- Menu-driven CLI to query min, max, and average statistics over custom 
time ranges

---

## 🛠️ Tech Stack

- **Language:** C  
- **Hardware:** Raspberry Pi III/IV, DHT11 Sensor, 16×2 LCD  
- **Database:** MySQL  
- **Libraries:** WiringPi, MySQL C API

---

## 📂 Project Structure

main.c                    # CLI for querying MySQL statistics
DHT11Console_LCDOutput.c  # Sensor reading & LCD display logic
data_analysis.h           # Data processing and SQL helpers
I2C-1.h                   # LCD I2C control functions
sql.h                     # MySQL connection credentials (excluded from 
repo)

