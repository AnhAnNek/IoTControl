import React, { useState } from "react";
import { Typography, Box, Button, TextField, Paper } from "@mui/material";
import { useWebSocket } from "../Providers/WebSocketProvider";

const FanControl = () => {
  const [messageToSend, setMessageToSend] = useState("");
  const { sendMessage, receivedMessages } = useWebSocket();

  const handleSendMessage = () => {
    if (messageToSend) {
      sendMessage(messageToSend);
      setMessageToSend(""); // Clear input field
    }
  };

  return (
    <Box sx={{ padding: "20px", maxWidth: "900px", margin: "0 auto" }}>
      <Typography variant="h4" component="h1" gutterBottom align="center">
        Fan Control Dashboard
      </Typography>

      <Box
        sx={{
          marginTop: "20px",
          padding: "20px",
          boxShadow: 3,
          borderRadius: 2,
          display: "flex",
          flexDirection: "column",
          gap: "16px",
        }}
      >
        <Typography variant="h6">Send Data to WebSocket Server</Typography>
        <TextField
          fullWidth
          variant="outlined"
          label="Message"
          value={messageToSend}
          onChange={(e) => setMessageToSend(e.target.value)}
        />
        <Button variant="contained" color="primary" onClick={handleSendMessage}>
          Send
        </Button>

        <Typography variant="h6" sx={{ marginTop: "20px" }}>
          Terminal (Received Messages)
        </Typography>
        <Paper
          sx={{
            padding: "10px",
            backgroundColor: "#000",
            color: "#0f0",
            borderRadius: "4px",
            minHeight: "200px",
            maxHeight: "400px",
            overflowY: "auto",
            fontFamily: "monospace",
          }}
        >
          {receivedMessages.length > 0 ? (
            receivedMessages.map((msg, index) => (
              <div key={index}>{msg}</div>
            ))
          ) : (
            <div>No messages yet.</div>
          )}
        </Paper>
      </Box>
    </Box>
  );
};

export default FanControl;
