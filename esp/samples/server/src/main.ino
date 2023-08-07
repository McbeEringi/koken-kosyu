#include <LittleFS.h>
#include <ESPAsyncWebSrv.h>
#define PIN 0

AsyncWebServer svr(80);

void setup(){
	Serial.begin();LittleFS.begin();neopixelWrite(PIN,16,0,0);
	delay(1000);
	WiFi.begin();Serial.printf("WiFi");neopixelWrite(PIN,16,16,0);
	for(uint8_t i=0;WiFi.status()!=WL_CONNECTED;i++){
		if(i>20){
			Serial.printf("\nWiFi not found.\n\nSmartConfig started.\n");neopixelWrite(PIN,16,0,16);
			WiFi.beginSmartConfig();while(!WiFi.smartConfigDone());Serial.printf("SmartConfig success!\n");
		}
		Serial.printf(".");delay(500);
	}
	neopixelWrite(PIN,0,16,0);
	svr.onNotFound([](AsyncWebServerRequest *request){request->redirect("/");});
	svr.serveStatic("/",LittleFS,"/").setDefaultFile("index.html");
	svr.begin();
}
void loop(){
	Serial.printf("SSID: %s  IP: %s\n",WiFi.SSID().c_str(),WiFi.localIP().toString().c_str());
	delay(2000);
}
