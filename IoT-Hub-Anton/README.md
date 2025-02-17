# MQTT Subscriber & PostgreSQL Logger

## Overview
This Python script listens for **MQTT messages**, logs the received **sensor data into a PostgreSQL database**, and periodically sends aggregated data to a **remote server API**.

## Features
- **Subscribes to an MQTT broker** to receive sensor data.
- **Stores received data in a PostgreSQL database**.
- **Deletes old records (older than 30 days)** to keep the database clean.
- **Aggregates data every 10 minutes** and sends it to a cloud server.

## Prerequisites

### Hardware
- A Raspberry Pi 2 or higher (or any machine running Python & PostgreSQL).
- An MQTT broker (Mosquitto, AWS IoT, etc.).
- A PostgreSQL database.

### Software
- Python 3.x
- PostgreSQL
- MQTT broker (Mosquitto recommended)
- Required Python libraries:
  - `paho-mqtt`
  - `psycopg2`
  - `requests`
  - `python-dotenv`

## Installation

### 1. Clone the Repository
```bash
git clone <repo-url>
cd <repo-directory>
```

### 2. Install Dependencies
```bash
pip install paho-mqtt psycopg2 requests python-dotenv
```

### 3. Configure Environment Variables
Create a `.env` file in the project root with the following structure:

```ini
DB_NAME=[Database name]
DB_USER=[Device name]
DB_PASSWORD=[Password]
DB_HOST=[Database host]
MQTT_BROKER=[MQTT Broker Host]
MQTT_TOPIC=[Your desired topic, e.g., sensors/edgedevice]
MQTT_USER=[MQTT User]
MQTT_PASSWORD=[MQTT Password]
SERVER_URL="server API url"
```

### 4. Run the Script
```bash
python mqtt_subscriber.py
```

## Configuration

### PostgreSQL Database Setup
Ensure you have a PostgreSQL instance running with a table created as follows:

```sql
CREATE TABLE sensor_log (
    device_id TEXT,
    lightlevel FLOAT,
    moisture FLOAT,
    error_code INT,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### MQTT Message Format
The script expects messages in the following JSON format:

```json
{
  "device_id": "Pico01",
  "lightlevel": 512,
  "moisture": 350,
  "error_code": 0
}
```

## Troubleshooting

### WiFi or Network Issues
- Ensure the MQTT broker is running and reachable.
- Verify that the correct `MQTT_BROKER` address is set in `.env`.
- Check firewall settings if communication is blocked.

### Database Issues
- Ensure PostgreSQL is running and accessible.
- Check `DB_HOST`, `DB_USER`, and `DB_PASSWORD` in `.env`.
- Verify that the table `sensor_log` exists.

### MQTT Message Not Received
- Ensure the correct topic is set (`MQTT_TOPIC`).
- Check if another script or client is consuming messages before this one.

## Future Improvements
- Add **buffering in case of network failures**.
- Improve **logging and error handling**.

## License
MIT License

## Acknowledgments
- Paho MQTT library for Python
- PostgreSQL for database management
- Raspberry Pi Foundation for IoT solutions

