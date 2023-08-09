#define F_CPU 20000000
#define WT_L 48

#include <avr/io.h>

const uint8_t wt[]={128,144,160,176,191,205,218,229,238,245,251,254,255,254,251,245,238,229,218,205,191,176,160,144,128,111,95,79,64,50,37,26,17,10,4,1,0,1,4,10,17,26,37,50,64,79,95,111};
// [...Array(48)].map((_,i,a)=>Math.round((Math.sin(i/a.length*Math.PI*2)*.5+.5)*255))+'';
uint8_t t=0;

void main(){
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB,0);
	PORTA.DIRSET=0b100;// PA2を出力ピンに設定

	TCA0.SINGLE.CTRLA=TCA_SINGLE_ENABLE_bm;// 分周無し TCA有効
	TCA0.SINGLE.CTRLB=TCA_SINGLE_CMP2EN_bm|TCA_SINGLE_WGMODE_SINGLESLOPE_gc;// wo2有効 単傾斜PWM
	TCA0.SINGLE.PER=0xff;// TOP 8bit

	TCB0.CTRLA=TCB_ENABLE_bm;// 分周無し TCB有効
	TCB0.CTRLB=TCB_CNTMODE_INT_gc;// 周期的割り込み動作
	TCB0.CCMP=F_CPU/440/WT_L;// TOP

	while(1){
		TCA0.SINGLE.CMP2BUF=wt[t++];// 波形取得
		if(WT_L<=t)t=0;
		while(!TCB0.INTFLAGS);TCB0.INTFLAGS=1;// TCB待ち 1書いて解除 0の書き込みは無効
	}
}