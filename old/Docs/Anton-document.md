# IoT Project Architectural Documentation Template

## Project Title
The G2 Project

## Team Members
- **Felix** – Scrum Master
- **Olle** – Teemo Scrum Master
- **Simon**
- **Vicky**
- **Anton**
- **Cassandra**
- **Marcus**
- **Max**

## Date
[Provide the date of this document]

---
## 1. Project Overview

### Objective
The objective of this project is to develop IoT architecture for monitoring plants and their environmental conditions within agricultural industries. The system will provide real-time data, analyze it, and provide information to enhance plant care and efficiency within the agricultural context.

### Scope
- Develop a sensor-based monitoring device to collect environmental data like temperature, humidity, soil moisture.
- Create a wireless communication system that transmits data to an IoT hub.
- Process data in the IoT Hub and send it to the cloud for further processing.
- Create a user-friendly interface for monitoring and alerts as well as AI analysis of data.
- Ensure scalability for different applications like agricultural industries and potentially home-use.

### Key Deliverables
- Functional IoT system prototype.
- Data acquisition from sensors and network transmission system.
- Web-based monitoring.
- Documentation and user manual.

---
## 2. System Architecture

### 2.1 Hardware Components
- **Raspberry Pico W** – Microprocessor.
- **Environmental Sensors:**
  - Photoresistor
  - Humidity sensor
  - Soil moisture sensors
- **IoT Hub:**
  - Raspberry Pi 2? 
- **Power Supply:**
  - Batteries?
  - Solar?
  - Outlet?

### 2.2 Software Components
- **Raspberry Pico SDK**
  - FreeRTOS?
- **Network Libraries:**
  - MQTT?
  - TCP?
  - HTTP?
- **Database:**
  - SQL?
- **User Interface:**
  - Web-platform
  - Mobile app

### 2.3 Network Configuration
- **WiFi communication** between sensor device and IoT hub:
  - LAN
  - MQTT?
  - HTTP?
- **IoT Hub to Cloud/Database:**
  - WAN
  - HTTP?

---
## 3. Design Principles

### 3.1 Modularity
- The system consists of independent modules like sensor devices, IoT Hub, cloud-based storage, ensuring easy updates and replacements.

### 3.2 Scalability
- The system is designed to be highly scalable, allowing the addition of multiple sensor devices as needed.

### 3.3 Security Measures
- Data is encrypted during transmission.
- Authentication is required for device connection.

### 3.4 Performance Optimization
- Devices coded in C ensure high performance.
- Data compression before network transmission enhances efficiency.

---
## 4. Development Process

### 4.1 Development Tools and Technologies
- **Visual Studio Code** – Primary IDE.
- **GitHub** – Version control and collaboration.
- **Raspberry Pico SDK** – Development framework for Raspberry Pico.

### 4.2 Version Control
- **Git with SSH authentication**.

### 4.3 Testing and Quality Assurance
- Implement **unit testing** for individual modules or devices.
- Conduct **integration testing** to ensure seamless data flow.

---
## 5. Data Management

### 5.1 Data Collection
- Sensor devices collect environmental data.

### 5.2 Data Processing
- Data is first processed in the IoT Hub before being sent to a cloud database/server.

### 5.3 Data Storage
- [Detail where and how data is stored, noting any data integrity and privacy measures.]

---
## 6. Challenges and Solutions
[List the major challenges faced during the project and the solutions implemented to address them.]

---
## 7. Results and Discussion

### 7.1 Project Outcomes
[Describe the outcomes of the project.]

### 7.2 Analysis of Results
[Provide an analysis of the results obtained, linking them back to the project objectives.]

### 7.3 Lessons Learned
[Discuss the key lessons learned during the project.]

---
## 8. Future Work
- Improve sensor accuracy and power efficiency.
- Implement AI-based analytics for predictive insights.
- Explore additional wireless communication technologies (e.g., LoRa, Bluetooth Low Energy).

---
## 9. References
[List any sources, articles, or external materials referenced in the document.]

---
## 10. Appendices
[Include any additional diagrams, code snippets, or materials relevant to the project.]

