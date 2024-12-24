package com.example.iot_web_server.message;

import jakarta.persistence.*;
import lombok.*;
import lombok.experimental.FieldDefaults;

import java.time.LocalDateTime;
import java.util.UUID;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Entity
@Table(name = "messages")
@Builder
@FieldDefaults(level = AccessLevel.PRIVATE)
public class Message {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    UUID id;

    @Column(nullable = false)
    String type;

    @Column(columnDefinition = "TEXT")
    String payload;

    @Column(nullable = false)
    LocalDateTime timestamp;

    @PrePersist
    @PreUpdate
    public void prePersist() {
        if (timestamp == null) {
            timestamp = LocalDateTime.now();
        }
    }
}
