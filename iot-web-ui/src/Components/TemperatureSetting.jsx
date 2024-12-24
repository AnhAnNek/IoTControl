import React, { useState } from "react";
import { Box, TextField, Button, Typography, Grid } from "@mui/material";
import { useWebSocket } from "../Providers/WebSocketProvider";

const TemperatureSetting = () => {
  const [minTemp, setMinTemp] = useState(25);
  const [maxTemp, setMaxTemp] = useState(35);
  const [minFanSpeed, setMinFanSpeed] = useState(0);
  const [maxFanSpeed, setMaxFanSpeed] = useState(255);
  const [status, setStatus] = useState("");
  const { sendMessage } = useWebSocket();

  const handleSendSettings = () => {
    const settings = {
      type: "SETTING_TEMP",
      minTemp: parseFloat(minTemp),
      maxTemp: parseFloat(maxTemp),
      minFanSpeed: parseInt(minFanSpeed, 10),
      maxFanSpeed: parseInt(maxFanSpeed, 10),
    };

    try {
      sendMessage(JSON.stringify(settings));
      setStatus("Settings sent successfully!");
    } catch (error) {
      setStatus("Failed to send settings. Please try again.");
    }
  };

  return (
    <Box sx={{ padding: "20px", maxWidth: "600px", margin: "0 auto" }}>
      <Typography variant="h5" gutterBottom>
        Temperature and Fan Settings
      </Typography>
      <Grid container spacing={2}>
        <Grid item xs={6}>
          <TextField
            label="Min Temperature (°C)"
            type="number"
            fullWidth
            value={minTemp}
            onChange={(e) => setMinTemp(e.target.value)}
            variant="outlined"
          />
        </Grid>
        <Grid item xs={6}>
          <TextField
            label="Max Temperature (°C)"
            type="number"
            fullWidth
            value={maxTemp}
            onChange={(e) => setMaxTemp(e.target.value)}
            variant="outlined"
          />
        </Grid>
        <Grid item xs={6}>
          <TextField
            label="Min Fan Speed"
            type="number"
            fullWidth
            value={minFanSpeed}
            onChange={(e) => setMinFanSpeed(e.target.value)}
            variant="outlined"
          />
        </Grid>
        <Grid item xs={6}>
          <TextField
            label="Max Fan Speed"
            type="number"
            fullWidth
            value={maxFanSpeed}
            onChange={(e) => setMaxFanSpeed(e.target.value)}
            variant="outlined"
          />
        </Grid>
      </Grid>
      <Button
        variant="contained"
        color="primary"
        onClick={handleSendSettings}
        sx={{ marginTop: "20px" }}
      >
        Send Settings
      </Button>
      {status && (
        <Typography
          variant="body2"
          color={status.includes("successfully") ? "green" : "red"}
          sx={{ marginTop: "10px" }}
        >
          {status}
        </Typography>
      )}
    </Box>
  );
};

export default TemperatureSetting;
