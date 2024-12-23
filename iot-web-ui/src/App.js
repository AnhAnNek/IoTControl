import React, { useState } from "react";
import "./App.css";
import FanControl from "./Pages/FanControl";
import FanMonitor from "./Pages/FanMonitor";
import {
  AppBar,
  Box,
  CssBaseline,
  Drawer,
  List,
  ListItem,
  ListItemButton,
  ListItemText,
  Toolbar,
  Typography,
} from "@mui/material";

const drawerWidth = 240;

function App() {
  const [selectedTab, setSelectedTab] = useState("FanControl");

  const handleTabChange = (tab) => {
    setSelectedTab(tab);
  };

  return (
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
            Fan Management Dashboard
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
              <ListItemButton onClick={() => handleTabChange("FanControl")}>
                <ListItemText primary="Fan Control" />
              </ListItemButton>
            </ListItem>
            <ListItem disablePadding>
              <ListItemButton onClick={() => handleTabChange("FanMonitor")}>
                <ListItemText primary="Fan Monitor" />
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
          ml: `${drawerWidth}px`,
        }}
      >
        <Toolbar />
        {selectedTab === "FanControl" && <FanControl />}
        {selectedTab === "FanMonitor" && <FanMonitor />}
      </Box>
    </Box>
  );
}

export default App;
