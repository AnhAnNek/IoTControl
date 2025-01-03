import React, { useState, useEffect, useRef } from "react";
import { Typography, Box, Button, TextField, Paper, FormControlLabel, Switch } from "@mui/material";
import CodeEditor from "@uiw/react-textarea-code-editor"; // Import the Code Editor
import { useWebSocket } from "../Providers/WebSocketProvider";

const ESPControl = () => {
  const [messageToSend, setMessageToSend] = useState("");
  const [searchQuery, setSearchQuery] = useState("");
  const [isJsonFormat, setIsJsonFormat] = useState(false); // Toggle for JSON/plain text
  const { sendMessage, receivedMessages } = useWebSocket();
  const terminalRef = useRef(null);

  const handleSendMessage = () => {
    if (messageToSend) {
      let message = messageToSend;
      if (isJsonFormat) {
        try {
          message = JSON.stringify(JSON.parse(messageToSend));
        } catch (error) {
          alert("Invalid JSON format. Please correct it.");
          return;
        }
      }
      sendMessage(message);
      setMessageToSend(""); // Clear input field
    }
  };

  const handleFormatJson = () => {
    if (!isJsonFormat) return; // Only format if JSON mode is enabled
    try {
      const formattedJson = JSON.stringify(JSON.parse(messageToSend), null, 2);
      setMessageToSend(formattedJson);
    } catch (error) {
      alert("Invalid JSON format. Please correct it before formatting.");
    }
  };

  // Auto-scroll effect when new messages are added
  useEffect(() => {
    if (terminalRef.current) {
      const terminal = terminalRef.current;
      const isAtBottom = terminal.scrollHeight - terminal.scrollTop === terminal.clientHeight;

      if (isAtBottom) {
        terminal.scrollTop = terminal.scrollHeight;
      }
    }
  }, [receivedMessages]);

  const highlightText = (text, query) => {
    if (!query) return text;
    const regex = new RegExp(`(${query})`, "gi");
    const parts = text.split(regex);
    return parts.map((part, index) =>
      regex.test(part) ? (
        <span key={index} style={{ backgroundColor: "yellow", color: "black" }}>
          {part}
        </span>
      ) : (
        part
      )
    );
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
        <FormControlLabel
          control={
            <Switch
              checked={isJsonFormat}
              onChange={(e) => setIsJsonFormat(e.target.checked)}
            />
          }
          label="Send as JSON"
        />
        <CodeEditor
          value={messageToSend}
          language={isJsonFormat ? "json" : "plaintext"}
          placeholder={isJsonFormat ? '{"key": "value"}' : "Enter plain text message"}
          onChange={(e) => setMessageToSend(e.target.value)}
          padding={10}
          style={{
            fontSize: 14,
            backgroundColor: "#f5f5f5",
            fontFamily: "monospace",
            borderRadius: "4px",
            border: "1px solid #ccc",
          }}
        />
        <Box sx={{ display: "flex", gap: "10px" }}>
          <Button variant="contained" color="primary" onClick={handleSendMessage}>
            Send
          </Button>
          {isJsonFormat && (
            <Button variant="outlined" color="secondary" onClick={handleFormatJson}>
              Format JSON
            </Button>
          )}
        </Box>

        <Typography variant="h6" sx={{ marginTop: "20px" }}>
          Terminal (Received Messages)
        </Typography>
        <TextField
          fullWidth
          variant="outlined"
          label="Search Terminal"
          value={searchQuery}
          onChange={(e) => setSearchQuery(e.target.value)}
          sx={{ marginBottom: "10px" }}
        />
        <Paper
          ref={terminalRef} // Attach the ref to the terminal container
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
              <div key={index}>{highlightText(msg, searchQuery)}</div>
            ))
          ) : (
            <div>No messages yet.</div>
          )}
        </Paper>
      </Box>
    </Box>
  );
};

export default ESPControl;
