#include <Arduino.h>
#include <WiFi.h>

#define PIN_SER 0
#define PIN_SERCLK 1
#define PIN_RCLK 2
#define PIN_OE_ 3

#define PIN_NEOPIXEL 8
#define PIN_SPK 10

#define PIN_BTN 9

#define nn2hz(N) (440*pow(2,((N)/12.)))

TaskHandle_t *h_flush;
typedef struct timeval timeval_t;
typedef struct tm tm_t;
timeval_t tv;tm_t tm;
const uint8_t num[]={
	0b11111100,0b01100000,0b11011010,0b11110010,
	0b01100110,0b10110110,0b10111110,0b11100000,
	0b11111110,0b11110110,0b11101110,0b00111110,
	0b10011100,0b01111010,0b10011110,0b10001110
};
uint8_t disp[4]={2,2,2,2};

enum sync_state_t{
	SYNC_BAD,
	SYNC_TRY,
  SYNC_WIFI_ING,
	SYNC_NTP_ING,
  SYNC_OK
};
sync_state_t sync_state=SYNC_TRY;

void flush(void *_){while(1)for(uint8_t i=0,x;i<4;i++){
	// >> __43_21d _GFEDCBA
	// 0bABCDEFG_ 0bd12_34__
	x=disp[i];
	// 0bABCDEFGd => 0bDEFABCGd
	if(i&2)x=((x<<3)&0b11100000)|((x>>3)&0b00011100)|(x&0b00000011);
	shiftOut(PIN_SER,PIN_SERCLK,MSBFIRST,x&0b11111110);
	shiftOut(PIN_SER,PIN_SERCLK,MSBFIRST,((x&1)<<7)|(0b01101100&~(1<<(uint8_t[]){6,5,3,2}[i])));
	digitalWrite(PIN_RCLK,LOW);digitalWrite(PIN_RCLK,HIGH);
	delay(1);
}}
void dispInit(){
	pinMode(PIN_SER,OUTPUT);digitalWrite(PIN_SER,HIGH);
	pinMode(PIN_SERCLK,OUTPUT);digitalWrite(PIN_SERCLK,HIGH);
	pinMode(PIN_RCLK,OUTPUT);digitalWrite(PIN_RCLK,HIGH);
	pinMode(PIN_OE_,OUTPUT);digitalWrite(PIN_OE_,LOW);
	xTaskCreateUniversal(flush,"flush",1024,NULL,1,h_flush,CONFIG_ARDUINO_RUNNING_CORE);
}
void dispBri(float x){analogWrite(PIN_OE_,(1-x)*256);}

void sync_task(void *_){
  sync_state=SYNC_BAD;
  WiFi.begin();
  for(uint8_t i=0;i<10;i++){
    sync_state=SYNC_WIFI_ING;
    if(WiFi.status()==WL_CONNECTED){
			sync_state=SYNC_NTP_ING;
			configTzTime("JST-9","ntp.nict.jp");getLocalTime(&tm);
			sync_state=SYNC_OK;
			break;
		}
    delay(1000);
  }
  WiFi.disconnect(true);
  vTaskDelete(NULL);
}
void ntpSync(){
	xTaskCreateUniversal(sync_task,"sync_task",4096,NULL,1,NULL,CONFIG_ARDUINO_RUNNING_CORE);
}

void setup(){
	neopixelWrite(PIN_NEOPIXEL,8,0,0);
	dispInit();
	pinMode(PIN_SPK,OUTPUT);digitalWrite(PIN_SPK,LOW);
	// tone(PIN_SPK,nn2hz(12+3),100);
	// tone(PIN_SPK,nn2hz(12+5),100);
	// tone(PIN_SPK,nn2hz(12+10),100);
	delay(1000);

}
void loop(){
	// neopixelWrite(PIN_NEOPIXEL,
	// 	(sin(millis()/1000.    )*.5+.5)*16.,
	// 	(sin(millis()/1000.+2.1)*.5+.5)*16.,
	// 	(sin(millis()/1000.+4.2)*.5+.5)*16.
	// );
	switch(sync_state){
		case SYNC_BAD:{neopixelWrite(PIN_NEOPIXEL,8,0,0);break;}
		case SYNC_TRY:{neopixelWrite(PIN_NEOPIXEL,6,2,0);break;}
		case SYNC_WIFI_ING:{neopixelWrite(PIN_NEOPIXEL,4,4,0);break;}
		case SYNC_NTP_ING:{neopixelWrite(PIN_NEOPIXEL,2,6,0);break;}
		case SYNC_OK:{neopixelWrite(PIN_NEOPIXEL,0,8,0);break;}
	}
	// dispBri(
	// 	(sin(millis()/1000.    )*.5+.5)
	// );
	// delay(10);

  if(!tm.tm_min&&!tm.tm_sec&&tv.tv_usec<200000)sync_state=SYNC_TRY;
	if(sync_state==SYNC_TRY)ntpSync();
	gettimeofday(&tv,NULL);localtime_r(&tv.tv_sec,&tm);//https://8ttyan.hatenablog.com/entry/2015/02/03/003428
  switch(tm.tm_sec%10){
    case 4:{// temp
			int8_t t=int8_t(floor(temperatureRead()))-5;
			disp[0]=t<0?2:0;
			disp[1]=num[abs(t/10%10)];
			disp[2]=num[abs(t%10)];
			disp[3]=0b10011101;
			break;
		}
		case 5:{// date
			disp[0]=(tm.tm_mon+1)<10?0:num[(tm.tm_mon+1)/10];
			disp[1]=num[(tm.tm_mon+1)%10]|1;
			disp[2]=tm.tm_mday<10?0:num[tm.tm_mday/10];
			disp[3]=num[tm.tm_mday%10];
			break;
		}
    default:{// time
			disp[0]=tm.tm_hour<10?0:num[tm.tm_hour/10];
			disp[1]=num[tm.tm_hour%10]|1;//50000<tv.tv_usec;
			disp[2]=num[tm.tm_min/10]|1;//50000<tv.tv_usec;
			disp[3]=num[tm.tm_min%10];
		}
  }
	delay(1000-(tv.tv_usec/1000));

}
