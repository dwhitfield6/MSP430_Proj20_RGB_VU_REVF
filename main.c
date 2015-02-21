#include <msp430.h>

#define audio1Input INCH_0
#define audio2Input INCH_4
#define audio3Input INCH_5

#define GREEN BIT1
#define RED BIT3
#define BLUE BIT2


int averageValue =35;

unsigned int analogRead(unsigned int pin);

unsigned int GREENPWM =0;
unsigned int GREENPWMtemp =0;
unsigned int REDPWM =0;
unsigned int REDPWMtemp =0;
unsigned int BLUEPWM =0;
unsigned int BLUEPWMtemp =0;
int audio1level =0;
int lightcolor =0;
int lighttemp =0;
int lighttempTEMP =0;
unsigned int count =0;
int lightArray[40];

long lightAverage=0;
char i,ii;
long delaycount =0;



int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    P1DIR |= (GREEN + RED + BLUE);
    P1OUT |= (GREEN + RED + BLUE);

    TA0CCTL0 |= CCIE;                             // CCR0 interrupt enabled
    TA0CTL = TASSEL_2 + MC_1 + ID_0;           // SMCLK/8, upmode
    TA0CCR0 =  1000;

    __enable_interrupt();

       while(1)
       {

delaycount++;
if(delaycount >300)
{
	delaycount =0;
}
           audio1level = analogRead(audio2Input);
           lightArray[i] =  audio1level;


         if (audio1level > (lightAverage +10))
         {

        	 lighttempTEMP = audio1level >>1;
        	 if(lighttempTEMP > (lighttemp + 15))
        			 {
        				 lighttemp = lighttempTEMP;
        			 }

         }

        	 if(delaycount == 0)
        	 {
        	 lighttemp--;
        	 }
        	 if(lighttemp < 0)
        	 {
        		 lightcolor++;
        		 if(lightcolor>6)
        		 {
        			 lightcolor=0;
        		 }
        		 lighttemp =0;
        	 }


if(lightcolor ==0)
{
	REDPWMtemp = lighttemp;
	GREENPWMtemp =0;
	BLUEPWMtemp =0;
}
else if(lightcolor ==1)
{
	REDPWMtemp = 0;
	GREENPWMtemp =lighttemp;
	BLUEPWMtemp =0;
}
else if(lightcolor ==2)
{
	REDPWMtemp = 0;
	GREENPWMtemp =0;
	BLUEPWMtemp =lighttemp;
}
else if(lightcolor ==3)
{
	REDPWMtemp = lighttemp;
	GREENPWMtemp =0;
	BLUEPWMtemp =lighttemp;
}
else if(lightcolor ==4)
{
	REDPWMtemp = 0;
	GREENPWMtemp =lighttemp;
	BLUEPWMtemp =lighttemp;
}
else if(lightcolor ==5)
{
	REDPWMtemp = lighttemp;
	GREENPWMtemp =lighttemp;
	BLUEPWMtemp =0;
}
else
{
	REDPWMtemp = lighttemp;
	GREENPWMtemp =lighttemp;
	BLUEPWMtemp =lighttemp;
}

                           i++;

                           if(i == averageValue)
                           {
                        	   for(ii = averageValue;ii>0;ii-- )
                        	   {
                        		   lightAverage += lightArray[ii-1];
                        	   }
                        	   lightAverage = lightAverage / averageValue;
                        	   i=0;
                           }
_delay_cycles(30);
       }

}

unsigned int analogRead(unsigned int pin) {
 ADC10CTL0 = ADC10ON + ADC10SHT_0 + ADC10SHT0;
 ADC10CTL1 = ADC10SSEL_0 + pin;
 if (pin==INCH_0){
 ADC10AE0 = 0x01;
 }
 else if (pin==INCH_1){
 ADC10AE0 = 0x02;
 }
 else if (pin==INCH_2){
 ADC10AE0 = 0x04;
 }
 else if (pin==INCH_3){
  ADC10AE0 = 0x08;
  }
 else if (pin==INCH_4){
  ADC10AE0 = 0x10;
  }
 else if(pin==INCH_5){
 ADC10AE0 = 0x20;
 }
 else if(pin==INCH_6){
  ADC10AE0 = 0x40;
  }
 else if(pin==INCH_7){
  ADC10AE0 = 0x80;
  }
 ADC10CTL0 |= ENC + ADC10SC;
 //_delay_cycles(10);
 while (1) {
 if (((ADC10CTL0 & ADC10IFG)==ADC10IFG)) {
 ADC10CTL0 &= ~(ADC10IFG +ENC);
 break;
 }
 }
 return ADC10MEM;
 }

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
    count++;
    if(count == 127)
    {

        BLUEPWM = BLUEPWMtemp;
        REDPWM = REDPWMtemp;
        GREENPWM = GREENPWMtemp;

 	   if(REDPWM != 0)
 	   {
 		   P1OUT &= ~(RED); //red on
 	   }
 	   if(GREENPWM != 0)
 	   {
 		   P1OUT &= ~(GREEN); //green on
 	   }
 	   if(BLUEPWM != 0)
 	   {
 		   P1OUT &= ~(BLUE); //blue on
 	   }
 	   count =0;
    }
    if(GREENPWM == count)
    {
 	   P1OUT |= GREEN;
    }
    if(BLUEPWM == count)
    {
 	   P1OUT |= BLUE;
    }
    if(REDPWM == count)
    {
 	   P1OUT |= RED;
    }

}
