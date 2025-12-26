#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "daly-bms-uart.h"
#include "website_data.h"
#include "secrets.h" // rename the secrets.example.h to secrets.h and update your data

#define DALY_RX 16
#define DALY_TX 17
#define UPDATE_INTERVAL 20000 // update every milliseconds

Daly_BMS_UART bms(Serial2);
WebServer server(80);

String jsonOutput;
unsigned long lastUpdateTime = 0;

void updateBmsJson()
{
    bms.update();

    DynamicJsonDocument doc(8192);

    doc["voltage"] = bms.get.packVoltage;
    doc["current"] = bms.get.packCurrent;
    doc["soc"] = bms.get.packSOC;

    JsonObject cells = doc.createNestedObject("cells");
    cells["max_mv"] = bms.get.maxCellmV;
    cells["max_num"] = bms.get.maxCellVNum;
    cells["min_mv"] = bms.get.minCellmV;
    cells["min_num"] = bms.get.minCellVNum;
    cells["diff"] = bms.get.cellDiff;
    cells["count"] = bms.get.numberOfCells;

    JsonArray cellV = cells.createNestedArray("voltages");
    for (int i = 0; i < bms.get.numberOfCells; i++)
    {
        cellV.add(bms.get.cellVmV[i]);
    }

    JsonObject temps = doc.createNestedObject("temperatures");
    temps["max"] = bms.get.tempMax;
    temps["min"] = bms.get.tempMin;
    temps["avg"] = bms.get.tempAverage;

    JsonArray tempSensors = temps.createNestedArray("sensors");
    for (int i = 0; i < bms.get.numOfTempSensors; i++)
    {
        tempSensors.add(bms.get.cellTemperature[i]);
    }

    doc["status"] = bms.get.chargeDischargeStatus;
    doc["charge_fet"] = bms.get.chargeFetState;
    doc["discharge_fet"] = bms.get.disChargeFetState;
    doc["res_capacity_mah"] = bms.get.resCapacitymAh;
    doc["cycles"] = bms.get.bmsCycles;

    jsonOutput = "";
    serializeJson(doc, jsonOutput);
    Serial.println("JSON Data Updated.");
}

void setup()
{
    Serial.begin(9600);
    Serial2.begin(9600, SERIAL_8N1, DALY_RX, DALY_TX);
    bms.Init();

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    Serial.print("ESP32 IP address: ");
    Serial.println(WiFi.localIP());

    // serve the web interface on the main route
    server.on("/", HTTP_GET, []()
              { server.send(200, "text/html", INDEX_HTML); });

    // serve the api on the /data route
    server.on("/data", HTTP_GET, []()
              { 
            server.sendHeader("Access-Control-Allow-Origin", "*");    
            server.send(200, "application/json", jsonOutput); });

    server.begin();
    updateBmsJson();
}

void loop()
{
    server.handleClient();

    if (millis() - lastUpdateTime >= UPDATE_INTERVAL)
    {
        lastUpdateTime = millis();
        updateBmsJson();
    }
}