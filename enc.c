/*
 * MotorPortA.c
PWM at PB3 - timer 0
interrupt_0 at PD2
interrupt_1 at PD3
Direction1 on PA0 and PA1
Direction2 on PA2 and PA3
 */ 
#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "UART.c"

#define sbi(x,y)  x|=(1<<y)     //set bit
#define cbi(x,y)  x&=~(1<<y)    //clear bit


#define BAUD 9600
	#define UBRR_pre ((F_CPU/(16UL*BAUD))-1)

	void UART_init(unsigned int ubrr)
	{
	UBRRH =(unsigned char) ubrr>>8;
	UBRRL =(unsigned char) ubrr;

	UCSRB |= (1<<TXEN | 1<<RXEN);

	UCSRC |=(1<<URSEL | 1<<UCSZ1 | 1<<UCSZ0);
	UCSRC &= ~(1 << USBS);
	}

	void UART_transmit(unsigned char data)
	{
	while(! (UCSRA &(1<<UDRE)));
	UDR=data;
	// while(!( UCSRA &(1<<TXC)));
	// _delay_ms(500);
	}

	unsigned char UART_receive()
	{
	while(! (UCSRA & (1<<RXC)));
	return UDR;
	}


unsigned char count1 =0;
unsigned char count2 =0;
	
//unsigned char V00,V11,H00,H11,C00,C01,C10,C11;
	

void up()
{
	sbi(PORTA,0);	//PORTA = 0x02;
	cbi(PORTA,1);  
}	
void forward()
{
	sbi(PORTA,2);	//PORTA = 0x02;
	cbi(PORTA,3);
}
void down()
{
	cbi(PORTA,0); //PORTA = 0x01;
	sbi(PORTA,1);
				
}
void reverse()
{
	cbi(PORTA,2); //PORTA = 0x01;
	sbi(PORTA,3);
	
}
void stop_V()
{
     cbi(PORTA,1);	//PORTA= 0x00;
	 cbi(PORTA,0);					
}
void stop_H()
{
	cbi(PORTA,2);	//PORTA= 0x00;
	cbi(PORTA,3);
}

void pwm_V(float D)
{
	sbi(DDRB,3);		//DDRB=0x08;
	TCCR0=0x71;			//no prescaler inverted phase correct pwm
	D=100-D;
	OCR0=255-(D*2.55);
}
void pwm_H( float D)
{
	sbi(DDRD,7);		//DDRB=0x08;
	TCCR2=0x71;			//no prescaler inverted phase correct pwm
	D=100-D;
	OCR2=255-(D*2.55);
}
void pwm_dec_V( float d,  float z)
{
	float x;
	x=count1/z;
	pwm_V(d-x);
}
void pwm_dec_H(float d, float z)
{
	float x,D;
	x=count2/z;
	D=d-x;
	pwm_H(D);
}

ISR(INT0_vect)
{
	count1++;
}
ISR(INT1_vect)
{
	count2++;
}

/*
					horizontal			vertical
					stop				down			1		V00
					stop				up				2		V11
					reverse				stop			3		H00
					forward				stop			4		H11
					reverse				down			5		C00
					reverse				up				6		C01
					forward				down			7		C10
					forward				up				8		C11
*/


#define V00 1
#define V11 2
#define H00 3
#define H11 4
#define C00 5
#define C01 6
#define C10 7
#define C11 8

 int flag = C00;
int main(void)
{
	UART_init(UBRR_pre);
	sbi(DDRA,0);   //DDRA=0xFF   direction pins;
	sbi(DDRA,1);
	sbi(DDRA,2);   //DDRA=0xFF   direction pins;
	sbi(DDRA,3);
	//cbi(DDRD,2);  //DDRD=0x00   interrupt at INT0;	
	sbi(PORTD,2);			//PORTD=0x04  pull up;	
	sbi(PORTD,3);
	sbi(GICR,INT0);			//GICR=1<<INT0;
	sbi(MCUCR,1);			//MCUCR=0x02  falling edge triggered;			
	sei();
	sbi(GICR,INT1);			//GICR=1<<INT0;
	sbi(MCUCR,3);			//MCUCR=0x02  falling edge triggered;
	sei();
	while(1)
	{

		UART_transmit(0xaa);
		_delay_ms(10);

		UART_transmit(count1); //vertical max 794(down)/1049 
		_delay_ms(10);
		UART_transmit(0xff);
		_delay_ms(10);
		UART_transmit(count2); //orizontal max 371(rev) 379(for)
		_delay_ms(10);
		switch (flag)
		{
			case V00 :
			{
				stop_H();
				if(count1>950)
				{
					stop_V();
				}
				else 
				{
					down();
					pwm_dec_V(70,40);					
				}
				break;
			}
			
			case V11:
			{
				
				stop_H();
				
				if(count1<00)
				{
					stop_V();					
				}
				else
				{
					up();
					pwm_dec_V(90,80);
				}
				break;
			}
			case H00:
			{
				
				stop_V();
				if(count2<00)
				{
					stop_H();
				}
				else
				{	
					reverse();
					pwm_dec_H(60,14);
				}
				break;
			}
			case H11:
			{
				stop_V();
				if (count2>350)
				{
					stop_H();
				}
				else
				{
					forward();
					pwm_dec_H(60,14);
				}
				break;
			}
			case C00:
			{	
				if(count2>350)
				{
		
					stop_H();
				}
				else
				{
					reverse();			
					pwm_dec_H(60,14);
				}
				if(count1>900)
				{
					stop_V();
				}
				else
				{
					down();
					pwm_dec_V(70,40);
				}
				break;
			}
			case C01 :
			{
				if(count1<00)
				{
					stop_V();
				}
				else
				{
					up();
					pwm_dec_V(90,80);
				}
				if(count2>371)
				{
					stop_H();
				}
				else
				{
					pwm_dec_H(60,14);
					reverse();
				}
				break;
			}
			case C10:
			{
				if(count1<00)
				{
					stop_V();
				}
				else
				{
					pwm_dec_V(70,40);
					down();
				}
				if(count2>371)
				{
					stop_H();
				}
				else 
				{
					forward();
					pwm_dec_H(60,14);
				}
				break;			
			}

			case C11:
			{
				if(count1>1029	)
				{
					PORTC = 0x02;
					stop_V();
				}
				else
				{
					up();
					pwm_dec_V(90,80);
				}
				if(count2>379)
				{
					PORTC = 0x01;
					stop_H();				
				}
				else
				{
					forward();
					pwm_dec_H(60,14);
				}
				break;
			}
			default:
			{
				stop_H();
				stop_V();
			}
		}
	}
}
