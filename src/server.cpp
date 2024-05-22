#include <WiFiUDP.h>
#include <WiFi.h>
#include "config.h"
#include "server.h"
#include "display.h"

WiFiUDP server;

void initWifi()
{
    Serial.println("Connecting WiFi");

    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(CONFIG.wifiHost, CONFIG.wifiPassword);
    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        retryCount++;
        if (retryCount > 10)
        {
            Serial.println("Connect WiFi failed");
            retryCount++;
            return;
        }
    }
    Serial.println("Connect Wifi successful. IP: " + WiFi.localIP().toString());
    setDrawString("IP: " + WiFi.localIP().toString());
}
void configNetTime()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        return;
    }
    Serial.println("Config local time");
    configTime(8 * 3600, 0, "ntp.aliyun.com", "cn.ntp.org.cn", "ntp.ntsc.ac.cn");
}

void startServer()
{
    server.begin(CONFIG.serverPort);
    Serial.println("Server start successful");
    String address = "Server IP: " + WiFi.localIP().toString() + " : " + CONFIG.serverPort;
    setDrawString(address);
}

void processClient()
{
    int packetSize = server.parsePacket();
    if (packetSize)
    {
        lightBlink();
        Serial.println("Get client connected: " + server.remoteIP());
        String msg = server.readString();
        Serial.printf("Get client %s message: %s", server.remoteIP().toString(), msg);
        setDrawString(msg);
    }
}