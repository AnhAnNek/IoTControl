package com.example.iot_web_server.ws;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;

import java.util.HashMap;
import java.util.Map;

@RestController
@RequestMapping("/ws")
public class WSController extends TextWebSocketHandler {

    private Map<WebSocketSession, String> clients = new HashMap<>();

    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        clients.put(session, "Connected");
        System.out.println("Client connected: " + session.getId());
    }

    @Override
    public void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        System.out.println("Received: " + message.getPayload());

        // Parse and process JSON payload
        try {
            if (message.getPayload().startsWith("{")) {
                String receivedMsg = message.getPayload();
                Map<String, Object> data = new ObjectMapper().readValue(receivedMsg, HashMap.class);
                System.out.println("Parsed Data: " + data);

                // Example: Adjust fan speed dynamically based on input
                String type = data.get("type").toString();

                if (type.equals("INFO")) {
                    broadcastToAll(receivedMsg);
                }
            }
        } catch (Exception e) {
            System.err.println("Error processing message: " + e.getMessage());
            session.sendMessage(new TextMessage("ERROR: Invalid message format"));
        }
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        clients.remove(session);
        System.out.println("Client disconnected: " + session.getId());
    }

    private void broadcastToAll(String message) {
        synchronized (clients) {
            for (WebSocketSession session : clients.keySet()) {
                if (session.isOpen()) {
                    try {
                        session.sendMessage(new TextMessage(message));
                    } catch (Exception e) {
                        System.err.println("Error broadcasting message: " + e.getMessage());
                    }
                }
            }
        }
    }

}
