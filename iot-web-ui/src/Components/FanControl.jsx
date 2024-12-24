import React, { useState, useEffect, useRef } from "react";
import {
  Typography,
  Box,
  Button,
  TextField,
  Paper,
} from "@mui/material";

const FanControl = () => {
  const [messageToSend, setMessageToSend] = useState(""); // Message input state
  const [receivedMessages, setReceivedMessages] = useState([]); // Array for received messages
  const socketRef = useRef(null);

  useEffect(() => {
    // Connect to WebSocket server
    const socket = new WebSocket("ws://127.0.0.1:8000/ws"); // Update with your server's address
    socketRef.current = socket;

    socket.onopen = () => {
      console.log("WebSocket connection established.");
      addReceivedMessage("WebSocket connection established.");
    };

    socket.onmessage = (event) => {
      console.log("Received from server:", event.data);
      addReceivedMessage(event.data); // Add received message to the array
    };

    socket.onclose = () => {
      console.log("WebSocket connection closed.");
      addReceivedMessage("WebSocket connection closed.");
    };

    return () => {
      socket.close();
    };
  }, []);

  const addReceivedMessage = (message) => {
    setReceivedMessages((prevMessages) => [...prevMessages, message]);
  };

  const sendMessage = () => {
    if (socketRef.current && messageToSend) {
      socketRef.current.send(messageToSend);
      setMessageToSend(""); // Clear input field
      console.log("Sent to server:", messageToSend);
    }
  };

  return (
    <Box sx={{ padding: "20px", maxWidth: "900px", margin: "0 auto" }}>
      <Typography variant="h4" component="h1" gutterBottom align="center">
        Fan Control Dashboard
      </Typography>

      {/* Message Input and Buttons */}
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
        <Button variant="contained" color="primary" onClick={sendMessage}>
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
