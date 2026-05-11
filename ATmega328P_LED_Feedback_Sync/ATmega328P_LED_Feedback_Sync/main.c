/*
 * Project: ATmega328P-IO-Synchronization-with-NOP
 * Description: 
 * - PB0: 외부 출력 LED (1초마다 토글)
 * - PB5: 내부 피드백 LED (PB0의 실제 핀 상태를 읽어와 동기화)
 * - _NOP()를 통해 전파 지연 문제를 해결함.
 */

#define F_CPU 16000000UL // 16MHz 클록 설정
#include <avr/io.h>
#include <util/delay.h>
#include <avr/cpufunc.h> // _NOP() 함수 사용을 위해 추가

int main(void)
{
	PORTB |= (1 << PORTB0) | (1 << PORTB5); // 출력 1
	DDRB |= (1 << DDB0) | (1 << DDB5); // 0, 5번 핀 출력 모드로.
	
    while (1) 
    {
		PORTB ^= (1 << PORTB0); // 0번 비트 Toggle.
		// 이 코드가 실행되면 PORTx D-FF 입력 D에 값이 저장되고 clk이 끝날때 Q로 업데이트 되면서 다음 명령어를 가져옴.
		_NOP();
		// 이 코드가 실행되면 cpu는 쉬기 때문에 High level 부분에서 Pxn으로 나간 전류를 다시 D-F로 불러와 Q값이 업데이트됨.
		// 그래서 최종적으로 PINx D-FF의 입력 D에 값이 저장됨.
		if(PINB & (1 << PINB0)){
			// 이제 상승 에지에서 PINx D-FF Q값이 업데이트되고 high-level에서 WPx가 활성화 되어 CPU가 데이터를 읽고 계산하는거임.
			PORTB |= (1 << PORTB5);
		}
		else{
			PORTB &= ~(1 << PORTB5);
		}
		_delay_ms(1000);
    }
}

