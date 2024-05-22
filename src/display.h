#include <U8g2lib.h>
#include <server.h>
#include <Arduino.h>

/**
 * 初始化屏幕
 */
void initDisplay();
/**
 * LED开
 */
void lightOn();
/**
 * LED关
 */
void lightOff();
/**
 * LED闪烁一次
 */
void lightBlink();
/**
 * 设置显示内容
 * @param str 
 */
void setDrawString(String str);
/**
 * 显示时间
 */
void drawTime();
/**
 * 绘制内容，超过屏幕自动折行
 */
void drawWithLineFeed();
/**
 * 绘制内容，超过屏幕自动滚动
 */
void drawWithRoll();