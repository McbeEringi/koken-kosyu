#include <Arduino.h>

void setup(){Serial.begin(9600);}
void loop(){Serial.printf("Hello World! %u\n",millis());delay(100);}
