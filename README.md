# The G2 Project

- DB: PostgreSQL DB
- Protocol communication: MQTT, HTTP
- Avg. calculations done on hub.
- Log handling, store in database, PostgreSQL

## High-Level Architechture Diagram

```mermaid

---
config:
  layout: fixed
---
flowchart LR
 subgraph subGraph0[" "]
        B("Connect")
        A["Phone App"]
        C["Website"]
  end
 subgraph subGraph1[" "]
        F["Moisture Sensor"]
        E1{"Plant Device"}
        G["Temperature Sensor"]
        H["Light Sensor"]
        I["Auto-Shading"]
        E3{"Actuator"}
        J["Auto-Watering"]
  end
 subgraph subGraph2[" "]
        Y1["Plant species"]
        Y{"Plant Database"}
        Y2["Frequency of watering"]
        Y22["Optimal values"]
        Y3["Amount of water"]
        Y32["Optimal values"]
        Y4["Sunlight"]
        Y42["Optimal values"]
        Y5["Types of dirt/earth"]
        Y6["Defences"]
        Y7["Insectides"]
        Y8["Fungicides"]
  end
    A --> B
    C --> B
    B --> Z["Cloud and Databases"]
    D{"IoT Hub"} <-- Gateway --> E1
    E1 --> E3
    Z --> Y
    Z <--> D
    D <--> E4{"AI system"}
    E1 <---> F & G & H
    E3 <--> I & J
    Y --> Y1 & Y2 & Y3 & Y4 & Y5 & Y6
    Y2 --> Y22
    Y3 --> Y32
    Y4 --> Y42
    Y6 --> Y7 & Y8
    style D stroke:#333,stroke-width:4px
```

## Network Topology
![image](https://github.com/user-attachments/assets/17baee00-ec64-478c-a127-39611861b1a1)
