# 🤖 ThermoBot – Thermal Inspection Bot

**ThermoBot** is an IoT-based mobile robotic system built for remote **thermal inspection** and **surveillance**. It integrates an **ESP32-CAM** for live video streaming and an **AMG8833 thermal sensor** for heat signature visualization. Controlled wirelessly over Wi-Fi via a **custom Android app** built on **MIT App Inventor**, it can be used in hazardous or inaccessible environments.

---

## 🔥 Features

- 🔴 Real-time thermal imaging using AMG8833 (8x8 infrared thermal array)
- 📸 Live video streaming from ESP32-CAM over IP
- 📱 Android App (MIT App Inventor) for robot control & feed access
- 🚗 Mobile robot movement (forward, backward, left, right, stop)
- 🌐 Wi-Fi-based local access with browser IP
- 💻 Thermal visualization via Python-based heatmap
- 🛠️ Low-cost, DIY hardware setup

---

## 🧠 How It Works

- ESP32-CAM streams live video via HTTP server (`http://<ESP32-IP>`)
- ESP32 reads data from AMG8833 and sends 8x8 thermal matrix
- Python (`thermal_processing.py`) reads serial data and creates real-time heatmaps
- Android App (via MIT App Inventor) controls movement and shows IP stream

---

## 📱 Android App – MIT App Inventor

The custom Android app was built using **MIT App Inventor** with the following features:

- UI to control directions (FWD, BWD, LEFT, RIGHT, STOP)
- Input field to set the ESP32-CAM **IP Address**
- Button to **open live video feed in browser** or WebViewer component
- Sends motor control commands over Wi-Fi

> 📲 The app communicates using HTTP GET or Wi-Fi socket.  
> Example: `http://<ESP-IP>/forward` triggers the bot to move forward.

---

## 🛠️ Tech Stack

| Component         | Description                         |
|------------------|-------------------------------------|
| **ESP32-CAM**     | For video streaming and processing |
| **AMG8833**       | Thermal sensor (8x8 heatmap data)  |
| **L298N**         | Motor driver for movement          |
| **MIT App Inventor** | Android app development        |
| **Python (OpenCV)** | For heatmap generation on PC    |
| **Arduino IDE**   | Programming ESP32 modules          |

---
