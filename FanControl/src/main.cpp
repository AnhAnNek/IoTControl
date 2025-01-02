#include <Arduino.h>
#include "SensorManager.h"
#include "WebSocketManager.h"
#include "WiFiManager.h"
#include "RelayControl.h"
#include "SensorData.h"
#include "Constants.h"
#include "DeviceHealth.h"

String deviceName;
DeviceHealth deviceHealth;

SensorManager sensorManager(TEMP_SENSOR_PIN, DHT_PIN);

RelayControl relay1(RELAY1_PIN, "relay1", LOW);
RelayControl relay2(RELAY2_PIN, "relay2", LOW);
RelayControl relay3(RELAY3_PIN, "relay3", LOW);
RelayControl relay4(RELAY4_PIN, "relay4", LOW);


const int pwmPin = 16;  // GPIO để kết nối với dây vàng của quạt
unsigned long pulseTime = 0;
float frequency = 0.0;
float rpm = 0.0;
const int pulsesPerRevolution = 1; // Giả sử mỗi pulse tương đương 1 vòng quay


void customMessageHandler(const char* message);
void initRelays();
void updateRelays(float currentTemp);
String getDeviceName();

// Function to initialize the system
void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi
  WiFiManager& wifiManager = WiFiManager::getInstance();
  wifiManager.initializeWiFi(WIFI_SSID, WIFI_PASSWORD);

  // Initialize WebSocketManager
  WebSocketManager& webSocketManager = WebSocketManager::getInstance();
  webSocketManager.initialize(WEBSOCKET_SERVER_HOST, WEBSOCKET_SERVER_PORT);
  webSocketManager.setOnReceivedMessageCallback(customMessageHandler);

  // Get the health status of devices
  String jsonDeviceStatus = deviceHealth.checkHealth();
  WebSocketManager::getInstance().send(jsonDeviceStatus.c_str());
  Serial.println(jsonDeviceStatus);
  
  // Initialize sensors
  sensorManager.initializeSensors();

  initRelays();

  deviceName = getDeviceName();

  pinMode(LED_PIN, OUTPUT);

  pinMode(pwmPin, INPUT_PULLUP);
}

// Function to run the main system loop
void loop() {
  // Handle WebSocket messages
  WebSocketManager::getInstance().handleMessages();

  float currentTemp = sensorManager.getTemperature();
  float envTemp = sensorManager.getEnvTemperature();
  float envHumidity = sensorManager.getEnvHumidity();

  if (currentTemp == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature!");
    return;
  }

  updateRelays(currentTemp);
  digitalWrite(LED_PIN, HIGH);  
  SensorData sensor("INFO", currentTemp, envTemp, envHumidity);
  const char* jsonSensorData = sensor.toJSON();
  WebSocketManager::getInstance().send(jsonSensorData);
  digitalWrite(LED_PIN, LOW);
  delete[] jsonSensorData;


  pulseTime = pulseIn(pwmPin, HIGH, 1000000);
  
  if (pulseTime > 0) {
    // Tính tần số PWM (Hz) từ thời gian giữa các pulse
    frequency = 1000000.0 / pulseTime;
    
    // Tính RPM từ tần số
    rpm = (frequency / pulsesPerRevolution) * 60;
    
    // In ra tần số PWM và RPM
    Serial.print("Tần số PWM: ");
    Serial.print(frequency);
    Serial.print(" Hz, ");
    Serial.print("RPM: ");
    Serial.println(rpm);
  }


  delay(1000);
}

void customMessageHandler(const char* message) 
{
  Serial.println("Custom Handler: Received message from server:");
  Serial.println(message);

  // Parse JSON
  StaticJsonDocument<128> jsonDoc;
  if (deserializeJson(jsonDoc, message) == DeserializationError::Ok) {
    if (jsonDoc.containsKey("type")) {
      String type = jsonDoc["type"];
      if (type == COMMAND_TURN_ON_RELAY && jsonDoc.containsKey("relayName")) {
        String relayName = jsonDoc["relayName"];
        if (relayName == relay1.getName()) {
          relay1.turnOn(); 
        } else if (relayName == relay2.getName()) {
          relay2.turnOn(); 
        } else if (relayName == relay3.getName()) {
          relay3.turnOn(); 
        } else if (relayName == relay4.getName()) {
          relay4.turnOn(); 
        }
      }
      else if (type == COMMAND_TURN_OFF_RELAY && jsonDoc.containsKey("relayName")) {
        String relayName = jsonDoc["relayName"];
        if (relayName == relay1.getName()) {
          relay1.turnOff(); 
        } else if (relayName == relay2.getName()) {
          relay2.turnOff(); 
        } else if (relayName == relay3.getName()) {
          relay3.turnOff(); 
        } else if (relayName == relay4.getName()) {
          relay4.turnOff(); 
        }
      }
      else if (type == COMMAND_SET_RELAY_TEMP && jsonDoc.containsKey("relayName")) {
        String relayName = jsonDoc["relayName"];
        float tempToRun = jsonDoc["tempToRun"];
        float tempToStop = jsonDoc["tempToStop"];
        if (relayName == relay1.getName()) {
          relay1.setAutoMode(true, tempToRun, tempToStop);
        } else if (relayName == relay2.getName()) {
          relay2.setAutoMode(true, tempToRun, tempToStop);
        } else if (relayName == relay3.getName()) {
          relay3.setAutoMode(true, tempToRun, tempToStop);
        } else if (relayName == relay4.getName()) {
          relay4.setAutoMode(true, tempToRun, tempToStop);
        }
      }
    }
  }
}

void initRelays() 
{
  relay1.begin();
  relay2.begin();
  relay3.begin();
  relay4.begin();
  
  relay1.setAutoMode(true, 26, 22);
  relay2.setAutoMode(true, 28, 24);
  relay3.setAutoMode(true, 29, 26);
  relay4.setAutoMode(true, 30, 28);
  Serial.println("Relay initialized");
}

void updateRelays(float currentTemp)
{
  relay1.update(currentTemp);
  relay2.update(currentTemp);
  relay3.update(currentTemp);
  relay4.update(currentTemp);

  // Create a JSON document
  StaticJsonDocument<256> jsonDoc;

  // Set the type field
  jsonDoc["type"] = "INFO_RELAYS";
  jsonDoc["deviceName"] = deviceName;

  // Create an array for relays
  JsonArray relaysArray = jsonDoc.createNestedArray("relays");

  // Add relay states to the JSON array
  JsonObject relay1Json = relaysArray.createNestedObject();
  relay1Json["name"] = relay1.getName();
  relay1Json["status"] = relay1.getState();
  relay1Json["isAutoMode"] = relay1.isAutoMode();
  relay1Json["tempToRun"] = relay1.getTempToRun();
  relay1Json["tempToStop"] = relay1.getTempToStop();

  JsonObject relay2Json = relaysArray.createNestedObject();
  relay2Json["name"] = relay2.getName();
  relay2Json["status"] = relay2.getState();
  relay2Json["isAutoMode"] = relay2.isAutoMode();
  relay2Json["tempToRun"] = relay2.getTempToRun();
  relay2Json["tempToStop"] = relay2.getTempToStop();

  JsonObject relay3Json = relaysArray.createNestedObject();
  relay3Json["name"] = relay3.getName();
  relay3Json["status"] = relay3.getState();
  relay3Json["isAutoMode"] = relay3.isAutoMode();
  relay3Json["tempToRun"] = relay3.getTempToRun();
  relay3Json["tempToStop"] = relay3.getTempToStop();

  JsonObject relay4Json = relaysArray.createNestedObject();
  relay4Json["name"] = relay4.getName();
  relay4Json["status"] = relay4.getState();
  relay4Json["isAutoMode"] = relay4.isAutoMode();
  relay4Json["tempToRun"] = relay4.getTempToRun();
  relay4Json["tempToStop"] = relay4.getTempToStop();

  // Serialize JSON to a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Print the JSON for debugging
  Serial.println(jsonString);

  // Optionally, send the JSON string over WebSocket or any other interface
  WebSocketManager::getInstance().send(jsonString.c_str());
}

String getDeviceName() {
  // Get the MAC address
  String macAddress = WiFi.macAddress();

  // Extract the last 3 bytes of the MAC address for uniqueness
  String uniquePart = macAddress.substring(macAddress.lastIndexOf(':') - 2);

  // Combine a custom name with the unique part
  String deviceName = "ESP32-" + uniquePart;
  return deviceName;
}