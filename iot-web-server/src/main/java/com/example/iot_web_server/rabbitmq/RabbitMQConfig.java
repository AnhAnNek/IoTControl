package com.example.iot_web_server.rabbitmq;

import org.springframework.amqp.core.*;
import org.springframework.amqp.rabbit.annotation.EnableRabbit;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
@EnableRabbit
public class RabbitMQConfig {

    @Bean
    public Queue exampleQueue() {
        return new Queue("exampleQueue", true);  // durable queue
    }

    @Bean
    public DirectExchange exchange() {
        return new DirectExchange("exampleExchange");
    }

    @Bean
    public Binding binding(Queue exampleQueue, DirectExchange exchange) {
        return BindingBuilder.bind(exampleQueue).to(exchange).with("exampleRoutingKey");
    }
}
