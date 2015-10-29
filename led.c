//Created to initialize a LED on the ISEN32 board
#include "stm32l1xx_nucleo.h"
void LED_B_INIT(int n) {
     RCC->AHBENR |= RCC_AHBPeriph_GPIOB; //Activate AHBENR bus
     (GPIOB->MODER) |= (1<<2*n); // pour changer le mode de fonctionnement de la pine pine pine
	 //GPIOB->OTYPER &= (~(1<<15)); //Do not change, this is the default value
     (GPIOB->OSPEEDR) |= (1<<2*n+1); // To change the speed
	  //Do not change the default value of No PU No PD
     LED_B_OFF(n);
  }
  
  void LED_B_OFF(int n) {
     (GPIOB->ODR) &= ~(1<<n); //To swtich off the nth LED
}

void LED_B_ON(int n) {
    (GPIOB->ODR) |= (1<<n); //To switch on the nth LED
}