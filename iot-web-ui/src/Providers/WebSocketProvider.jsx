import React, { createContext, useContext, useEffect, useRef, useState } from "react";
import { WEBSOCKET_URL } from "../Utils/constants";

const WebSocketContext = createContext();

export const WebSocketProvider = ({ children }) => {
  const [receivedMessages, setReceivedMessages] = useState([]);
  const socketRef = useRef(null);

  useEffect(() => {
    const socket = new WebSocket(WEBSOCKET_URL);
    socketRef.current = socket;

    socket.onopen = () => {
      console.log("WebSocket connection established.");
      addReceivedMessage("WebSocket connection established.");
    };

    socket.onmessage = (event) => {
      console.log("Received from server:", event.data);
      addReceivedMessage(event.data);
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
    setReceivedMessages((prev) => [...prev, message]);
  };

  const sendMessage = (message) => {
    if (socketRef.current && socketRef.current.readyState === WebSocket.OPEN) {
      socketRef.current.send(message);
      console.log("Sent to server:", message);
    } else {
      console.error("WebSocket is not open.");
    }
  };

  return (
    <WebSocketContext.Provider value={{ sendMessage, receivedMessages }}>
      {children}
    </WebSocketContext.Provider>
  );
};

export const useWebSocket = () => useContext(WebSocketContext);
