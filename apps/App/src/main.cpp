#include <Arduino.h>

#include "Io.h"
#include "LibA.h"

void setup() {
  Serial.begin(115200);
  ioSetup();
}

void loop() {
  ioLoop();
  Serial.println(libA());
  delay(1000);
}
