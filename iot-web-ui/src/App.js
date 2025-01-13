import React, { useState } from "react";
import "./App.css";
import ESPControl from "./Components/ESPControl";
import TemperatureMonitor from "./Components/TemperatureMonitor";
import {
  AppBar,
  Box, createTheme,
  CssBaseline,
  Drawer,
  List,
  ListItem,
  ListItemButton,
  ListItemText, ThemeProvider,
  Toolbar,
  Typography,
} from "@mui/material";
import RobotControl from "./Components/RobotControl";

const drawerWidth = 190;

const theme = createTheme({
  palette: {
    primary: {
      main: "#1976d2", // blue color
    },
    secondary: {
      main: "#dc004e", // red color
    },
  },
});

function App() {
  const [selectedTab, setSelectedTab] = useState("ESPControl");

  const handleTabChange = (tab) => {
    setSelectedTab(tab);
  };

  return (
    <ThemeProvider theme={theme}>
      <Box sx={{ display: "flex" }}>
        <CssBaseline />

        {/* AppBar */}
        <AppBar
          position="fixed"
          sx={{
            width: `calc(100% - ${drawerWidth}px)`,
            ml: `${drawerWidth}px`,
          }}
        >
          <Toolbar>
            <Typography variant="h6" noWrap>
              IoT Management Dashboard
            </Typography>
          </Toolbar>
        </AppBar>

        {/* Drawer (Sidebar) */}
        <Drawer
          variant="permanent"
          sx={{
            width: drawerWidth,
            flexShrink: 0,
            [`& .MuiDrawer-paper`]: {
              width: drawerWidth,
              boxSizing: "border-box",
            },
          }}
        >
          <Toolbar />
          <Box sx={{ overflow: "auto" }}>
            <List>
              <ListItem disablePadding>
                <ListItemButton
                  onClick={() => handleTabChange("ESPControl")}
                  selected={selectedTab === "ESPControl"}
                >
                  <ListItemText primary="Main Control" />
                </ListItemButton>
              </ListItem>
              <ListItem disablePadding>
                <ListItemButton
                  onClick={() => handleTabChange("TemperatureMonitor")}
                  selected={selectedTab === "TemperatureMonitor"}
                >
                  <ListItemText primary="Temperature Monitor" />
                </ListItemButton>
              </ListItem>
              <ListItem disablePadding>
                <ListItemButton
                  onClick={() => handleTabChange("RobotControl")}
                  selected={selectedTab === "RobotControl"}
                >
                  <ListItemText primary="Robot Control" />
                </ListItemButton>
              </ListItem>
            </List>
          </Box>
        </Drawer>

        {/* Content Area */}
        <Box
          component="main"
          sx={{
            flexGrow: 1,
            bgcolor: "background.default",
            p: 3,
          }}
        >
          <Toolbar />
          {selectedTab === "ESPControl" && <ESPControl />}
          {selectedTab === "TemperatureMonitor" && <TemperatureMonitor />}
          {selectedTab === "RobotControl" && <RobotControl />}
        </Box>
      </Box>
    </ThemeProvider>
  );
}

export default App;
