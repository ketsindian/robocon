/*
motor direction pins on portA
motor 		ina1		ina2
1 			PA0			PA1
2			PA2			PA3
3			PA4			PA5
4			PA6			PA7			

interrupts on port E
motor 		interrupt 		 pin
1 			INT_4			 PE4
2			INT_5			 PE5
3			INT_6			 PE6
4			INT_7			 PE7

PWMs for motors
motor		pin 		timer
1 			PB5			1(OC1A)
2			PE3			3(OC3A)
3			PH3			4(OC4A)
4			PL3			5(OC5A)

X motor1 moto3
Y motor2 motor4
*/


#ifndef F_CPU
#define F_CPU 1000000UL
#endif 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define sbi(x,y)  x|=(1<<y)     //set bit
#define cbi(x,y)  x&=~(1<<y)    //clear bit

#define ppm 500

volatile unsigned int count1 =0;
volatile unsigned int count2 =0;
volatile unsigned int count3 =0;
volatile unsigned int count4 =0;

ISR(INT4_vect)
{
	count1++;
}
ISR(INT5_vect)
{
	count2++;
}
ISR(INT6_vect)
{
	count3++;
}
ISR(INT7_vect)
{
	count4++;
}


void pwm1(unsigned int D)
{
	sbi(DDRB,5);
	TCCR1A=0xC1;
	TCCR1B=0x09;  // fast pwm mode 5 no prescaling inverted pwm
	OCR1AH=0;
	OCR1AL=D;
}
void pwm2(unsigned int D)
{
	sbi(DDRE,3);
	TCCR3A=0xC1;
	TCCR3B=0x09;  // fast pwm mode 5 no prescaling inverted pwm
	OCR3AH=0;
	OCR3AL=D;
}
void pwm3(unsigned int D)
{
	sbi(DDRH,3);
	TCCR4A=0xC1;
	TCCR4B=0x09;  // fast pwm mode 5 no prescaling inverted pwm
	OCR4AH=0;
	OCR4AL=D;
}
void pwm4(unsigned int D)
{
	sbi(DDRL,3);
	TCCR5A=0xC1;
	TCCR5B=0x09;  // fast pwm mode 5 no prescaling inverted pwm
	OCR5AH=0;
	OCR5AL=D;
}


void stop1()
{
	cbi(PORTA,0);
	cbi(PORTA,1);
}
void stop2()
{
	cbi(PORTA,2);
	cbi(PORTA,3);
}
void stop3()
{
	cbi(PORTA,4);
	cbi(PORTA,5);
}
void stop4()
{
	cbi(PORTA,6);
	cbi(PORTA,7);
}
 
void forward1()
{
	sbi(PORTA,0);
	cbi(PORTA,1);
}
void forward2()
{
	sbi(PORTA,2);
	cbi(PORTA,3);
}
void forward3()
{
	sbi(PORTA,4);
	cbi(PORTA,5);
}
void forward4()
{
	sbi(PORTA,6);
	cbi(PORTA,7);
}


void reverse1()
{
	sbi(PORTA,1);
	cbi(PORTA,0);
}
void reverse2()
{
	sbi(PORTA,3);
	cbi(PORTA,2);
}
void reverse3()
{
	sbi(PORTA,5);
	cbi(PORTA,4);
}
void reverse4()
{
	sbi(PORTA,7);
	cbi(PORTA,6);
}

void mapX(x)
{
	stop2();
	stop4();
	if(count1>300 || count3>300)
	{
		stop1();
		stop3();
	}
	else
	{
		forward1();
		forward3();
		pwm1(130);
		pwm3(130);
	}
}
void mapY(y)
{
	stop1();
	stop3();
	if(count2>y*ppm || count4>y*ppm)
	{
		stop2();
		stop4();
	}
	else
	{
		forward2();
		forward4();
		pwm2(255);
		pwm4(255);
	}
}
void mapXY(x,y)
{
	unsigned char flag=0;
	if((count1>(x*ppm))|| (count3>(x*ppm)))
	{
		stop1();
		stop3();
		stop2();
		stop4();
		flag=1;
	}
	else
	{
		forward1();
		pwm1(255);
		pwm3(255);
		forward3();
		stop2();
		stop4();
	}
	if((count2>(y*ppm))&&(count4>(y*ppm))&& flag==1)
	{
		stop2();
		stop4();
		stop3();
		stop1();
	}
	else if(flag==1)
	{
		forward2();
		forward4();
		pwm2(255);
		pwm4(255);
		stop3();
		stop1();
	}
	else 
	{
		
	}
}
void mapX_Y(x,y)
{
	unsigned int d;
	if ((count2>(y*ppm))&&(count4>(y*ppm)))
	{
		stop1();
		stop2();
		stop3();
		stop4();
	}
	else
	{
		forward1();
		forward2();
		forward3();
		forward4();
		pwm1(d);
		pwm2(d);
		pwm3((y/x)*d);
		pwm4((y/x)*d);
	}
}

 int main()
{
	sei();
	// count1 =0;
	// count2 =0;
	// count4 =0;
	// count3 =0;
	DDRA=0xFF;		//direction bits
	EIMSK=0xF0;		//interrupts enabled
	EICRB=0xAA;		//all interrupts are set as falling edge
	DDRC=0xFF;
	while(1)
	{
	stop2();
	stop4();
	if(count3>300 )
	{
		stop1();
		stop3();
	}
	else
	{
		forward1();
		forward3();
		pwm1(130);
		pwm3(130);
	}
		//mapX(2);
//		forward1();
	}			
}