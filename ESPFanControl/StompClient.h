#ifndef STOMPCLIENT_H
#define STOMPCLIENT_H

#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <map>

using namespace websockets;

class StompClient {
private:
    WebsocketsClient client;
    String serverUrl;
    std::map<String, void (*)(const String&)> destinationCallbacks;
    int subscriptionCounter = 0; // Counter for auto-generating subscription IDs

    void onMessageCallback(WebsocketsMessage message);

public:
    StompClient(const char* serverUrl);

    bool connectToServer();
    void subscribe(const char* destination, void (*callback)(const String&));
    void unsubscribe(const char* destination);
    void disconnect();
    void loop();
    void setMessageHandler();
};

#endif // STOMPCLIENT_H
#ifndef STOMPCLIENT_H
#define STOMPCLIENT_H

#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <map>

using namespace websockets;

class StompClient {
private:
    WebsocketsClient client;
    String serverUrl;
    std::map<String, void (*)(const String&)> destinationCallbacks;
    int subscriptionCounter = 0; // Counter for auto-generating subscription IDs

    void onMessageCallback(WebsocketsMessage message);

public:
    StompClient(const char* serverUrl);

    bool connectToServer();
    void subscribe(const char* destination, void (*callback)(const String&));
    void unsubscribe(const char* destination);
    void disconnect();
    void loop();
    void setMessageHandler();
};

#endif // STOMPCLIENT_H
