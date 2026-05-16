# ATmega328P-IO-Feedback-Sync

- 이 프로젝트는 ATmega328P의 I/O 포트 제어 시 발생하는 물리적 동기화 지연을 분석하고, 이를 소프트웨어적으로 해결하기 위한 _NOP() 명령어의 필요성을 하드웨어 타이밍 관점에서 검증하는 프로젝트입니다.
- 즉, 외부 버튼 입력을 감지하여 LED를 토글하고, 하드웨어 전파 지연을 고려한 _NOP() 동기화 로직을 통해 실제 출력 상태를 피드백 받아 내부 LED(PB5)와 실시간으로 동기화했습니다.



### 1. General Digital I/O 회로도
<img width="787" height="680" alt="image (4)" src="https://github.com/user-attachments/assets/11eaaa01-d146-4ece-aa5d-7452568f7e27" />

- 일반적인 디지털 I/O 회로에서 PORTx(출력)에 쓴 값이 PINx(입력) 레지스터에 안착하기까지 다음 경로를 거침
  - 출발: PORTxn D-FF
  - 경로: 출력 버퍼 -> 실제 외부 핀(Pxn) -> 슈미트 트리거 -> D-L
  - 도착: PINxn D-FF

### 2. 타이밍 분석
<img width="677" height="288" alt="image (5)" src="https://github.com/user-attachments/assets/aa34c1ec-aeda-4abb-875c-62a3592cbb36" />

- out 명령어로 데이터를 내보낸 직후 in 명령어로 읽을 때 발생하는 1클록의 시차 분석
- 명령어 실행 흐름
  - out PORTx, r16
    - CPU가 WRx 신호를 활성화 해 데이터를 PORTxn D-FF 입구에 전달
  - nop
    - 상승 에지에서 PORTxn의 Q값이 업데이트되어 실제 핀으로 전기가 감.
    - 상승 레벨에서 전기가 한 바퀴 돌아와 SYNC LATCH 문이 열리며 데이터가 안착함.
  - in r17, PINx
    - 다음 clk 주기가 시작될 때 상승 에지 때 PINxn D-FF의 Q값이 최신 데이터로 업데이트 됨.
    - 명령어가 실행되어 RPx 신호가 버퍼를 열어 CPU가 최신 데이터를 안정적으로 읽어감.
    - 이제 다음 clk 주기 때 CPU가 r17에 데이터를 write 해 최종적으로 r17에 값이 업데이트 됨. 
  

### 3. 배운 점.
- 이론과 실무의 간극 해소: 대학교 실습 때 왜 핀 값을 읽을 때 가끔 오작동할까 라는 의문을 데이터시트 분석을 통해 해결.
- 최적화된 코드 설계 능력: 무의미한 delay 함수 대신, 정확히 필요한 타이밍만큼만 쉬어주는 _NOP() 명령어를 활용하여 시스템 자원을 낭비하지 않는 최적화된 펌웨어 설계 기술을 익힘.


