/*
 * MotorPortA.c
 *
 * Created: 7/15/2016 9:10:28 AM
 *  Author: Asus laptop
 */ 
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
int count =0;

void motor_init(void)
{
	DDRA  = DDRA  | 0xFF;
	//PORTA = PORTA | 0x00;
	
 }
 void encoder_init(void)
 {
	DDRD=0x00;
	PORTD=1<<2;//PULL UP to interrupt
 }

void forward(void)
{
	PORTA = PORTA |0x02;

}	
void stop(void)
{
	PORTA= 0x00;
}
ISR(INT0_vect)
{
	count++;
}

int main(void)
{
	motor_init();
	encoder_init();
	GICR=1<<INT0;
	sei();
	while(1)
	{
		if (count > 2000)
		{
			stop();
		}
		else
		{
			DDRB=0x08;
			TCCR0=0x73;
			//TODO:: Please write your application code 
			OCR0=127;
			forward();
		}
	}
}