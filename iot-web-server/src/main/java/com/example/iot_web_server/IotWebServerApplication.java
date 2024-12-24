package com.example.iot_web_server;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableAsync;

@SpringBootApplication
@EnableAsync
public class IotWebServerApplication {

	public static void main(String[] args) {
		SpringApplication.run(IotWebServerApplication.class, args);
	}

}
