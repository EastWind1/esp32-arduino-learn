#include "server.h"
#include "display.h"


void setup()
{
  Serial.begin(115200);
  initDisplay();
  initWifi();
  configNetTime();

  startServer();
  lightOn();
}

void loop()
{
  processClient();
  drawWithRoll();
}
