#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>

// WiFi Credentials
const char* ssid = "SSID";         
const char* password = "PASSWORD"; 

// Motor Control Pins enter as per your configs
#define IN1 2
#define IN2 4
#define IN3 5
#define IN4 18

// Thermal Sensor
Adafruit_AMG88xx amg;
WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    
    // WiFi Setup
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

    // Initialize Thermal Sensor
    if (!amg.begin()) {
        Serial.println("Failed to initialize AMG8833!");
        while (1);
    } else {
        Serial.println("AMG8833 Initialized!");
    }

    // Initialize Motor Pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    
    server.begin();
}

// 🚗 Motor Control Functions
void moveForward() {
    Serial.println("Moving Forward");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void moveBackward() {
    Serial.println("Moving Backward");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void turnLeft() {
    Serial.println("Turning Left");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnRight() {
    Serial.println("Turning Right");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void stopMotors() {
    Serial.println("Motors Stopped");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

// 🌡 Thermal Data
void sendThermalData(WiFiClient client) {
    float pixels[64];
    amg.readPixels(pixels);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print("[");
    for (int i = 0; i < 64; i++) {
        client.print(pixels[i]);
        if (i < 63) client.print(",");
    }
    client.println("]");
}

// 🌐 Web Handling
void loop() {
    WiFiClient client = server.available();
    if (client) {
        String request = client.readStringUntil('\r');
        client.flush();

        Serial.println("Client connected");
        Serial.println("Request: " + request); // Debugging Request

        // Handle Motor Commands
        if (request.indexOf("/forward") != -1) {
            moveForward();
        } else if (request.indexOf("/backward") != -1) {
            moveBackward();
        } else if (request.indexOf("/left") != -1) {
            turnLeft();
        } else if (request.indexOf("/right") != -1) {
            turnRight();
        } else if (request.indexOf("/stop") != -1) {
            stopMotors();
        } else if (request.indexOf("/thermal_data") != -1) {
            Serial.println("Thermal Data Sent");
            sendThermalData(client);
        } else {
            Serial.println("Unknown Command Received");
        }

        client.stop();
    }
}
