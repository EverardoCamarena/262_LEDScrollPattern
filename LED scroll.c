#include <SI_EFM8BB1_Register_Enums.h>

void InitDevice(void)
{
	//Disable Watchdog Timer
	//Disable Watchdog with key sequence
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key
	// [WDTCN – Watchdog Timer Control]$

	//Setup Defaults for P1
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
	| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN
	| P1MDOUT_B4__PUSH_PULL | P1MDOUT_B5__OPEN_DRAIN
	| P1MDOUT_B6__OPEN_DRAIN | P1MDOUT_B7__OPEN_DRAIN;

	//Enable Weak Pullups
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;

	//Disable Interrupts
	IE = IE_EA__DISABLED | IE_EX0__DISABLED | IE_EX1__DISABLED
	| IE_ESPI0__DISABLED | IE_ET0__DISABLED | IE_ET1__DISABLED
	| IE_ET2__ENABLED | IE_ES0__DISABLED;

}

main(void)
{
	unsigned char i;
	unsigned char Out;
	int overflow;
	InitDevice();
	
	//initialization, execute once
	Out=0x01; //start led pattern at 0x01
	TMOD = 0x01; //configure timer 0
	TL0 = 0x4B; //timer 0 lower 8 bits, 40,011 for 
	TH0 = 0x9C; //timer 0 higher 8 bits, 0.1 seconds
	TCON = TCON | (0x01<<4); //TR0 = 1, start timer 0
	
	while(1)
	{
		//shift left
		i = 0;
		while(i<7){
			if((TCON & (0x01<<5)) == 0x20){
				TCON = TCON &~ (0x01<<4);
				TL0 = 0x4B;
				TH0 = 0x9C;
				TCON = TCON &~ (0x01<<5);
				TCON = TCON | (0x01<<4);
				overflow = overflow + 1;
				
			}
			if(~P0 & 0x04){
				if(overflow == 10){
					Out = Out<<1;
					if(P0 & 0x02){
						P1 = ~Out;
					} else {
						P1 = Out;
					}
					overflow = 0;
					i = i + 1;
				}
			}else{
				if(overflow == 5){
				Out = Out<<1;
				if(P0 & 0x02){
						P1 = ~Out;
					}else{
						P1 = Out;
					}
					overflow = 0;
					i = i + 1;
				}
			}
		}
		
		
		//shift right
		i = 0;
		while(i<7){
			if((TCON & (0x01<<5)) == 0x20){
				TCON = TCON &~ (0x01<<4);
				TL0 = 0x4B;
				TH0 = 0x9C;
				TCON = TCON &~ (0x01<<5);
				TCON = TCON | (0x01<<4);
				overflow = overflow + 1;
				
			}
			if(~P0 & 0x04){
				if(overflow == 10){
					Out = Out>>1;
					if(P0 & 0x02){
						P1 = ~Out;
					} else {
						P1 = Out;
					}
					overflow = 0;
					i = i + 1;
					
				}
			}else{
				if(overflow == 5){
				Out = Out>>1;
				if(P0 & 0x02){
						P1 = ~Out;
					}else{
						P1 = Out;
					}
					overflow = 0;
					i = i + 1;
				}
			}
		}		
	}
}