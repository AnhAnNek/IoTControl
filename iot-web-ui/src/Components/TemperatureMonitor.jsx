import React, { useState, useEffect, useRef } from "react";
import { Line } from "react-chartjs-2";
import {
  Chart as ChartJS,
  LineElement,
  CategoryScale,
  LinearScale,
  PointElement,
  Legend,
} from "chart.js";
import {
  Card,
  CardContent,
  Typography,
  Grid,
  Box,
  Button,
  CircularProgress,
  Tooltip,
  TextField,
} from "@mui/material";
import { WEBSOCKET_URL } from "../Utils/constants";

// Register required Chart.js components
ChartJS.register(CategoryScale, LinearScale, PointElement, LineElement, Legend);

const TemperatureMonitor = () => {
  const [temperature, setTemperature] = useState(0.0);
  const [envTemp, setEnvTemp] = useState(0);
  const [envHumidity, setEnvHumidity] = useState(0);
  const [tempHistory, setTempHistory] = useState([]);
  const [timeHistory, setTimeHistory] = useState([]);
  const socketRef = useRef(null);

  // Relay states
  const [relays, setRelays] = useState([
    { status: false, tempToRun: 0, tempToStop: 0, isAutoMode: true },
    { status: false, tempToRun: 0, tempToStop: 0, isAutoMode: true },
    { status: false, tempToRun: 0, tempToStop: 0, isAutoMode: true },
    { status: false, tempToRun: 0, tempToStop: 0, isAutoMode: true },
  ]);

  const [settingTemps, setSettingTemps] = useState([
    { tempToRun: 0, tempToStop: 0 },
    { tempToRun: 0, tempToStop: 0 },
    { tempToRun: 0, tempToStop: 0 },
    { tempToRun: 0, tempToStop: 0 },
  ]);

  useEffect(() => {
    const socket = new WebSocket(WEBSOCKET_URL); // Update with your server's address
    socketRef.current = socket;

    socket.onopen = () => {
      console.log("WebSocket connection established.");
    };

    socket.onmessage = (event) => {
      console.log("Received from server:", event.data);
      const data = JSON.parse(event.data);

      if (data.temp !== undefined) {
        setTemperature(parseFloat(data.temp.toFixed(1))); // Ensure one decimal place

        // Update history for graph
        setTempHistory((prev) => [
          ...prev.slice(-19),
          parseFloat(data.temp.toFixed(1)),
        ]); // Keep the last 20 data points
        setTimeHistory((prev) => [
          ...prev.slice(-19),
          new Date().toLocaleTimeString([], {
            hour: "2-digit",
            minute: "2-digit",
            second: "2-digit",
            hour12: false,
          }),
        ]);
      }

      // Update environmental data
      if (data.envTemp !== undefined) setEnvTemp(data.envTemp);
      if (data.envHumidity !== undefined) setEnvHumidity(data.envHumidity);

      const returnedRelays = data.relays;
      if (data.type === "INFO_RELAYS" && Array.isArray(returnedRelays)) {
        setRelays(returnedRelays);
      }
    };

    socket.onclose = () => {
      console.log("WebSocket connection closed.");
    };

    return () => {
      socket.close();
    };
  }, []);

  const handleRelayToggle = (index) => {
    const relayName = `relay${index + 1}`;
    const updatedRelays = [...relays];
    updatedRelays[index].status = !updatedRelays[index].status;
    setRelays(updatedRelays);

    // Send updated relay control to the server
    if (socketRef.current) {
      socketRef.current.send(
        JSON.stringify({
          type: updatedRelays[index].status
            ? "COMMAND_TURN_ON_RELAY"
            : "COMMAND_TURN_OFF_RELAY",
          relayName,
        })
      );
    }
  };

  const handleSettingTempChange = (index, field, value) => {
    const updatedRelays = [...settingTemps];
    updatedRelays[index][field] = parseFloat(value) || 0;
    setSettingTemps(updatedRelays);
  };

  const handleTempSet = (index) => {
    const relayName = `relay${index + 1}`;
    const relay = settingTemps[index];

    // Send updated relay configuration to the server
    if (socketRef.current) {
      socketRef.current.send(
        JSON.stringify({
          type: "COMMAND_SET_RELAY_TEMP",
          relayName,
          tempToRun: relay.tempToRun,
          tempToStop: relay.tempToStop,
        })
      );
    }
  };

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

  const minTemp =
    tempHistory.length > 0 ? Math.round(Math.min(...tempHistory) - 1) : 0;
  const maxTemp =
    tempHistory.length > 0 ? Math.round(Math.max(...tempHistory) + 1) : 10;
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
        suggestedMin: minTemp,
        suggestedMax: maxTemp,
      },
    },
  };

  return (
    <Box
      sx={{
        padding: "20px",
        display: "flex",
        flexDirection: "column",
        alignItems: "flex-start",
      }}
    >
      <Typography variant="h5" gutterBottom>
        Temperature Monitor
      </Typography>

      <Grid container spacing={2} alignItems="flex-start">
        {/* Card 1: Current Temperature */}
        <Grid item xs={12} sm={6} md={3}>
          <Tooltip
            title="This is the temperature of the target device. Use this to control the relays."
            arrow
          >
            <Card sx={{ boxShadow: 3 }}>
              <CardContent>
                <Typography
                  variant="h6"
                  color="textSecondary"
                  sx={{
                    whiteSpace: "nowrap",
                    overflow: "hidden",
                    textOverflow: "ellipsis",
                  }}
                >
                  Target Temperature
                </Typography>
                <Typography variant="h4" color="primary">
                  {temperature !== null ? (
                    `${temperature} °C`
                  ) : (
                    <CircularProgress />
                  )}
                </Typography>
              </CardContent>
            </Card>
          </Tooltip>
        </Grid>

        {/* Card 2: Ambient Temperature */}
        <Grid item xs={12} sm={6} md={3}>
          <Card sx={{ boxShadow: 3 }}>
            <CardContent>
              <Typography
                variant="h6"
                color="textSecondary"
                sx={{
                  whiteSpace: "nowrap",
                  overflow: "hidden",
                  textOverflow: "ellipsis",
                }}
              >
                Ambient Temperature
              </Typography>
              <Typography variant="h4" color="primary">
                {envTemp !== null ? `${envTemp} °C` : <CircularProgress />}
              </Typography>
            </CardContent>
          </Card>
        </Grid>

        {/* Card 3: Humidity */}
        <Grid item xs={12} sm={6} md={3}>
          <Card sx={{ boxShadow: 3 }}>
            <CardContent>
              <Typography
                variant="h6"
                color="textSecondary"
                sx={{
                  whiteSpace: "nowrap",
                  overflow: "hidden",
                  textOverflow: "ellipsis",
                }}
              >
                Ambient Humidity
              </Typography>
              <Typography variant="h4" color="primary">
                {envHumidity !== null ? (
                  `${envHumidity}%`
                ) : (
                  <CircularProgress />
                )}
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

      <Box sx={{ marginTop: "30px", width: "100%" }}>
        <Typography variant="h6" gutterBottom bold>
          Relay Controls
        </Typography>
        <Grid container spacing={2}>
          {relays.map((relay, index) => (
            <Grid item xs={12} sm={6} md={3} key={index}>
              <Card sx={{ boxShadow: 3 }}>
                <CardContent>
                  <Typography variant="h5" color="textSecondary">
                    <strong>Relay {index + 1}</strong>
                  </Typography>
                  <Typography variant="body1" color="textSecondary" mt={1}>
                    <strong>Auto Mode:</strong>{" "}
                    {relay?.isAutoMode ? "ON" : "OFF"}
                  </Typography>
                  <Typography variant="body1" color="textSecondary">
                    <strong>Status:</strong> {relay.status ? "ON" : "OFF"}
                  </Typography>
                  <Box mb={2} mt={1}>
                    <Button
                      variant="contained"
                      color={relay.status ? "error" : "primary"}
                      onClick={() => handleRelayToggle(index)}
                      sx={{ marginRight: "10px" }}
                    >
                      {relay.status ? "Turn OFF" : "Turn ON"}
                    </Button>
                  </Box>
                  <Typography variant="body1" color="textSecondary">
                    <strong>Temp to Run:</strong> {relay.tempToRun || ""}°C
                  </Typography>
                  <Typography variant="body1" color="textSecondary">
                    <strong>Temp to Stop:</strong> {relay.tempToStop || ""}°C
                  </Typography>
                  <Box sx={{ marginTop: "10px" }}>
                    <TextField
                      label="Temp to Run (°C)"
                      type="number"
                      value={settingTemps[index]?.tempToRun || ""}
                      onChange={(e) =>
                        handleSettingTempChange(
                          index,
                          "tempToRun",
                          e.target.value
                        )
                      }
                      fullWidth
                      margin="dense"
                    />
                    <TextField
                      label="Temp to Stop (°C)"
                      type="number"
                      value={settingTemps[index]?.tempToStop || ""}
                      onChange={(e) =>
                        handleSettingTempChange(
                          index,
                          "tempToStop",
                          e.target.value
                        )
                      }
                      fullWidth
                      margin="dense"
                    />
                    <Button
                      variant="outlined"
                      color="secondary"
                      sx={{ marginTop: "10px" }}
                      onClick={() => handleTempSet(index)}
                      fullWidth
                    >
                      Set Temperatures
                    </Button>
                  </Box>
                </CardContent>
              </Card>
            </Grid>
          ))}
        </Grid>
      </Box>
    </Box>
  );
};

export default TemperatureMonitor;
