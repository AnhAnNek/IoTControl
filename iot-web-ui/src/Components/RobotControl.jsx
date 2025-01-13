import React, { useState } from "react";
import MapDisplay from "./MapDisplay";
import {
  Box,
  Button,
  Typography,
  FormControlLabel,
  Switch,
  Slider,
  Paper,
} from "@mui/material";
import { useWebSocket } from "../Providers/WebSocketProvider";
import { Joystick } from "react-joystick-component";
import { styled } from "@mui/system";

// Styled components using Material UI's styled API
const SpeedSlider = styled(Slider)(({ theme }) => ({
  color: theme.palette.secondary.main,
  height: 10,
  borderRadius: 5,
  "& .MuiSlider-thumb": {
    backgroundColor: theme.palette.primary.main,
    "&:hover": {
      backgroundColor: theme.palette.primary.dark,
    },
  },
}));

const RobotControl = () => {
  const [isAutoMode, setIsAutoMode] = useState(false); // Track auto/manual mode
  const [speed, setSpeed] = useState(100); // Default speed
  const { sendMessage } = useWebSocket();

  // Handle joystick movement events
  const handleJoystickMove = (event) => {
    const { direction, distance } = event;

    let command = "";
    switch (direction) {
      case "FORWARD":
        command = "MOVE_FORWARD";
        break;
      case "BACKWARD":
        command = "MOVE_BACKWARD";
        break;
      case "LEFT":
        command = "ROTATE_LEFT";
        break;
      case "RIGHT":
        command = "ROTATE_RIGHT";
        break;
      default:
        command = "";
    }

    if (command) {
      const message = {
        type: "ROBOT_CONTROL",
        device: "robot",
        command: command,
        speed: Math.round(speed * (distance / 100)), // Adjust speed based on joystick distance
      };
      sendMessage(JSON.stringify(message));
    }
  };

  // Handle joystick stop event
  const handleJoystickStop = () => {
    const message = {
      type: "ROBOT_CONTROL",
      device: "robot",
      command: "STOP",
      speed: 0,
    };
    sendMessage(JSON.stringify(message));
  };

  return (
    <Box sx={{ padding: "30px", maxWidth: "900px", margin: "0 auto", textAlign: "center" }}>
      <Typography variant="h4" component="h1" gutterBottom sx={{ color: "#333" }}>
        Robot Control Dashboard
      </Typography>

      <Paper sx={{ padding: "20px", boxShadow: 3, borderRadius: "16px", backgroundColor: "#f9f9f9" }}>
        {/* Auto/Manual Control Switch */}
        <FormControlLabel
          control={
            <Switch
              checked={isAutoMode}
              onChange={(e) => setIsAutoMode(e.target.checked)}
              sx={{ marginBottom: "20px" }}
            />
          }
          label={isAutoMode ? "Auto Mode" : "Manual Mode"}
        />

        {/* Joystick for Manual Mode */}
        {!isAutoMode && (
          <Box
            sx={{
              display: "flex",
              flexDirection: "column",
              gap: "20px",
              alignItems: "center",
              justifyContent: "center",
              width: "100%",
              maxWidth: "500px",
            }}
          >
            <Typography variant="h6" sx={{ color: "#555", marginBottom: "10px" }}>
              Use the joystick to control the robot
            </Typography>

            <Joystick
              size={150}
              baseColor="#ccc"
              stickColor="#666"
              throttle={100} // Higher value reduces joystick sensitivity
              move={handleJoystickMove}
              stop={handleJoystickStop}
            />

            <Box sx={{ marginTop: "20px" }}>
              <Button
                variant="contained"
                color="error"
                onClick={() => handleJoystickStop()}
                sx={{
                  width: "150px",
                  height: "50px",
                  borderRadius: "25px",
                  backgroundColor: "#ff4e42",
                  boxShadow: "0px 4px 12px rgba(0, 0, 0, 0.2)",
                  "&:hover": {
                    backgroundColor: "#ff2f1f",
                  },
                }}
              >
                Stop
              </Button>
            </Box>
          </Box>
        )}

        {/* Speed Control (Only visible in Manual Mode) */}
        {!isAutoMode && (
          <Box sx={{ marginTop: "20px", width: "100%", maxWidth: "500px" }}>
            <Typography variant="h6" sx={{ color: "#555" }}>Speed Control</Typography>
            <SpeedSlider
              value={speed}
              onChange={(e, newValue) => setSpeed(newValue)}
              aria-labelledby="speed-slider"
              valueLabelDisplay="auto"
              valueLabelFormat={(value) => `${value}`}
              min={0}
              max={255}
            />
            <Typography variant="body1" sx={{ color: "#777" }}>Speed: {speed}</Typography>
          </Box>
        )}
      </Paper>

      {/* Map Display */}
      <MapDisplay />
    </Box>
  );
};

export default RobotControl;