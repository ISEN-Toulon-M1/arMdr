void ConfigADC()
{
  /*activation de l'horloge sur l'ADC*/
  RCC->APB2ENR |= (1<<9);
  /*activation de la HSI CLOCK*/
  RCC->CR |= (1<<0);
  /*choix du prescalaire*/
  ADC->CCR =1;
}
 
uint16_t read_ADC(uint16_t chan)
{  
    uint16_t conv;
    ADC1->CR2 &= ~(1<<0);
  /*1 seul  canal de conversion*/
  ADC1->SQR1 &=(~((1<<24)|(1<<23)|(1<<22)|(1<<21)));
    ADC1->SQR5 = 0x00;
    ADC1->SQR5 |=chan;
    /*Routage interfacing*/
    /* Création Lien PC0 à l'ADC*/
    RI->ASCR1 |=(1<<10);
    /*activation de l'horloge sur le RI (COMPEN)*/
    RCC->APB1ENR |= (1<<31);
    
   
    /*activation de l'adc*/
    ADC1->CR2 |= (1<<0);
   
    /*tant que le periph n'est pas pret j' attends*/
    while((ADC1->SR &(1<<6)) == 0);
    /*lancement conversion*/
    ADC1->CR2 |= (1<<30);
    /*tant que la conversion n'est pas finie j' attends*/
     while((ADC1->SR &(1<<1)) == 0);
    conv= ADC1->DR;
    return conv;
   
}
