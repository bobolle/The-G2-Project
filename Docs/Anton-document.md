IoT Project Architectural Documentation Template
Project Title: 

The G2 Project
Team Members: 

Felix – Scrum master
Olle – Teemo scrum master
Simon
Vicky
Anton
Cassandra
Marcus
Max
Date: 
[Provide the date of this document]
1. Project Overview
Objective: 

The objective of this project is to develop IoT architecture for monitoring plants and their environmental conditions within agricultural industries. The system will provide real-time data, analyze it and provide information to enhance plant care and efficiency within the agricultural context.
Scope: 

•	Develop a sensor-based monitoring device to collect environmental data like temperature, humidity, soil moisture.
•	Create a wireless communication system that transmits data to IoT-hub. 
•	Process data in IoT-Hub and send to cloud for more processing.
•	Create a user-friendly interface for monitoring and alerts as well as AI-analyzes of data.
•	Ensure scalability for different applications like agricultural industries and maybe home-use.
Key Deliverables: 

•	Functional IoT-system prototype.
•	Data acquisition from sensors and network transmission-system.
•	Web-based monitoring.
•	Documentation and user manual

2. System Architecture
2.1 Hardware Components 

•	Raspberry Pico W – Microprocessor 
•	Environmental sensors
o	Photoresistor
o	Humidity sensor
o	Soil moisture sensors
•	IoT hub
o	Raspberry Pi 2?
•	Power supply
o	Batteries? 
o	Solar?
o	Outlet?

2.2 Software Components 

•	Raspberry Pico SDK
o	FreeRTOS?
•	Network libraries
o	MQTT?
o	TCP?
o	HTTP?
•	Database
o	SQL?
•	UI
o	Web-platform
o	Mobile app


2.3 Network Configuration 

•	WiFi-communication between sensor-device and IoT-hub.
o	LAN
o	MQTT?
o	HTTP?
•	IoT-hub to cloud/database
o	WAN
o	HTTP?

3. Design Principles
3.1 Modularity

•	The system consists of independent modules like senor-devices, IoT Hub, cloud based storage and ensures easy updates and replacements.


3.2 Scalability 

•	The system is supposed to be very scalable with the possibility of adding as many sensor devices as desired.

3.3 Security Measures 

•	Data is encrypted during transmission
•	Authentication at device connection

3.4 Performance Optimization

•	With devices coded in C we ensure performance.
•	Data-compression before network transmission can improve performance.

4. Development Process
4.1 Development Tools and Technologies 

•	Visual studio code
o	Primary IDE
•	GitHub
o	Version control and colab.
•	Raspberry Pico SDK
o	SDK for programming Raspberry Pico.

4.2 Version Control 

•	Git with SSH authentication

4.3 Testing and Quality Assurance 

•	Implement unit testing for individual modules or devices.
•	Integration testing to ensure seamless flow of data.

5. Data Management
5.1 Data Collection 

•	Sensor-devices collect environmental data.

5.2 Data Processing 

•	Data is first processed in IoT Hub and sent to database cloud/server.

5.3 Data Storage 
[Detail where and how data is stored, noting any data integrity and privacy measures.]
6. Challenges and Solutions
[List the major challenges faced during the project and the solutions implemented to address them.]
7. Results and Discussion
7.1 Project Outcomes 
[Describe the outcomes of the project.]
7.2 Analysis of Results 
[Provide an analysis of the results obtained, linking them back to the project objectives.]
7.3 Lessons Learned 
[Discuss the key lessons learned during the project.]
8.	Future Work
[Outline potential future developments and improvements that could be made to the project.]
9.	References
[List any sources, articles, or external materials referenced in the document.]
10. Appendices
[Include any additional diagrams, code snippets, or materials relevant to the project.]
This template helps students structure their report in a comprehensive and organized manner, ensuring that all critical aspects of their IoT project are thoroughly documented and analyzed.
