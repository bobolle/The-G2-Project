# iot-hub

IoT hub for collecting, storing and sending data to cloud.
Goal is to make it easy to setup and use.

TODO:
- [x] Store data on postgresql
- [x] Check so it works with pico-w but i cba because mqtt with lwip is dogshit
- [ ] Create working healthchecks
- [ ] Auth for mqtt broker
- [x] HTTP requests to cloud

## Start hub
```
./docker.sh
```

## Cleanup
```
./clean.sh
```

## PSQL in DB
```
./exec_db.sh
```

```mermaid
graph LR
    E[Edge] --> |pub: sensor/distance|B
    subgraph Fog["Fog iot-hub"]
        B[MQTT-Broker] -.-> A[Hub Subscriber] --> DB[(Hub DB)]
        A[Hub Subscriber] --> |sub: sensor/distance|B[MQTT-Broker]
        DB[(Hub DB)] -.-> A[Hub Subscriber]
    end
    A[Hub Subscriber] --> |HTTP POST|C[Cloud]
```
