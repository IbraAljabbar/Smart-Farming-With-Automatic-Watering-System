# Embbeded-Systems-Smart-Farming-Automatic-Watering-System
An automated plant watering system for Smart Farming built with ESP32, a capacitive soil moisture sensor, and a DHT11 sensor to optimize agricultural water efficiency.

This project is an automated agricultural system (Smart Farming) designed to optimize water usage efficiency for plants through real-time monitoring and automation. Powered by the ESP32 microcontroller, the system accurately tracks environmental parameters using a Capacitive Soil Moisture Sensor to measure soil dryness and a DHT11 sensor to monitor ambient temperature and humidity.

When the system detects that the soil moisture drops below a predefined threshold, the ESP32 triggers a 5V 1-Channel Relay to activate a Mini Water Pump powered by a 9V Battery. Water is then distributed directly to the plant via a Vinyl Tubing/Hose. For safety and status indication, an audible Buzzer alerts users while the watering process is active or if the system encounters critical conditions. All components are modularly assembled using a Breadboard and Jumper Wires, making this setup a reliable, efficient prototype that is fully capable of future IoT (Internet of Things) cloud integration.

| Component Name | Description / Function |
| :--- | :--- |
| ESP32-WROOM-32D (Micro USB) | Main microcontroller with Wi-Fi & Bluetooth to process sensor data. |
| Capacitive Soil Moisture Sensor | Measures soil moisture levels (corrosion-resistant). |
| DHT11 | Measures ambient temperature and air humidity. |
| Mini Water Pump | Pumps and distributes water to the plants through the hose. |
| 5V 1-Channel Relay Module | Acts as an electronic switch for the ESP32 to safely control the mini water pump. |
| Buzzer | Provides audible alerts during watering or system errors. |
| 9V Battery | External power supply dedicated to driving the mini water pump via the relay. |
| Water Tubing / Hose | Directs the water flow from the source to the plant soil. |
| Breadboard & Jumper Wires | Used for prototyping and connecting all electronic components together. |
