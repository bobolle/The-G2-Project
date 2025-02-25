# Plant Test Project

This project consists of a React frontend and an Express backend. The frontend is built using Create React App, and the backend uses Postgresql and sqlalchemy for database operations.

Graph:
```mermaid
graph TD
    subgraph Client
        A[React Frontend]
    end

    subgraph Server
        B[Express Backend]
        C[Postgresql Database]
    end

    subgraph External
        E[Raspberry Pi]
    end

    E-->|POST /api/data|B
    A-->|GET /api/devices|B
    A-->|GET /api/devices/:deviceId/data|B

    B<-->|Database Operations|C
```

Sequence diagram:
```mermaid
sequenceDiagram
    participant Raspberry Pi
    participant Express Backend
    participant Postgresql Database
    participant React Frontend

    Raspberry Pi->>Express Backend: POST /api/data
    Express Backend->>Postgresql Database: Database Operations (Insert)
    Postgresql Database-->>Express Backend: Success
    Express Backend-->>Raspberry Pi: 200 OK

    React Frontend->>Express Backend: GET /api/devices
    Express Backend->>Postgresql Database: Database Operations (Query)
    Postgresql Database-->>Express Backend: Devices Data
    Express Backend-->>React Frontend: Devices Data

    React Frontend->>Express Backend: GET /api/devices/:deviceId/data
    Express Backend->>Postgresql Database: Database Operations (Query)
    Postgresql Database-->>Express Backend: Device Data
    Express Backend-->>React Frontend: Device Data
```
