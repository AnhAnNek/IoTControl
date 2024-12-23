#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

using namespace websockets;

// Pin configuration
#define TEMP_PIN 26      // DS18B20 DATA pin
#define FAN_PWM_PIN 25   // MOS IRF520N PWM pin

// Wi-Fi credentials
const char* ssid = "Anh An";
const char* password = "qwertyuiop";

// WebSocket server configuration
const char* websockets_server = "192.168.31.100:8000"; // Server address and port

// WebSocket client
WebsocketsClient client;

// Temperature sensor library
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

// Global variables
float currentTemp = 0.0;
int fanSpeed = 0; // Value range: 0-255

// Function to read temperature
void readTemperature() {
    sensors.requestTemperatures();
    currentTemp = sensors.getTempCByIndex(0);
}

// WebSocket message handler
void onMessageCallback(WebsocketsMessage message) {
    String msg = message.data();
    Serial.println("Received: " + msg);

    // Check and handle fan speed control command
    if (msg.startsWith("SET_SPEED:")) {
        fanSpeed = msg.substring(10).toInt();
        ledcWrite(0, fanSpeed);
        Serial.println("Fan Speed Set To: " + String(fanSpeed));
    }
}

// WebSocket event handler
void onEventsCallback(WebsocketsEvent event, String data) {
    if (event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("WebSocket Connection Opened");
    } else if (event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("WebSocket Connection Closed");
    } else if (event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if (event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

void setup() {
    // Initialize Serial
    Serial.begin(115200);

    // Start the temperature sensor
    sensors.begin();

    // Configure PWM for fan control
    ledcSetup(0, 25000, 8); // Channel 0, 25kHz frequency, 8-bit resolution
    ledcAttachPin(FAN_PWM_PIN, 0); // Attach FAN_PWM_PIN to channel 0

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);

    // Wait for Wi-Fi connection
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nWi-Fi connected. IP address: " + WiFi.localIP().toString());

    // Setup WebSocket callbacks
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);

    // Connect to the WebSocket server
    if (client.connect(websockets_server)) {
        Serial.println("Connected to WebSocket server");
    } else {
        Serial.println("Failed to connect to WebSocket server");
    }
}

void loop() {
    // Read temperature
    readTemperature();

    // Create JSON object
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["temperature"] = currentTemp;
    jsonDoc["fanSpeed"] = map(fanSpeed, 0, 255, 0, 100); // Convert fan speed to percentage

    // Serialize JSON to a string
    String jsonString;
    serializeJson(jsonDoc, jsonString);

    // Send JSON data to the WebSocket server
    if (client.available()) {
        client.send(jsonString);
    }

    // Maintain WebSocket connection
    client.poll();

    // Wait before the next update
    delay(1000);
}
