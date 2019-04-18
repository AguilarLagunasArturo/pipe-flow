#include "mkr_config.h"

void setup() {
  Serial.begin(9600);
  wifi_setup();
  printWiFiStatus();
}

void loop() {
  create_web_page();
}
