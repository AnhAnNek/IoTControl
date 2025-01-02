package com.example.iot_web_server.message;

import com.example.iot_web_server.util.Utils;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api/messages")
public class MessageController {

    @GetMapping("/get-shared-values")
    public String getSharedValues() {
        return Utils.SHARED_INFO;
    }

    @PutMapping("/update-shared-values")
    public ResponseEntity<String> updateSharedValues(String sharedValues) {
        Utils.SHARED_INFO = sharedValues;
        return ResponseEntity.ok(Utils.SHARED_INFO);
    }
}
