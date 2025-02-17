import React, { useState, useEffect } from 'react';
import { Container, Paper, Grid, Typography, MenuItem, Select } from '@mui/material';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend } from 'recharts';
import axios from 'axios';

const API_URL = 'http://localhost:5000/api';

function App() {
  const [devices, setDevices] = useState([]);
  const [selectedDevice, setSelectedDevice] = useState('');
  const [deviceData, setDeviceData] = useState([]);

  useEffect(() => {
    fetchDevices();
  }, []);

  useEffect(() => {
    if (selectedDevice) {
      fetchDeviceData(selectedDevice);
    }
  }, [selectedDevice]);

  const fetchDevices = async () => {
    try {
      const response = await axios.get(`${API_URL}/devices`);
      setDevices(response.data);
    } catch (error) {
      console.error('Error fetching devices:', error);
    }
  };

  const fetchDeviceData = async (deviceId) => {
    try {
      const response = await axios.get(`${API_URL}/devices/${deviceId}/data`);
      setDeviceData(response.data);
    } catch (error) {
      console.error('Error fetching device data:', error);
    }
  };

  return (
    <Container maxWidth="lg" sx={{ mt: 4 }}>
      <Paper sx={{ p: 3 }}>
        <Grid container spacing={3}>
          <Grid item xs={12}>
            <Typography variant="h4" gutterBottom>
              The G2 project
            </Typography>
          </Grid>
          
          <Grid item xs={12}>
            <Select
              value={selectedDevice}
              onChange={(e) => setSelectedDevice(e.target.value)}
              fullWidth
              displayEmpty
            >
              <MenuItem value="">Select a device</MenuItem>
              {devices.map((device) => (
                <MenuItem key={device.id} value={device.id}>
                  {device.name}
                </MenuItem>
              ))}
            </Select>
          </Grid>

          {selectedDevice && (
            <>
              <Grid item xs={12}>
                <Typography variant="h6">Light Level</Typography>
                <LineChart width={800} height={300} data={deviceData}>
                  <CartesianGrid strokeDasharray="3 3" />
                  <XAxis dataKey="timestamp" />
                  <YAxis />
                  <Tooltip />
                  <Legend />
                  <Line
                    type="monotone"
                    dataKey="lightLevel"
                    stroke="#8884d8"
                    name="Light Level"
                  />
                </LineChart>
              </Grid>

              <Grid item xs={12}>
                <Typography variant="h6">Moisture Level</Typography>
                <LineChart width={800} height={300} data={deviceData}>
                  <CartesianGrid strokeDasharray="3 3" />
                  <XAxis dataKey="timestamp" />
                  <YAxis />
                  <Tooltip />
                  <Legend />
                  <Line
                    type="monotone"
                    dataKey="moistureLevel"
                    stroke="#82ca9d"
                    name="Moisture Level"
                  />
                </LineChart>
              </Grid>
            </>
          )}
        </Grid>
      </Paper>
    </Container>
  );
}

export default App;