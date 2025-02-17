Rough structure

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

    E -->|POST /api/data| B
    A -->|GET /api/devices| B
    A -->|GET /api/devices/:deviceId/data| B

    B -->|Database Operations| C
