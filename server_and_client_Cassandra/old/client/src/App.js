import React, { useEffect, useState } from 'react';
import axios from 'axios';
import './App.css';

function App() {
  const [items, setItems] = useState([]);
  const [devices, setDevices] = useState([]);
  const [selectedItem, setSelectedItem] = useState(null);
  const [selectedDevice, setSelectedDevice] = useState(null);
  const [latestDeviceData, setLatestDeviceData] = useState(null);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const itemsResponse = await axios.get('http://localhost:5000/api/items');
        setItems(itemsResponse.data);
        const devicesResponse = await axios.get('http://localhost:5000/api/devices');
        setDevices(devicesResponse.data);
      } catch (error) {
        console.error('Error fetching data:', error);
      }
    };

    fetchData();
  }, []);

  useEffect(() => {
    const fetchLatestDeviceData = async () => {
      if (selectedDevice) {
        try {
          const response = await axios.get(`http://localhost:5000/api/devices/${selectedDevice.id}/data`);
          if (response.data.length > 0) {
            setLatestDeviceData(response.data[0]);
          } else {
            setLatestDeviceData(null);
          }
        } catch (error) {
          console.error('Error fetching latest device data:', error);
        }
      }
    };

    fetchLatestDeviceData();
  }, [selectedDevice]);

  return (
    <div className="App">
      <header className="App-header">
        <div className="sidebar">
          <h2>Items</h2>
          <ul>
            {items.map(item => (
              <li
                key={item.id}
                onClick={() => setSelectedItem(item)}
                className={selectedItem && selectedItem.id === item.id ? 'selected' : ''}
              >
                {item.name || item.id}
              </li>
            ))}
          </ul>
          <h2>Devices</h2>
          <ul>
            {devices.map(device => (
              <li
                key={device.id}
                onClick={() => setSelectedDevice(device)}
                className={selectedDevice && selectedDevice.id === device.id ? 'selected' : ''}
              >
                {device.name || device.id}
              </li>
            ))}
          </ul>
        </div>
        <div className="content">
          {selectedItem ? (
            <div className="card">
              <h2>Selected Item</h2>
              <pre>{JSON.stringify(selectedItem, null, 2)}</pre>
            </div>
          ) : (
            <div className="placeholder">
              <h2>Select an item from the sidebar</h2>
            </div>
          )}
          {selectedDevice && (
            <div className="card">
              <h2>Selected Device</h2>
              <pre>{JSON.stringify(selectedDevice, null, 2)}</pre>
              {latestDeviceData && (
                <div className="card">
                  <h2>Latest Device Data</h2>
                  <pre>{JSON.stringify(latestDeviceData, null, 2)}</pre>
                </div>
              )}
            </div>
          )}
        </div>
      </header>
    </div>
  );
}

export default App;