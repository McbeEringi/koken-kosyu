#include <Arduino.h>

#define PIN_SER 0
#define PIN_SERCLK 1
#define PIN_RCLK 2
#define PIN_OE_ 3

void setup(){
	
	pinMode(PIN_SER,OUTPUT);digitalWrite(PIN_SER,HIGH);
	pinMode(PIN_SERCLK,OUTPUT);digitalWrite(PIN_SERCLK,HIGH);
	pinMode(PIN_RCLK,OUTPUT);digitalWrite(PIN_RCLK,HIGH);
	pinMode(PIN_OE_,OUTPUT);digitalWrite(PIN_OE_,LOW);
	analogWrite(PIN_OE_,200);

}
void loop(){
	shiftOut(PIN_SER,PIN_SERCLK,MSBFIRST,0b11111110);
	shiftOut(PIN_SER,PIN_SERCLK,MSBFIRST,0b10101100);
	digitalWrite(PIN_RCLK,LOW);digitalWrite(PIN_RCLK,HIGH);
	delay(1);
	shiftOut(PIN_SER,PIN_SERCLK,MSBFIRST,0b01100000);
	shiftOut(PIN_SER,PIN_SERCLK,MSBFIRST,0b01001100);
	digitalWrite(PIN_RCLK,LOW);digitalWrite(PIN_RCLK,HIGH);
	delay(1);
}
