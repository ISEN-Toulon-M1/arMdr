void ConfigADC()
{
  /*activation de l'horloge sur l'ADC*/
  RCC->APB2ENR |= (1<<9);
  
  /*PC0 en analogique*/
    GPIOC_MODER = 0x03;
  /*activation de la HSI CLOCK*/
  RCC->CR |= (1<<0);
  /*choix du prescalaire*/
  ADC1->CCR &=(~((1<<16)|(1<<17)));
  /*activation de l'horloge sur le RI (COMPEN)*/
   RCC->APB1ENR |= (1<<31);
}

uint16_t  litADC()
{
  
   uint16_t conv;
      
    ADC1->CR2 &= ~(1<<0);
    ADC1->CR1 &= ~((1<<25)|(1<<24)); //resolution
 
    /*Canal de conversion*/
    ADC1->SQR1 &= ~((1<<24) | (1<<23) | (1<<22) | (1<<21) | (1<<20)) ;
    ADC1->SQR5 |=0xA;
    
    /*Routage interfacing*/
    RI_ASCR1->|=(1<<10);
    
    /*Scan mode*/
    ADC1->CR1&= ~(1<<8);
        
    /* ADON = 1 */
    ADC1->CR2 |= (1<<0);
    /*SI ADON = 0, rien faire */
    while((ADC1->SR &(1<<6)) == 0);
    /*swstart = 1 -> debut de conversion*/
    ADC1->CR2 |= (1<<30);
    /*eoc à 1 -> conversion réalisé */
    while((ADC1->SR &(1<<1)) == 0);  //
    val = ADC1->DR;
    return val;
  
 }
