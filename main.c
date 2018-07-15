#ifndef F_CPU
// 16 MHz clock speed
#endif

#define D0 eS_PORTD0
#define D1 eS_PORTD1
#define D2 eS_PORTD2
#define D3 eS_PORTD3
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include "lcd.h"
#include<string.h>

#define KEY_PRT 	PORTA
#define KEY_DDR		DDRA
#define KEY_PIN		PINA

unsigned char cst[1];
unsigned char keypad[4][4] = {	{'1','4','7','*'},
{'2','5','8','0'},
{'3','6','9','#'},
{'A','B','C','D'}};

unsigned char colloc, rowloc;

void intefaceMotor()
{
	int period;
	DDRB = 0x0F;		/* Make PORTD lower pins as output */
	period = 10;		/* Set period in between two steps */
	while (1)
	{
		/* Rotate Stepper Motor clockwise with Half step sequence */
		for(int i=0;i<60;i++)
		{
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x08;
			_delay_ms(period);
			PORTB = 0x0C;
			_delay_ms(period);
			PORTB = 0x04;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x02;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x01;
			_delay_ms(period);
		}
		PORTB = 0x09;		/* Last step to initial position */
		//_delay_ms(period);
		_delay_ms(1000);

		/* Rotate Stepper Motor Anticlockwise with Full step sequence */
		for(int i=0;i<70;i++)
		{
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0C;
			_delay_ms(period);
		}
		PORTB = 0x09;
		_delay_ms(period);
		_delay_ms(1000);
		break;
	}
}

char keyfind()
{
	while(1)
	{
		KEY_DDR = 0xF0;           /* set port direction as input-output */
		KEY_PRT = 0xFF;

		do
		{
			KEY_PRT &= 0x0F;      /* mask PORT for column read only */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F); /* read status of column */
		}while(colloc != 0x0F);
		
		do
		{
			do
			{
				_delay_ms(20);             /* 20ms key debounce time */
				colloc = (KEY_PIN & 0x0F); /* read status of column */
				}while(colloc == 0x0F);        /* check for any key press */
				
				_delay_ms (40);	            /* 20 ms key debounce time */
				colloc = (KEY_PIN & 0x0F);
			}while(colloc == 0x0F);

			/* now check for rows */
			KEY_PRT = 0xEF;            /* check for pressed key in 1st row */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 0;
				break;
			}

			KEY_PRT = 0xDF;		/* check for pressed key in 2nd row */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 1;
				break;
			}
			
			KEY_PRT = 0xBF;		/* check for pressed key in 3rd row */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 2;
				break;
			}

			KEY_PRT = 0x7F;		/* check for pressed key in 4th row */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 3;
				break;
			}
		}

		if(colloc == 0x0E)
		return(keypad[rowloc][0]);
		else if(colloc == 0x0D)
		return(keypad[rowloc][1]);
		else if(colloc == 0x0B)
		return(keypad[rowloc][2]);
		else
		return(keypad[rowloc][3]);
	}

	int main(void)
	{
		
		while(1){
			
			
		DDRB = 0b00010000;
		DDRD = 0xFF;
		DDRC = 0xFF;
		Lcd8_Init();
		
		Lcd8_Set_Cursor(0,1);
		Lcd8_Write_String("enter your pin");
		Lcd8_Set_Cursor(2,0);
		
		
		int key=0;
		char c;
		char str[8];
		char password[8]={"456B456B"};	
			
		while(1)
		{
			 c=keyfind();
			if(c)
			{
				Lcd8_Write_Char(c);       /* Display which key is pressed */
				str[key]=c;
				++key;
				str[key]='\0';
				if(key==8)
				{
					break;
				}
			}
			
			
			
		}
		Lcd8_Clear();
		Lcd8_Set_Cursor(2,0);
		int k,flag1=0;
		for(k=0;k<8;k++)
		{
			if(str[k]!=password[k])
			{
				
				flag1=1;
				
				break;
			}
		}
		
		if(flag1==0)
		{
			Lcd8_Write_String("OK");
			Lcd8_Set_Cursor(2,4);
			Lcd8_Write_String(str);
			PORTB=0b00010000;
			_delay_ms(500);
			PORTB=0;
			//intefaceMotor();
			
		}
		else
		{
			Lcd8_Write_String("wrong ");
			Lcd8_Set_Cursor(2,6);
			Lcd8_Write_String(str);
			PORTB=0b00010000;
			_delay_ms(200);
			PORTB=0;
			_delay_ms(200);
			PORTB=0b00010000;
			_delay_ms(200);
			PORTB=0;
			_delay_ms(200);
			PORTB=0b00010000;
			_delay_ms(200);
			PORTB=0;
			//Lcd8_Set_Cursor(2,8);
			
		}
		flag1=1;
		_delay_ms(5000);
		Lcd8_Clear();
		}
		
		
	}
