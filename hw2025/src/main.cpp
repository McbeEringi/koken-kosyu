#include <Arduino.h>
// #include <driver/ledc.h>

#define PIN_NEOPIXEL 8

#define PIN_SER 0
#define PIN_SERCLK 1
#define PIN_RCLK 2
#define PIN_OE_ 3
#define PIN_SPK 10

// #define PWM_OE_ 1
//
// #define PWM_FREQ 9700
// #define PWM_BIT LEDC_TIMER_12_BIT
// #define PWM_MAX (1<<12)



#define nn2hz(N) (440*pow(2,((N)/12.)))

TaskHandle_t *h_flush;

void np(){
	neopixelWrite(PIN_NEOPIXEL,
		(sin(millis()/1000.    )*.5+.5)*16.,
		(sin(millis()/1000.+2.1)*.5+.5)*16.,
		(sin(millis()/1000.+4.2)*.5+.5)*16.
	);
}
void seg(uint16_t x){
	shiftOut(PIN_SER,PIN_SERCLK,LSBFIRST,x&255);
	shiftOut(PIN_SER,PIN_SERCLK,LSBFIRST,x>>8);
	digitalWrite(PIN_RCLK,LOW);digitalWrite(PIN_RCLK,HIGH);
}
void flush(void *_){
	while(1){
	// 0b_GFEDCBA
	// 0b__43_21d
	// seg(0b0011010000111111);delay(1);
	// seg(0b0011001100000110);delay(1);
	// seg(0b0010011101011011);delay(1);
	// seg(0b0001011001111001);delay(1);
	seg(0b0011010001101101);delay(1);
	seg(0b0011001000000110);delay(1);
	seg(0b0010011001011011);delay(1);
	seg(0b0001011101000011);delay(1);
	}
}

void setup(){
	pinMode(PIN_SER,OUTPUT);digitalWrite(PIN_SER,HIGH);
	pinMode(PIN_SERCLK,OUTPUT);digitalWrite(PIN_SERCLK,HIGH);
	pinMode(PIN_RCLK,OUTPUT);digitalWrite(PIN_RCLK,HIGH);
	pinMode(PIN_OE_,OUTPUT);//ledcSetup(PWM_OE_,PWM_FREQ,PWM_BIT);ledcAttachPin(PIN_OE_,PWM_OE_);
	pinMode(PIN_SPK,OUTPUT);
	tone(PIN_SPK,nn2hz(12+3),100);
	tone(PIN_SPK,nn2hz(12+5),100);
	tone(PIN_SPK,nn2hz(12+10),100);
	xTaskCreateUniversal(flush,"flush",1024,NULL,1,h_flush,CONFIG_ARDUINO_RUNNING_CORE);
}
void loop(){
	np();
	analogWrite(PIN_OE_,(sin(millis()/1000.)*.5+.5)*255);delay(10);
}
