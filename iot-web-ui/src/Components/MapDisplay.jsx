import React, { useEffect, useState } from 'react';
import { Box, Container, Typography, Paper, Button, Grid } from '@mui/material';

const MapDisplay = () => {
  const [mapData, setMapData] = useState([]);
  const [isDataUpdated, setIsDataUpdated] = useState(false);

  // Function to simulate adding random points to the map
  const generateRandomMapData = () => {
    const newMapData = [...mapData]; // Keep the old map data

    const width = 20; // Width of the map (number of points on the X-axis)
    const height = 20; // Height of the map (number of points on the Y-axis)

    // Generate random coordinates for the new point
    const randomX = Math.floor(Math.random() * width); // Random X position
    const randomY = Math.floor(Math.random() * height); // Random Y position

    // Add new point to the map with a 20% chance of being an obstacle
    newMapData.push({
      x: randomX,
      y: randomY,
      isObstacle: Math.random() < 0.2, // 20% chance of being an obstacle
    });

    // Update the map data state
    setMapData(newMapData);
    setIsDataUpdated(true);
  };

  useEffect(() => {
    // Redraw the map when the data has been updated
    if (isDataUpdated) {
      const canvas = document.getElementById('mapCanvas');
      const ctx = canvas.getContext('2d');
      const cellSize = 40; // Size of each cell on the map

      ctx.clearRect(0, 0, canvas.width, canvas.height); // Clear the canvas before redrawing

      // Loop through each point in the mapData and draw it
      mapData.forEach((point) => {
        if (point.isObstacle) {
          ctx.fillStyle = 'red'; // Red color for obstacles
        } else {
          ctx.fillStyle = 'blue'; // Blue color for free spaces
        }
        ctx.fillRect(point.x * cellSize, point.y * cellSize, cellSize, cellSize); // Draw the point as a square
      });
    }
  }, [mapData, isDataUpdated]);

  return (
    <Container>
      <Typography variant="h4" gutterBottom>
        Robot Map Simulation
      </Typography>
      <Paper elevation={3} sx={{ padding: 2, marginBottom: 2 }}>
        <Box display="flex" justifyContent="center" flexDirection="column" alignItems="center">
          <canvas
            id="mapCanvas"
            width={20 * 40} // Width of the canvas
            height={20 * 40} // Height of the canvas
            style={{ border: '2px solid black' }}
          ></canvas>
          <Button
            variant="contained"
            sx={{ marginTop: 2 }}
            onClick={generateRandomMapData}
          >
            Add Random Point to Map
          </Button>

          {/* Legend explaining the colors */}
          <Grid container spacing={2} justifyContent="center" sx={{ marginTop: 3 }}>
            <Grid item>
              <Box
                sx={{
                  width: '20px',
                  height: '20px',
                  backgroundColor: 'blue',
                  border: '1px solid black',
                }}
              />
              <Typography variant="body1" sx={{ marginTop: 1 }}>
                Free Space (Blue)
              </Typography>
            </Grid>
            <Grid item>
              <Box
                sx={{
                  width: '20px',
                  height: '20px',
                  backgroundColor: 'red',
                  border: '1px solid black',
                }}
              />
              <Typography variant="body1" sx={{ marginTop: 1 }}>
                Obstacle (Red)
              </Typography>
            </Grid>
          </Grid>
        </Box>
      </Paper>
    </Container>
  );
};

export default MapDisplay;
