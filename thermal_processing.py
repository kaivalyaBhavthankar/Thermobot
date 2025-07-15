#!/usr/bin/env python3
import cv2
import numpy as np
import requests
from flask import Flask, Response

app = Flask(_name_)

ESP32_IP = "ENTER IP HERE"  # Replace with your ESP32's actual IP

# Thermal Data Processing
def get_thermal_frame():
    while True:
        try:
            response = requests.get(f"http://{ESP32_IP}/thermal_data", timeout=2)
            if response.status_code == 200:
                raw_data = response.text.strip("[]\r\n").split(",")
                thermal_data = [float(x.strip()) for x in raw_data if x.strip().replace('.', '', 1).isdigit()]

                if len(thermal_data) != 64:
                    print("Invalid data received. Skipping frame.")
                    continue

                # Reshape data and apply heatmap
                thermal_array = np.array(thermal_data).reshape((8, 8))
                thermal_img = cv2.resize(thermal_array, (320, 320), interpolation=cv2.INTER_CUBIC)
                heatmap = cv2.applyColorMap(cv2.convertScaleAbs(thermal_img, alpha=3), cv2.COLORMAP_JET)

                # Encode as JPEG for MJPEG stream
                _, jpeg = cv2.imencode('.jpg', heatmap)

                # MJPEG stream format for WebViewer
                yield (b'--frame\r\n'
                       b'Content-Type: image/jpeg\r\n\r\n' + jpeg.tobytes() + b'\r\n')

        except Exception as e:
            print("Error:", e)

@app.route('/thermal_feed')
def video_feed():
    return Response(get_thermal_frame(), mimetype='multipart/x-mixed-replace; boundary=frame')

if _name_ == "_main_":
    app.run(host="0.0.0.0", port=5000, debug=False)
