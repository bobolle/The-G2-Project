# The G2 Project

## Overview
The G2 Project is an IoT application that utilizes an Express server to handle data from various devices and interact with Firebase Firestore for data storage and retrieval.

## Project Structure
```
The-G2-Project
├── server_and_client_Cassandra
│   └── server
│       ├── Dockerfile
│       ├── .dockerignore
│       ├── server.js
│       ├── package.json
│       ├── serviceAccountKey.json
│       └── README.md
```

## Getting Started

### Prerequisites
- Node.js (version 14 or higher)
- npm (Node Package Manager)
- Docker (for containerization)

### Installation
1. Clone the repository:
   ```
   git clone <repository-url>
   cd The-G2-Project/server_and_client_Cassandra/server
   ```

2. Install the required dependencies:
   ```
   npm install
   ```

### Running the Server
To run the server locally, use the following command:
```
npm start
```
The server will start on `http://localhost:5000`.

### Docker Setup
To build and run the Docker container, follow these steps:

1. Build the Docker image:
   ```
   docker build -t g2-project-server .
   ```

2. Run the Docker container:
   ```
   docker run -p 5000:5000 g2-project-server
   ```

The server will be accessible at `http://localhost:5000` from your host machine.

### API Endpoints
- `POST /api/data`: Store data from a device.
- `GET /api/items`: Retrieve items from the plant database.
- `GET /api/devices`: Retrieve devices from the device database.
- `GET /api/devices/:deviceId/data`: Retrieve the latest data for a specific device.

### License
This project is licensed under the MIT License. See the LICENSE file for details.