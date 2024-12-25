package com.example.iot_web_server.ws;

import com.example.iot_web_server.message.service.MessageService;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
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

    private static final Logger log = LogManager.getLogger(WSController.class);
    private Map<WebSocketSession, String> clients = new HashMap<>();

    @Autowired
    private MessageService messageService;

    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        clients.put(session, "Connected");
        log.info("Client connected: " + session.getId());
    }

    @Override
    public void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        log.info("Received: " + message.getPayload());

        // Parse and process JSON payload
        try {
            if (message.getPayload().startsWith("{")) {
                String receivedMsg = message.getPayload();
                Map<String, Object> data = new ObjectMapper().readValue(receivedMsg, HashMap.class);
                log.info("Parsed Data: " + data);

                // Example: Adjust fan speed dynamically based on input
                String type = data.get("type").toString();

                if (type.equals("INFO")) {
                    // Save asynchronously
                    messageService.saveMessageAsync(type, receivedMsg);

                    broadcastToAll(receivedMsg);
                }

                if (type.equals("INFO_RELAYS")) {
                    broadcastToAll(receivedMsg);
                }

                if (type.equals("COMMAND_TURN_ON_RELAY")) {
                    broadcastToAll(receivedMsg);
                }

                if (type.equals("COMMAND_TURN_OFF_RELAY")) {
                    broadcastToAll(receivedMsg);
                }

                if (type.equals("COMMAND_SET_RELAY_TEMP")) {
                    broadcastToAll(receivedMsg);
                }
            }
        } catch (Exception e) {
            log.error("Error processing message: ", e);
            session.sendMessage(new TextMessage("ERROR: Invalid message format"));
        }
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        clients.remove(session);
        log.info("Client disconnected: " + session.getId());
    }

    private void broadcastToAll(String message) {
        synchronized (clients) {
            for (WebSocketSession session : clients.keySet()) {
                if (session.isOpen()) {
                    try {
                        session.sendMessage(new TextMessage(message));
                    } catch (Exception e) {
                        log.error("Error broadcasting message: ", e);
                    }
                }
            }
        }
    }

}
