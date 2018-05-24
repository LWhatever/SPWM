#include <msp430.h> 
#include "setclock.h"
#define Num 256
#define RATE 510
#define adjust 22

/**
 * main.c
 */
unsigned int cnt = 4, tnt = 0, mode = 0;
const unsigned int sin_wave[Num]=
{
	495 ,489 ,483 ,477 ,471 ,465 ,459 ,453 ,447 ,441 ,435 ,429 ,423 ,417 ,411 ,405 ,399 ,
	393 ,388 ,382 ,376 ,370 ,364 ,358 ,353 ,347 ,341 ,335 ,330 ,324 ,319 ,313 ,307 ,302 ,
	296 ,291 ,285 ,280 ,275 ,269 ,264 ,259 ,253 ,248 ,243 ,238 ,233 ,228 ,223 ,218 ,213 ,
	208 ,203 ,198 ,194 ,189 ,184 ,180 ,175 ,171 ,166 ,162 ,157 ,153 ,149 ,145 ,140 ,136 ,
	132 ,128 ,124 ,120 ,117 ,113 ,109 ,106 ,102 ,98 ,95 ,92 ,88 ,85 ,82 ,79 ,75 ,72 ,69 ,
	67 ,64 ,61 ,58 ,56 ,53 ,50 ,48 ,46 ,43 ,41 ,39 ,37 ,35 ,33 ,31 ,29 ,27 ,26 ,24 ,23 ,
	21 ,20 ,18 ,17 ,16 ,15 ,14 ,13 ,12 ,11 ,10 ,10 ,9 ,9 ,8 ,8 ,8 ,7 ,7 ,7 ,7 ,7 ,7 ,8 ,
	8 ,8 ,9 ,9 ,10 ,10 ,11 ,12 ,13 ,14 ,15 ,16 ,17 ,18 ,20 ,21 ,23 ,24 ,26 ,27 ,29 ,31 ,
	33 ,35 ,37 ,39 ,41 ,43 ,46 ,48 ,50 ,53 ,56 ,58 ,61 ,64 ,67 ,69 ,72 ,75 ,79 ,82 ,85 ,
	88 ,92 ,95 ,98 ,102 ,106 ,109 ,113 ,117 ,120 ,124 ,128 ,132 ,136 ,140 ,145 ,149 ,153 ,
	157 ,162 ,166 ,171 ,175 ,180 ,184 ,189 ,194 ,198 ,203 ,208 ,213 ,218 ,223 ,228 ,233 ,
	238 ,243 ,248 ,253 ,259 ,264 ,269 ,275 ,280 ,285 ,291 ,296 ,302 ,307 ,313 ,319 ,324 ,
	330 ,335 ,341 ,347 ,353 ,358 ,364 ,370 ,376 ,382 ,388 ,393 ,399 ,405 ,411 ,417 ,423 ,
	429 ,435 ,441 ,447 ,453 ,459 ,465 ,471 ,477 ,483 ,489 ,495
};
const unsigned int sin_wave1[Num]=
{
	489,483,477,471,465,459,453,447,441,435,429,423,417,411,405,399,393,387,382,376,370,
	364,358,352,347,341,335,329,324,318,313,307,301,296,290,285,279,274,269,263,258,253,
	247,242,237,232,227,222,217,212,207,202,197,192,188,183,178,174,169,165,160,156,151,
	147,143,139,134,130,126,122,118,114,111,107,103,100,96,92,89,86,82,79,76,73,69,66,63,
	61,58,55,52,50,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,8,7,6,
	5,4,4,3,3,2,2,2,1,1,1,1,1,1,2,2,2,3,3,4,4,5,6,7,8,9,10,11,12,14,15,17,18,20,21,23,25,27,
	29,31,33,35,37,40,42,44,47,50,52,55,58,61,63,66,69,73,76,79,82,86,89,92,96,100,103,107,
	111,114,118,122,126,130,134,139,143,147,151,156,160,165,169,174,178,183,188,192,197,202,
	207,212,217,222,227,232,237,242,247,253,258,263,269,274,279,285,290,296,301,307,313,318,
	324,329,335,341,347,352,358,364,370,376,382,387,393,399,405,411,417,423,429,435,441,447,
	453,459,465,471,477,483,489
};

void ADC_Init()
{
    P6SEL |= BIT3;                            // P6.3 ADC option select
    ADC12CTL0 = ADC12ON + ADC12MSC;         // Sampling time, ADC12 on
    ADC12CTL1 = ADC12CONSEQ1 + ADC12SHP;
    ADC12CTL2 = ADC12RES_0;
    ADC12MCTL0 = ADC12INCH_3;
    ADC12CTL0 |= ADC12ENC;
    ADC12IE = 0x01;
}

void Timer_Init()
{
	P1DIR |= BIT4;
	P1SEL |= BIT4;
	P1DIR |= BIT3;
	P1SEL |= BIT3;
	TA0CCTL3 = OUTMOD_6;//Q1,3
	TA0CCTL2 = OUTMOD_2 ;//Q2,4
	TA0CCR0 = RATE;
	TA0CTL |= MC_3 + TASSEL_2 + TACLR;

	TB0CCR0 = RATE*2-1;
	TB0CTL |= MC_1 + TASSEL_2 + TACLR;
	TB0CCTL0 = CCIE;
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	SetClock_MCLK12MHZ_SMCLK12MHZ_ACLK32_768K();
	Timer_Init();
	//ADC_Init();
	__bis_SR_register(GIE);
	while(1)
	{
//		ADC12CTL0 |= ADC12SC;
	}
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
{
	cnt++;
	if(cnt == Num-adjust)
	{
		cnt = adjust;
		mode++;
	}
	mode %=2;
	switch(mode)
	{
	case 0:
		if(cnt == adjust)
		{
			TA0CCTL3 = OUTMOD_6;
			TA0CCTL2 = OUTMOD_2;
		}
		TA0CCR3 = sin_wave[cnt];
		TA0CCR2 = sin_wave1[cnt];
		break;
	case 1:
		if(cnt == adjust)
		{
			TA0CCTL2 = OUTMOD_6
			TA0CCTL3 = OUTMOD_2;
		}
		TA0CCR3 = sin_wave1[cnt];
		TA0CCR2 = sin_wave[cnt];
		break;
	default:break;
	}
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
	switch(__even_in_range(ADC12IV,34))
	{
		case  0: break;                           // Vector  0:  No interrupt
		case  2: break;                           // Vector  2:  ADC overflow
		case  4: break;                           // Vector  4:  ADC timing overflow
		case  6:                                  // Vector  6:  ADC12IFG0
			if(ADC12MEM0>128)
				P2OUT |= BIT7;
			else
				P2OUT &= ~BIT7;

			P4OUT = ADC12MEM0;
			 __no_operation();
		case  8: break;                           // Vector  8:  ADC12IFG1
		case 10: break;                           // Vector 10:  ADC12IFG2
		case 12: break;                           // Vector 12:  ADC12IFG3
		case 14: break;                           // Vector 14:  ADC12IFG4
		case 16: break;                           // Vector 16:  ADC12IFG5
		case 18: break;                           // Vector 18:  ADC12IFG6
		case 20: break;                           // Vector 20:  ADC12IFG7
		case 22: break;                           // Vector 22:  ADC12IFG8
		case 24: break;                           // Vector 24:  ADC12IFG9
		case 26: break;                           // Vector 26:  ADC12IFG10
		case 28: break;                           // Vector 28:  ADC12IFG11
		case 30: break;                           // Vector 30:  ADC12IFG12
		case 32: break;                           // Vector 32:  ADC12IFG13
		case 34: break;                           // Vector 34:  ADC12IFG14
		default: break;
	}
}
