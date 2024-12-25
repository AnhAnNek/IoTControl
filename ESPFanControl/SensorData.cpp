#include "SensorData.h"
#include <ArduinoJson.h>

// Convert SensorData object to JSON format
const char* SensorData::toJSON() {
    // Create a JSON document
    StaticJsonDocument<256> doc;

    // Add properties
    doc["type"] = type;
    doc["temp"] = temp;
    doc["envTemp"] = envTemp;
    doc["envHumidity"] = envHumidity;

    // Allocate memory for the JSON string
    size_t jsonSize = measureJson(doc) + 1; // +1 for null terminator
    char* jsonBuffer = new char[jsonSize];

    // Serialize JSON into the allocated buffer
    serializeJson(doc, jsonBuffer, jsonSize);

    // Return the dynamically allocated JSON string
    return jsonBuffer;
}
