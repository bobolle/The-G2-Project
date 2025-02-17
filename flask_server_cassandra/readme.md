# IoT Data Collection Server

A Flask-based API server with PostgreSQL database for collecting and managing IoT device data.

## System Architecture

- **Flask Server**: RESTful API endpoints for device data management
- **PostgreSQL Database**: Persistent storage for devices and sensor readings
- **Docker**: Containerized deployment with docker-compose

## Prerequisites

- Docker Desktop
- Python 3.9+
- curl (for testing)

## Quick Start

1. Clone the repository:
```bash
git clone <repository-url>
cd flask_server_cassandra

2. Start the containers:
docker-compose up --build
The server will be available at http://localhost:5000

API Endpoints
Endpoint	Method	Description
/	GET	API documentation
/api/devices	GET	List all devices
/api/devices/<deviceId>/data	GET	Get data for specific device
/api/data	POST	Submit new device reading
Example Usage
Submit Device Reading
Using curl
Database Schema
Device: Stores IoT device information

id: Primary key
name: Device name
DeviceData: Stores sensor readings

id: Primary key
device_id: Foreign key to Device
value: Sensor reading value
Docker Configuration
The docker-compose.yml file sets up:

PostgreSQL database container
Flask application container
Persistent volume for database data
Health checks for both services
Automatic service restart
Development
Install Python dependencies:
Set environment variables:
Run the Flask server locally:
Contributing
Fork the repository
Create a feature branch
Submit a pull request
License