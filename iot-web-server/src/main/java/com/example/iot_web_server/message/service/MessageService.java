package com.example.iot_web_server.message.service;

import com.example.iot_web_server.message.Message;

import java.util.concurrent.CompletableFuture;

public interface MessageService {
    CompletableFuture<Message> saveMessageAsync(String type, String payload);
}
