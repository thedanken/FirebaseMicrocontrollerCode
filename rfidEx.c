/**
 *    Keil project for MFRC522 RFID Reader
 *
 *    @author     Tilen Majerle
 *    @email        tilen@majerle.eu
 *    @website    http://stm32f4-discovery.net
 *    @ide        Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_mfrc522.h"
 
#include <stdio.h>

void TIM2_Config(void);
void USART2_init(void);
void USART2_print(char s[]);
void USART2_write (int  ch);
void delayMs(int n);
 
int main(void) {
    //Recognized card ID
    uint8_t CardID[4];
    //My cards id
    //I read them with program below, and write this here
    uint8_t MyID[4] = {
        0xe3, 0xdf, 0xa6, 0x2e   //My card on my keys
    };
    char buffer[50];
 
    //Initialize system
    SystemInit();
    //Initialize delay
    TIM2_Config();
    //Initialize LCD
    USART2_init();
    //Initialize MFRC522 RFID
    TM_MFRC522_Init();

    while (1) {
			USART2_print("Hello World");
			char buf[64];
        //If any card detected
        if (TM_MFRC522_Check(CardID) == MI_OK) {
            //CardID is valid
            
            //Check if this is my card
            if (TM_MFRC522_Compare(CardID, MyID) == MI_OK) {
								sprintf(buf, "Card Matches!\n");
								USART2_print(buf);						
            } else {
                sprintf(buf, "Not a match\n");
								USART2_print(buf);
            }
            //Print on LCD
  					sprintf(buf, "Card Detected      ");
						USART2_print(buf);
            sprintf(buf, "0x%02x\n0x%02x\n0x%02x\n0x%02x\n", CardID[0], CardID[1], CardID[2], CardID[3]);
            USART2_print(buf);
        } else {
            //Some printing to delete content
            sprintf(buf, "Card Not Detected");
						USART2_print(buf);
					  sprintf(buf, "             ");
						USART2_print(buf);
            sprintf(buf, "    \n    \n    \n    \n    ");
						USART2_print(buf);
        }
    }
}
void TIM2_Config(){
	//configure TIM2 to wrap around at 1Hz
	RCC->APB1ENR |= 1;			//enable TIM2 clock
	TIM2->PSC = 1600 - 1;   //divided by 1600
	TIM2->ARR = 10 - 1;			//divided by 10 = 1kHz
	TIM2->CNT = 0;					//clear counter
	TIM2->CR1 = 1;					//enable TIM2
}

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void USART2_init (void) {
    RCC->AHB1ENR |= 1;          /* Enable GPIOA clock */
    RCC->APB1ENR |= 0x20000;    /* Enable USART2 clock */

    /* Configure PA2 for USART2_TX */
    GPIOA->AFR[0] &= ~0x0F00;
    GPIOA->AFR[0] |=  0x0700;   /* alt7 for USART2 */
    GPIOA->MODER  &= ~0x0030;
    GPIOA->MODER  |=  0x0020;   /* enable alternate function for PA2 */

    USART2->BRR = 0x0683;       /* 9600 baud @ 16 MHz */
    USART2->CR1 = 0x0008;       /* enable Tx, 8-bit data */
    USART2->CR2 = 0x0000;       /* 1 stop bit */
    USART2->CR3 = 0x0000;       /* no flow control */
    USART2->CR1 |= 0x2000;      /* enable USART2 */
}
/* Write a string to USART2 */
void USART2_print (char s[]) {
  int i = 0;
	while(s[i] != NULL){
		USART2_write(s[i]);
		i++;
	}
}
/* Write a character to USART2 */
void USART2_write (int  ch) {
   while (!(USART2->SR & 0x0080)) {}   // wait until Tx buffer empty
   USART2->DR = (ch & 0xFF);
}
void delayMs(int n){
	while(n > 0){
		while(!(TIM2->SR & 1)) {};
		TIM2->SR &= ~1;
		n--;
	}
}

