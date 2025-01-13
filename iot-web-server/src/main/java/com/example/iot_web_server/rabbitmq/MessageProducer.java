package com.example.iot_web_server.rabbitmq;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class MessageProducer {

    private static final Logger log = LogManager.getLogger(MessageProducer.class);

    @Autowired
    private RabbitTemplate rabbitTemplate;

    public void sendMessage(String message) {
        rabbitTemplate.convertAndSend("exampleExchange", "exampleRoutingKey", message);
        log.info("Message sent: " + message);
    }
}
