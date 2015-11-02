void config_TIMX()
{
  RCC->APB1ENR |=1<<0; //cfg timer
  TIM2->CR1 &=~(0xFE);
  TIM2->PSC=1;
  TIM2->ARR=399;
  //configuration interne 
  TIM2->CR1|=1<<0;
  TIM2->SR |=1<<0;
  while((TIM2->SR & 1<<0)!=1<<0){};
  TIM2->SR &=~(0x01);
  TIM2->CR1 &=~(0x01);
}
