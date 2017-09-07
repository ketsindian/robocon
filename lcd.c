/*
 * lomotion2560.c
 *
 * Created: 01-12-2016 13:18:37
 * Author : UD&PJ
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

 void LCD_command(unsigned char ); //Function to pass command to the LCD
void LCD_data(unsigned char ); //Function to write character to the LCD

void lcd_IMU(unsigned int i)
{

    // unsigned char d;
    //unsigned int i;
    // unsigned int x;
    //unsigned char msg[]="parshu la aarchi bhetlich pahije";
    int j;
    unsigned char p[5];
    DDRA = 0xFF;
    DDRF =0x00; //adc port
    DDRC=0xFF;
    DDRJ=0xFF;    //Configuring PORTJ as output
        LCD_command(0x38);
        LCD_command(0x01);
        LCD_command(0x0E);
        LCD_command(0x0C);
        LCD_command(0xC0);

    _delay_ms(10);
                       
for(j=2;j>=0;j--)
    {
      p[j]=i%0x0A;
      p[j]=p[j]+0x30;
      i=i/0x0A;
    }
	LCD_data(p[0]);
    LCD_data(p[1]);
    LCD_data(p[2]);    
    // LCD_data(p[3]);
    _delay_ms(100);
    // LCD_data('c');tttttttttttttttttttttt
    // LCD_data('m');
     LCD_command(0xC0);
}

void lcd_compass(unsigned int i)
{

    unsigned char d;
	//unsigned int i;
	unsigned int x;
	//unsigned char msg[]="parshu la aarchi bhetlich pahije";
	int j;
	unsigned char p[5] ;
	DDRA = 0xFF;
	DDRF =0x00; //adc port
	DDRC=0xFF;
	DDRJ=0xFF;
        LCD_command(0x38);
        LCD_command(0x01);
        // LCD_command(0x0E);
        LCD_command(0x0C);
        LCD_command(0x84);


	for(j=2;j>=0;j--)
	{
	  p[j]=i%0x0A;
	  p[j]=p[j]+0x30;
	  i=i/0x0A;
	}
	_delay_ms(10);
    	               //Configuring PORTJ as output
    LCD_data(p[0]);
    LCD_data(p[1]);
    LCD_data(p[2]);
    // LCD_data(p[3]);
    _delay_ms(100);
    // LCD_data(0xA7);
    // LCD_data('m');
     LCD_command(0x84);
     


}
 
void LCD_command(unsigned char cmd)
{
	PORTC=cmd;
	PORTJ=0x00;
	PORTJ=(1<<PJ2);
	_delay_ms(10);
        PORTJ=0x00;
}

void LCD_data(unsigned char e)
{
	PORTC=e;
	PORTJ=0x00;
	PORTJ=((1<<PJ0)|(1<<PJ2));
	_delay_ms(10);
	 PORTJ=0x00;
}