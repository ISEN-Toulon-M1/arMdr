void configure_dac ()
{
/* Activation du Buffer */
DAC->CR1 = (1<<1)

/* Activation du DAC */
DAC->CR1 = (1<<0)

}

void Use_DAC ()
{
  // Charger la valeur de la tension dans DHRx
}
