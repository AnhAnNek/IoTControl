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

// Pin Configuration
#define TEMP_PIN 26      // DS18B20 DATA pin
#define FAN_PWM_PIN 16   // GPIO for fan speed control (L298N ENA or IRF520N G)
#define PHOTORESISTOR_PIN 17

// DS18B20 Setup
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

const char* ssid = "Anh An"; //Enter SSID
const char* password = "qwertyuiop"; //Enter Password
const char* websockets_server_host = "192.168.31.2"; //Enter server adress
const uint16_t websockets_server_port = 8000; // Enter server port

// WebSocket Setup
using namespace websockets;
WebsocketsClient client;

// Global Variables
float currentTemp = 0.0;
int fanSpeed = 0; // PWM value (0-255)
const char* websocketServer = "192.168.31.2"; // Replace with your WebSocket server's IP or hostname
const int websocketPort = 8000;                // Replace with your WebSocket server's port
const char* websocketPath = "/ws";            // Replace with your WebSocket server's path

void setup() {
  Serial.begin(115200);

  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }

  // Check if connected to wifi
  if(WiFi.status() != WL_CONNECTED) {
      Serial.println("No Wifi!");
      return;
  }

  // Print IP address when connected
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Try to connect to WebSocket server
  Serial.print("Connecting to WebSocket server...");
  bool connected = client.connect(websockets_server_host, websockets_server_port, "/ws");
  if (connected) {
    Serial.println("Connected to WebSocket server!");
    client.send("Hello Server");
  } else {
    Serial.println("Failed to connect to WebSocket server!");
  }
  
  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message){
      Serial.print("Got Message: ");
      Serial.println(message.data());
      handleServerMessage(message.data().c_str());
  });

  // Start the temperature sensor
  sensors.begin();
  Serial.println("Temperature-based Fan Control with WebSocket");

  // Initialize the FAN_PWM_PIN as output
  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(PHOTORESISTOR_PIN, OUTPUT);
}

void loop() {
  // let the websockets client check for incoming messages
  if(client.available()) {
      client.poll();
  }

  // Read temperature
  sensors.requestTemperatures();
  currentTemp = sensors.getTempCByIndex(0);

  if (currentTemp == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature!");
    return;
  }

  // Map temperature to fan speed (example: 30°C = 50%, 60°C = 100%)
  if (currentTemp < 28) {
    fanSpeed = 0; // Turn off fan
  } else if (currentTemp > 28 && currentTemp <= 32) {
    fanSpeed = map(currentTemp, 30, 60, 128, 255); // Gradual increase
  } else {
    fanSpeed = 255; // Full speed
  }

  // Apply PWM to fan
  analogWrite(FAN_PWM_PIN, fanSpeed);

  // Debug output
  Serial.print("Temperature: ");
  Serial.print(currentTemp);
  Serial.print(" °C, Fan Speed: ");
  Serial.println(map(fanSpeed, 0, 255, 0, 100)); // Display fan speed as percentage

  // Send data to WebSocket server in JSON format
  sendToWebSocket(currentTemp, fanSpeed);

  delay(1000); // Update every second
}

// Function to handle received messages
void handleServerMessage(const char* message) {
  StaticJsonDocument<128> jsonDoc;

  // Deserialize JSON
  DeserializationError error = deserializeJson(jsonDoc, message);
  if (error) {
    Serial.println("Error parsing JSON message");
    return;
  }

  // Example: Process fan speed command
  if (jsonDoc.containsKey("setFanSpeed")) {
    int newSpeed = jsonDoc["setFanSpeed"];
    Serial.print("Setting new fan speed to: ");
    Serial.println(newSpeed);

    // Update fan speed based on the server command
    analogWrite(FAN_PWM_PIN, map(newSpeed, 0, 100, 0, 255));
  }
}

// Function to send JSON data to WebSocket server
void sendToWebSocket(float temp, int fanSpeed) {
  // Check if client is connected
  if (!client.available()) {
    Serial.println("WebSocket client not available, attempting to reconnect...");

    // Try reconnecting to the WebSocket server
    if (client.connect(websockets_server_host, websockets_server_port, "/ws")) {
      Serial.println("Reconnected to WebSocket server!");
      client.send("Reconnected: Hello Server");
    } else {
      Serial.println("Failed to reconnect to WebSocket server.");
      return; // Exit function if reconnect fails
    }
  }

  // Create JSON object
  StaticJsonDocument<128> jsonDoc;
  jsonDoc["type"] = "INFO";
  jsonDoc["temp"] = temp;
  jsonDoc["fanSpeed"] = map(fanSpeed, 0, 255, 0, 100); // Convert to percentage

  // Serialize JSON to string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Send JSON data to the WebSocket server
  client.send(jsonString);

  // Debug output
  Serial.print("Sent to WebSocket: ");
  Serial.println(jsonString);
}
