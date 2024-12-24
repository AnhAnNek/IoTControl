package com.example.iot_web_server.async;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.context.annotation.Configuration;
import org.springframework.scheduling.annotation.AsyncConfigurer;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;

import java.util.concurrent.Executor;

@Configuration
public class AsyncConfig implements AsyncConfigurer {

    private final Logger log = LogManager.getLogger(AsyncConfig.class);

    @Override
    public Executor getAsyncExecutor() {
        int defaultCores = 4; // Default number of cores if unavailable
        int processors;

        try {
            processors = Runtime.getRuntime().availableProcessors();
            if (processors <= 0) {
                throw new IllegalStateException("Unable to detect processor count.");
            }
        } catch (Exception e) {
            log.warn("Failed to get the number of available processors. Falling back to default value.", e);
            processors = defaultCores;
        }

        int corePoolSize = processors; // Use detected or default value
        int maxPoolSize = processors * 2; // Set max pool size as double the core size
        int queueCapacity = 100; // Queue capacity for tasks

        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(corePoolSize);
        executor.setMaxPoolSize(maxPoolSize);
        executor.setQueueCapacity(queueCapacity);
        executor.setThreadNamePrefix("AsyncExecutor-");
        executor.initialize();

        // Log thread pool configuration
        log.info("Async Executor Configured:");
        log.info(" - Core Pool Size: {}", corePoolSize);
        log.info(" - Max Pool Size: {}", maxPoolSize);
        log.info(" - Queue Capacity: {}", queueCapacity);

        return executor;
    }
}
