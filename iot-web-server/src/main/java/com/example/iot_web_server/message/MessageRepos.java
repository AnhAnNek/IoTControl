package com.example.iot_web_server.message;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.UUID;

@Repository
public interface MessageRepos extends JpaRepository<Message, UUID> {
}
