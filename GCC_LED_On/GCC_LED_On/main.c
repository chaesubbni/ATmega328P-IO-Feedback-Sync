/*
 * GCC_LED_On.c
 *
 * Created: 2026-05-07 오전 2:43:05
 * Author : smart
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h> // _delay_ms() 함수 사용할려고.

int main(void)
{
	// 출력 모드
    DDRB |= (1 << DDB5);
	
    while (1) 
    {
		// LED 켜기
		PORTB |= (1 << PORTB5);
		_delay_ms(500);
		// LED 끄기
		PORTB &= ~(1 << PORTB5);
		_delay_ms(2000);
    }
}

