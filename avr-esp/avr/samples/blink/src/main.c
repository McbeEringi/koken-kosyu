#define F_CPU 2500000 // 念のため
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define BTN_DOWN ~VPORTA.IN&(1<<7)

ISR(PORTA_PORT_vect){PORTA.INTFLAGS=PORT_INT7_bm;}// リセット必須

void blink(uint8_t x){for(uint8_t i=0;i<8;i++){if((x>>i)&1)PORTA.OUTSET=0b1000;else PORTA.OUTCLR=0b1000;_delay_ms(125);}}// 下位bitから読み込み MSBの状態のまま離脱 1/16秒*8
void main(){
	PORTA.DIRSET=0b1000;// 出力: PA3
	PORTA.PIN7CTRL=PORT_PULLUPEN_bm|PORT_ISC_LEVEL_gc;// PA7 プルアップ ピン割り込みはBOTHEDGESかLEVELじゃなきゃ起きない 
	while(1){
		blink(0b00110011);
		sei();SLPCTRL.CTRLA=SLPCTRL_SMODE_PDOWN_gc|SLPCTRL_SEN_bm;sleep_cpu();cli();while(BTN_DOWN)_delay_ms(1);
	}
}