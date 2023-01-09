#include <Arduino.h>
// struct Config
// {
//     const char *wifiHost;
//     const char *wifiPassword;
//     const char *mqttHost;
//     const unsigned short int mqttPort;
//     const char *mqttUser;
//     const char *mqttPassWord;
//     const char *mqttTopic;
//     const char *mqttClientID;
// };
#include "config.h"
#include <U8g2lib.h>
#include <WiFi.h>
// #include <WebServer.h>
#include <PubSubClient.h>
#include "gif.h"


#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// WebServer server(80);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);

void lightOn()
{
    digitalWrite(LED_BUILTIN, HIGH);
}

void lightOff()
{
    digitalWrite(LED_BUILTIN, LOW);
}

void lightBlink()
{
    lightOn();
    delay(30);
    lightOff();
    delay(30);
    lightOn();
}

void configWifi()
{
    Serial.println("> Connecting WiFi");

    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(config.wifiHost, config.wifiPassword);
    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        retryCount++;
        if (retryCount > 10)
        {
            Serial.println("> Connect WiFi failed");
            retryCount++;
            return;
        }
    }
    Serial.println("");

    Serial.println("> Config local time");
    configTime(8 * 3600, 0, "ntp.aliyun.com", "cn.ntp.org.cn", "ntp.ntsc.ac.cn");
    Serial.println("Connect Wifi successful. IP: " + WiFi.localIP().toString());
}
String curDisplay = "";

void drawString(String str)
{
    curDisplay = str;
}

void drawTime()
{
    struct tm tm;
    char timeStr[64];
    if (getLocalTime(&tm))
    {
        strftime(timeStr, 64, "%Y-%m-%d %H:%M:%S", &tm);
        u8g2.drawUTF8(12, 12, timeStr);
    }
}
void drawWithLineFeed()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b);

    drawTime();

    int screenWidth = u8g2.getDisplayWidth() - 12;
    int strWidth = u8g2.getUTF8Width(curDisplay.c_str());

    if (strWidth <= screenWidth)
    {
        u8g2.drawUTF8(0, 12, curDisplay.c_str());
    }
    else
    {
        int startIndex = 0;
        int index = 0;
        int row = 0;
        while (index < curDisplay.length())
        {
            while (index < curDisplay.length() && u8g2.getUTF8Width(curDisplay.substring(startIndex, index).c_str()) < screenWidth)
            {
                index++;
            }
            u8g2.drawUTF8(0, 12 * (row + 2), curDisplay.substring(startIndex, index).c_str());
            startIndex = index;
            row++;
        }
    }

    u8g2.sendBuffer();
}

void drawWithRoll()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
    int screenWidth = u8g2.getDisplayWidth();
    int strWidth = u8g2.getUTF8Width(curDisplay.c_str());
    if (strWidth <= screenWidth)
    {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
        drawTime();
        u8g2.drawUTF8(0, 24, curDisplay.c_str());
        u8g2.sendBuffer();
    }
    else
    {
        for (int i = 0; i < strWidth + screenWidth; i++)
        {
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
            drawTime();
            u8g2.drawUTF8(screenWidth - i, 24, curDisplay.c_str());
            u8g2.sendBuffer();
        }
    }
}
// void configWebServer() {
//     Serial.print("Starting server");
//     server.begin();
//     Serial.print("Start server successful");
//     server.on("/", HTTP_GET, handleRoot);
// }
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    lightBlink();
    char *msg = (char *)malloc(length + 1);
    for (int i = 0; i < length; i++)
    {
        msg[i] = (char)payload[i];
    }
    msg[length] = '\0';
    Serial.printf("Get MQTT Message: [%s] - [%s]\n", topic, msg);
    drawString(msg);
}
void mqttReconnect()
{
    while (!mqttClient.connected())
    {
        Serial.println("MQTT connection...");
        if (mqttClient.connect(config.mqttClientID, config.mqttUser, config.mqttPassWord))
        {
            Serial.println("MQTT connected");
            mqttClient.subscribe(config.mqttTopic);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}
void configMQTT()
{
    mqttClient.setServer(config.mqttHost, config.mqttPort);
    mqttClient.setCallback(mqttCallback);
}

// void handleRoot()
// {
//     const char *indexHtml = "hello esp32";
//     log("Get request: /");
//     server.send(200, "text/plain", indexHtml);
// }

void drawGIF()
{

    for (int i = 35; i >= 0; i--)
    {
        u8g2.setBitmapMode(1);
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
        u8g2.drawXBM(10, 0, 64, 64, gif[i]);
        u8g2.sendBuffer();
        delay(20);
    }
}

void setup()
{
    Serial.begin(115200);
    u8g2.begin();
    u8g2.enableUTF8Print();

    configWifi();

    // configWebServer();

    configMQTT();

    pinMode(LED_BUILTIN, OUTPUT);
    lightOn();
}

void loop()
{
    // server.handleClient();
    if (!mqttClient.connected())
    {
        mqttReconnect();
    }
    mqttClient.loop();
    // struct tm tm;
    // if (getLocalTime(&tm))
    // {
    //     char timeStr[64];
    //     strftime(timeStr, 64, "%Y-%m-%d %H:%M:%S", &tm);
    //     drawString(timeStr);
    // }
    drawGIF();
    // drawWithRoll();
    // drawWithLineFeed();
}
