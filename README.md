# Daly BMS UART API

This project turns an ESP32 into a small local network API and web interface for Daly BMS devices.
The ESP32 communicates with the BMS via UART and exposes battery data over HTTP in JSON format.

It is ideal for dashboards, home automation, monitoring tools, or custom battery projects.

## Features
- UART communication with Daly BMS
- REST-style JSON API (/data)
- Built-in web interface served by the ESP32
- Automatic data refresh every 20 seconds
- Runs entirely in the local network
- CORS enabled for easy external access

## Connection
|Daly BMS|ESP32|
|----|----|
|TX|GPIO 16 (RX)|
|RX|GPIO 17 (TX)|
|GND| GND|

## Software Setup
1. Clone the repository
2. Rename `secrets.example.h` to `secrets.h`
3. Fill in your WiFi credentials inside secrets.h
4. Upload to the esp32

## Usage
After uploading your code, watch the serial console (at 9600 baud rate) for the ip address of your esp32.
Open the ip address inside your browser and you will see a simple website with information of your bms.
For the API, you can simple request the `/data` route on your esp32 ip address.

## Used Library

This project uses the daly-bms-uart library:
https://github.com/maland16/daly-bms-uart
All BMS communication is handled through this library.
