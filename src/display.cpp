#include "display.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

String curDisplay = "";
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);
void initDisplay() {
    pinMode(LED_BUILTIN, OUTPUT);
    u8g2.begin();
    u8g2.enableUTF8Print();
}
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

void setDrawString(String str)
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
