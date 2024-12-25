import React, { useState, useEffect, useRef } from "react";
import { Line } from "react-chartjs-2";
import {
  Chart as ChartJS,
  LineElement,
  CategoryScale,
  LinearScale,
  PointElement,
  Tooltip,
  Legend,
} from "chart.js";
import {
  Card,
  CardContent,
  Typography,
  Grid,
  Box,
  CircularProgress,
} from "@mui/material";
import {WEBSOCKET_URL} from "../Utils/constants";

// Register required Chart.js components
ChartJS.register(CategoryScale, LinearScale, PointElement, LineElement, Tooltip, Legend);

const TemperatureMonitor = () => {
  const [temperature, setTemperature] = useState(0.0);
  const [fanSpeed, setFanSpeed] = useState(0);
  const [tempHistory, setTempHistory] = useState([]);
  const [timeHistory, setTimeHistory] = useState([]);
  const socketRef = useRef(null);

  useEffect(() => {
    const socket = new WebSocket(WEBSOCKET_URL); // Update with your server's address
    socketRef.current = socket;

    socket.onopen = () => {
      console.log("WebSocket connection established.");
    };

    socket.onmessage = (event) => {
      console.log("Received from server:", event.data);
      const data = JSON.parse(event.data);

      if (data.temp !== undefined && data.fanSpeed !== undefined) {
        setTemperature(parseFloat(data.temp.toFixed(1))); // Ensure one decimal place
        setFanSpeed(data.fanSpeed);

        // Update history for graph
        setTempHistory((prev) => [...prev.slice(-19), parseFloat(data.temp.toFixed(1))]); // Keep the last 20 data points
        setTimeHistory((prev) => [
          ...prev.slice(-19),
          new Date().toLocaleTimeString([], { hour: "2-digit", minute: "2-digit" }), // Custom time format
        ]);
      }
    };

    socket.onclose = () => {
      console.log("WebSocket connection closed.");
    };

    return () => {
      socket.close();
    };
  }, []);

  const graphData = {
    labels: timeHistory,
    datasets: [
      {
        label: "Temperature (°C)",
        data: tempHistory,
        borderColor: "#3f51b5",
        borderWidth: 2,
        tension: 0.4,
        fill: false,
      },
    ],
  };

  const graphOptions = {
    responsive: true,
    plugins: {
      legend: {
        display: true,
      },
    },
    scales: {
      x: {
        title: {
          display: true,
          text: "Time",
        },
      },
      y: {
        title: {
          display: true,
          text: "Temperature (°C)",
        },
        suggestedMin: 0,
        suggestedMax: 100,
      },
    },
  };

  return (
    <Box sx={{ padding: "20px", display: "flex", flexDirection: "column", alignItems: "flex-start" }}>
      <Typography variant="h5" gutterBottom>
        Fan Monitor
      </Typography>

      <Grid container spacing={2} alignItems="flex-start">
        <Grid item xs={12} sm={6}>
          <Card sx={{ boxShadow: 3 }}>
            <CardContent>
              <Typography variant="h6" color="textSecondary">
                Current Temperature
              </Typography>
              <Typography variant="h4" color="primary">
                {temperature !== null ? `${temperature} °C` : <CircularProgress />}
              </Typography>
            </CardContent>
          </Card>
        </Grid>

        <Grid item xs={12} sm={6}>
          <Card sx={{ boxShadow: 3 }}>
            <CardContent>
              <Typography variant="h6" color="textSecondary">
                Current Fan Speed
              </Typography>
              <Typography variant="h4" color="primary">
                {fanSpeed !== null ? `${fanSpeed} %` : <CircularProgress />}
              </Typography>
            </CardContent>
          </Card>
        </Grid>
      </Grid>

      <Box
        sx={{
          marginTop: "30px",
          padding: "20px",
          boxShadow: 3,
          borderRadius: 2,
          alignSelf: "stretch",
        }}
      >
        <Line data={graphData} options={graphOptions} />
      </Box>
    </Box>
  );
};

export default TemperatureMonitor;
