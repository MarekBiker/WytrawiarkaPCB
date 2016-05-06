/*
* ProjektTEMP.c
*
* Created: 11.02.2016 22:29:16
* Author : in¿. Marek Czapiewski
*/

//// DEFINICJE ////
#define histereza 1
#define max_licznik 15

//// BIBLIOTEKI ////
#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
#include <stdio.h>
#include "ds18b20.h"
#include <stdlib.h>

int main(void)
{
	double temp = 0;
	char bufor[16];
	char bufor2[16];
	int zadana = 40;
	int licznik_odczytow = 0;
	short grzanie = 0;
	short p_start = 0;
	short p_start_old = 0;
	//USTAWIENIE PORTU B JAKO WYJSCIE//
	DDRB = 0b11111111;
	//USTAWIENIE PORTU C JAKO WEJSCIE//
	DDRC = 0b0000000;
	PORTC = 0b111111;
	LCD_Initalize();
	
	while (1)
	{
		//WARTOSC<<BIT
		//PORTB |= 1<<0;
		//PORTB &= 0<<0;
		//// ODCZYT STANU PRZYCISKÓW I TEMPERATURY ////
		p_start = !(PINC & (1<<3)) & 0x01;
		if (!(PINC & (1<<5))) {
			zadana = zadana + 1;
		}
		if (!(PINC & (1<<4))) {
			zadana = zadana - 1;
		}
		if ( (p_start == 1) & (p_start_old == 0) ) {
			grzanie = !grzanie;
		}
		p_start_old = p_start;
		if (licznik_odczytow == max_licznik) {
			temp = ds18b20_gettemp();
			dtostrf(temp,5,1,bufor);
		}
		licznik_odczytow++;
		if(licznik_odczytow > max_licznik) {
			licznik_odczytow = 0;
		}
		if (grzanie == 1) {
			if (temp > zadana + histereza) {
				PORTB &= 0<<7;
				PORTB |= 1<<0;
			}
			if (temp < zadana - histereza) {
				PORTB |= 1<<7;
				PORTB |= 1<<0;
			}
		}
		else {
			PORTB &= 0<<0;
			PORTB &= 0<<7;
		}
		
		LCD_Clear();
		LCD_Home();
		sprintf(bufor2,"Z:%d%cC A:     %cC",zadana,0xdf,0xdf);
		LCD_WriteText(bufor2);
		LCD_GoTo(9,0);
		LCD_WriteText(bufor);
		LCD_GoTo(0,1);
		LCD_WriteText("Grzanie:");
		LCD_GoTo(10,1);
		if (grzanie == 1) {
			LCD_WriteText("WL");
		}
		else {
			LCD_WriteText("WYL");
		}
		_delay_ms(140);
		
	}
}

