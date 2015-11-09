void ConfigADC()
{
  /* PC0 en analogique*/
  GPIOC->MODER=0x03;
  /*activation de la HSI CLOCK*/
  RCC->CR |= (1<<0);
  /*Prescalaire*/
  ADC->CCR &=(~((1<<16)|(1<<17)));
  /*activation de l'horloge sur le RI */
  RCC->APB1ENR |= (1<<31);
}
 
uint16_t read_ADC()
{  
    uint16_t conv;
    ADC1->CR2 &= ~(1<<0);
    /*1 seul  canal de conversion*/
    ADC1->SQR5 |=0x0A;
    /*Routage interfacing*/
    /* Création Lien PC0 à l'ADC*/
    RI->ASCR1 |=(1<<10);
    /*activation de l'horloge sur le RI (COMPEN)*/
    RCC->APB1ENR |= (1<<31);
    /*route PA0 sur ADC_IN0 de l'ADC1*/
    RI->ASCR1|=1<<10;
   
    /*activation de l'adc*/
    ADC1->CR2 |= (1<<0);
   
    /*Attendre ADON*/
    while((ADC1->SR &(1<<6)) == 0);
    /*lancer la conversion*/
    ADC1->CR2 |= (1<<30);
    /*Attendre la conversion*/
     while((ADC1->SR &(1<<1)) == 0);
     
    conv= ADC1->DR;
    
    return conv;
   
}
