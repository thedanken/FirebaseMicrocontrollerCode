/*
 * main.cpp
 *
 *  Created on: Nov 14, 2022
 *      Author: acaci
 */




/* p2_3.c Turn on or off LED by a switch
 *
 * This program turns on the green LED (LD2) by pressing the user
 * button B1 of the Nucleo board.
 * The user button is connected to PC13. It has a pull-up resitor
 * so PC13 stays high when the button is not pressed.
 * When the button is pressed, PC13 becomes low.
 * The green LED (LD2) is connected to PA5.
 * A high on PA5 turns on the LED.
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */
//PA0 is user button b1
//PG13 is user LED3
#include "stm32f4xx.h"
#include "stdio.h"
//#include "stm32f4xx_nucleo_144.h"


void TIM2_Config();
void delayMs(int n);
void buttonConfig();

void delayMs(int n){
	while(n > 0){
		while(!(TIM2->SR & 1)) {};
		TIM2->SR &= ~1;
		n--;
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
void buttonConfig(){
	#define LED1ON GPIOB->BSRR = 0x00000001
	#define LED1OFF GPIOB->BSRR = 0x00010000
	#define B1Pressed GPIOC->IDR & 0x2000
	
	RCC->AHB1ENR |=  0x6;                 /* enable GPIOB & GPIOC clock */

  GPIOC->MODER &= !0x0C000000;	  /* reset all pins and PC13 as input mode (button 1)*/
	GPIOB->MODER &= !0x00000003;
	GPIOB->MODER |=  0x00000001;			/* reset all pins and PB0 as output mode (LED1)*/

}
int main(void) {
	buttonConfig();
	TIM2_Config();

  while(1) {
		if(B1Pressed){
			LED1OFF;
		}
		else{
			LED1OFF;
		}
  }
}


