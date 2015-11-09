#include "stm32l1xx_nucleo.h"
#define LUT_size 51 
uint16_t coeffs[LUT_size+1]={1024,1152,1279,1401,1517,1626,1725,1813,1889,1951,1998,2030,2046,2046,2030,1998,1951,1889,1813,1725,1626,1517,1401,1279,1152,1024,896,769,647,531,422,323,235,159,97,50,18,2,2,18,50,97,159,235,323,422,531,647,769,896,1024};
float coeffsFIR[11]={-0.0051  , -0.0000  ,  0.0419  , -0.0000 ,  -0.2885  ,  0.4968  , -0.2885,-0.0000


   , 0.0419 ,  -0.0000 ,  -0.0051


};

uint16_t bufferE[4096];
uint16_t bufferS[4096];
uint32_t i = 0;
int etat=0;

void LED_B_OFF(int n) {
     (GPIOB->ODR) &= ~(1<<n); //eteindre la LED n
}	 
void LED_B_INIT(int n) {
     RCC->AHBENR |= RCC_AHBPeriph_GPIOB; //Active le bus AHBENR 
     (GPIOB->MODER) |= (1<<2*n); // pour changer le mode de fonctionnement de la pine pine pine
	 //GPIOB->OTYPER &= (~(1<<15)); //ne pas changer la valeur apr defaut pour le type
     (GPIOB->OSPEEDR) |= (1<<2*n+1); // pour changer la vitesse
	  //ne pas changer la valeur par defaut du No PU No PD
     LED_B_OFF(n);
  }
void LED_B_ON(int n) {
    (GPIOB->ODR) |= (1<<n); //Allumer la LED
}

void LEDS_B_INIT() {
	LED_B_INIT(1);
	LED_B_INIT(2);
	LED_B_INIT(10);
        LED_B_INIT(11);
}

void initialise_buffer()
{
  for(i=0; i<4096; i++)
  {
    bufferE[i]=0;
    bufferS[i]=0;
  }
}

uint16_t read_ADC()
{  
    uint16_t val;
     
    /*lancer la conversion*/
    ADC1->CR2 |= (1<<30);
    /*Attendre la conversion*/
     while((ADC1->SR &(1<<1)) == 0);
     
    val= ADC1->DR;
    
    return val;
   
}

void INIT_IT() {
RCC->AHBENR|=1; //  Active GPIO A 
RCC->APB2ENR|=(1<<1); // Active le SYSVFG
//---- GPIO A ----
GPIOA->MODER|=0;
//GPIOA->PUPDR|=0;
//-----
SYSCFG->EXTICR[2]|=0; // On travaille sur PA
EXTI->IMR|=(1<<11); // Démasque le 11ième bit
EXTI->FTSR|=0; // Activation au front montant
EXTI->RTSR|=(1<<11); // Désactive les fronts descendants
NVIC->ISER[1]=0x100;
}

void EXTI15_10_IRQHandler(void) {
  EXTI->PR|=(1<<11);
etat == 3 ? etat=0 : etat++ ;
printf("%d\n", etat);
}
void TIM2_IRQHandler (void)
{
    TIM2->SR &= ~(0x01);   
    bufferE[i%4096]= read_ADC();
    switch(etat)
    {
    case 0 : DAC->DHR12R1= bufferE[i%4096];
              LED_B_ON(1);
              LED_B_OFF(2);
              LED_B_OFF(10);
              LED_B_OFF(11);
      break;
    case 1 : DAC->DHR12R1= bufferE[i%4096] + bufferE[(i-1200)%4096] ;  // Double
              LED_B_ON(2);
              LED_B_OFF(1);
              LED_B_OFF(10);
              LED_B_OFF(11);
      break;
    case 2 : DAC->DHR12R1= bufferE[i%4096]+0.7*bufferS[(i-2400)%4096];   // Echo
             bufferS[i%4096]=DAC->DOR1;   
             LED_B_ON(10);
             LED_B_OFF(2);
              LED_B_OFF(1);
              LED_B_OFF(11);
      break;
    case 3 : 
              float tmp=0;
              for(int j=0;j<5;j++)
              {
                tmp+= (float)bufferE[(i)%4096]*coeffsFIR[(j)%4096];   // FIR 10khz
              }
             DAC->DHR12R1=tmp;
             LED_B_ON(11);
             LED_B_OFF(2);
             LED_B_OFF(10);
             LED_B_OFF(1);
             TIM2->SR &= ~(0x01); 
             break;
    }
    i++;
    }

void ConfigADC()
{
  /* PC0 en analogique*/
  GPIOC->MODER=0x03;
  /* Activation de l'horloge sur l'ADC*/
  RCC->APB2ENR |= (1<<9);
  /*activation de la HSI CLOCK*/
  RCC->CR |= (1<<0);
  /*Prescalaire*/
  ADC->CCR &=(~((1<<16)|(1<<17)));
  /*activation de l'horloge sur le RI */
  RCC->APB1ENR |= (1<<31);
  
    /*1 seul  canal de conversion*/
    ADC1->SQR5 |=0x0A;
    /*Routage interfacing*/
    /* Création Lien PC0 à l'ADC*/
    RI->ASCR1 |=(1<<10);
  
  
    /*activation de l'adc*/
    ADC1->CR2 |= (1<<0);
    /*Attendre ADON*/
    while((ADC1->SR &(1<<6)) == 0);
   
}
 

/*void TIM2_IRQHandler (void)
{
    TIM2->SR &= ~(0x01);   
    bufferE[i%4096]= read_ADC();
    //DAC->DHR12R1= bufferE[i%4096];
    //DAC->DHR12R1= bufferE[i%4096] + bufferE[(i-1200)%4096] ;  // Double
    DAC->DHR12R1= bufferE[i%4096]+0.7*bufferS[(i-1400)%4096];   // Echo
    bufferS[i%4096]=DAC->DOR1;                                  // Echo
    i++;
}*/
void configure_dac ()
{
  RCC->APB1ENR |= (1<<29);
  GPIOA->MODER |= ((1<<9) | (1<<8));
  /* Activation du Buffer */
  DAC->CR = (1<<1);
  /* Activation du DAC */
  DAC->CR = (1<<0);
}
void ConfigTIM2 ()			//CTN represente la valeur en Khz voulu
{
    NVIC->ISER[0] |= 1<<28;                     //TIM2_IRQHandler = 28
    RCC->APB1ENR |= 1<<0;                       //configure l'horloge de TIM2
    TIM2->PSC = 1;                            //Configurer TIM2 avec : PSC+1 * ARR+1 = Fclk/Fctn avec Fclk = 16MHz
    TIM2->ARR = 399;                           
    TIM2->CR1 |= 0x00;                          //"Eteint" le TIMER
    TIM2->DIER |= 0x01;                         //Autorise l'interruption
}
int main (void)
{
    i = 0;  
    initialise_buffer();
    INIT_IT();
    LEDS_B_INIT();
    ConfigTIM2();
    ConfigADC();
    configure_dac();
    TIM2->CR1 |= 0x01;
    
    while (1)                                   //Boucle Infini
    {      
    }
}
