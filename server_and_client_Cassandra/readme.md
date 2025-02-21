# Plant Test Project

This project consists of a React frontend and an Express backend. The frontend is built using Create React App, and the backend uses Postgresql and sqlalchemy for database operations.

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
    |GET /api/devices|B-->A
    |GET /api/devices/:deviceId/data|B-->A

    B<-->|Database Operations|C
```
