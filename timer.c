void ConfigTIM2 ()			
{
    NVIC->ISER[0] |= 1<<28;                     //TIM2_IRQHandler = 28
    RCC->APB1ENR |= 1<<0;                       //configure l'horloge de TIM2
    TIM2->PSC = 1;                            //Configurer TIM2 avec : PSC+1 * ARR+1 = Fclk/Fctn avec Fclk = 16MHz
    TIM2->ARR = 399;                           
    TIM2->CR1 |= 0x00;                          //"Eteint" le TIMER
    TIM2->DIER |= 0x01;                         //Autorise l'interruption
}

void TIM2_IRQHandler (void)
{
    TIM2->SR &= ~(0x01);
     if (i == 1)
      {
        i = 0;
      }
      else
      {        
        i = 1;
      }
}
