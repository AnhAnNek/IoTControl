#include "StompClient.h"

StompClient::StompClient(const char* serverUrl) : serverUrl(serverUrl) {}

bool StompClient::connectToServer() {
    Serial.println("Connecting to WebSocket server...");
    if (client.connect(serverUrl)) {
        Serial.println("Connected to WebSocket server");

        // Send STOMP CONNECT frame
        client.send("CONNECT\naccept-version:1.2\n\n\0");
        Serial.println("STOMP CONNECT sent");
        return true;
    } else {
        Serial.println("WebSocket connection failed");
        return false;
    }
}

void StompClient::subscribe(const char* destination, void (*callback)(const String&)) {
    String destStr = String(destination);
    String subscriptionId = "sub-" + String(subscriptionCounter++); // Auto-generate subscription ID

    destinationCallbacks[destStr] = callback;

    String subscribeFrame = "SUBSCRIBE\nid:" + subscriptionId + "\ndestination:" + destStr + "\n\n\0";
    client.send(subscribeFrame);
    Serial.println("Subscribed to " + destStr + " with ID: " + subscriptionId);
}

void StompClient::unsubscribe(const char* destination) {
    String destStr = String(destination);

    auto it = destinationCallbacks.find(destStr);
    if (it != destinationCallbacks.end()) {
        String subscriptionId = "sub-" + String(std::distance(destinationCallbacks.begin(), it));

        String unsubscribeFrame = "UNSUBSCRIBE\nid:" + subscriptionId + "\n\n\0";
        client.send(unsubscribeFrame);
        destinationCallbacks.erase(it);
        Serial.println("Unsubscribed from " + destStr);
    } else {
        Serial.println("No subscription found for " + destStr);
    }
}

void StompClient::disconnect() {
    if (client.available()) {
        // Send STOMP DISCONNECT frame
        client.send("DISCONNECT\n\n\0");
        client.close();
        Serial.println("Disconnected from WebSocket server");
    } else {
        Serial.println("No active WebSocket connection to disconnect.");
    }
}

void StompClient::loop() {
    if (client.available()) {
        client.poll();  // Poll for messages
    }
}

void StompClient::onMessageCallback(WebsocketsMessage message) {
    String data = message.data();
    Serial.println("Received: " + data);

    // Extract the destination (assuming it's in the message header, adjust if needed)
    int destStart = data.indexOf("destination:") + 12;
    int destEnd = data.indexOf("\n", destStart);
    String destination = data.substring(destStart, destEnd);

    // Pass the message to the registered callback for this destination
    if (destinationCallbacks.find(destination) != destinationCallbacks.end()) {
        String body = data.substring(data.indexOf("\n\n") + 2); // Message body
        destinationCallbacks[destination](body);
    } else {
        Serial.println("No callback registered for destination: " + destination);
    }
}

void StompClient::setMessageHandler() {
    client.onMessage([this](WebsocketsMessage message) { this->onMessageCallback(message); });
}
