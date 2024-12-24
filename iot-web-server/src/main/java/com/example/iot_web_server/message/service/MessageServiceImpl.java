package com.example.iot_web_server.message.service;

import com.example.iot_web_server.message.Message;
import com.example.iot_web_server.message.MessageRepos;
import lombok.RequiredArgsConstructor;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.concurrent.CompletableFuture;

@Service
@RequiredArgsConstructor
public class MessageServiceImpl implements MessageService {

    private final Logger log = LogManager.getLogger(MessageServiceImpl.class);

    private final MessageRepos messageRepos;

    @Async
    public CompletableFuture<Message> saveMessageAsync(String type, String payload) {
        try {
            Message message = Message.builder()
                    .type(type)
                    .payload(payload)
                    .timestamp(LocalDateTime.now())
                    .build();
            Message savedMessage = messageRepos.save(message);
            log.info("Message saved to database.");
            return CompletableFuture.completedFuture(savedMessage);
        } catch (Exception e) {
            log.error("Error saving message: ", e);
            return null;
        }
    }
}
