#include <Arduino.h>

void setup(){
	Serial.begin(115200);
	pinMode(4,INPUT_PULLUP);
	pinMode(5,INPUT_PULLUP);
}
void loop(){
	Serial.println(
		digitalRead(4)|
		(digitalRead(5)<<1)
	);
}
