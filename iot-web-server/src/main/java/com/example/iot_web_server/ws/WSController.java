package com.example.iot_web_server.ws;

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

        // Xử lý dữ liệu từ ESP32
        if (message.getPayload().startsWith("{")) {
            System.out.println("Data: " + message.getPayload());
        }

        // Gửi lệnh thay đổi tốc độ quạt
        if (message.getPayload().contains("SET_SPEED")) {
            session.sendMessage(new TextMessage("SET_SPEED:128"));
        }
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        clients.remove(session);
        System.out.println("Client disconnected: " + session.getId());
    }
}
