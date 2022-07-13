#include "pwm.h"

#include "MKL25Z4.h"

void sysPwmTPM2CH0(void) {
  // Enable clock TPM2
  SIM->SCGC6 |= (1 << 26);

  // 0 MCGFLLCLK clock (48MHz)
  SIM->SOPT2 &= ~(1 << 16);

  SIM->SOPT2 |= (0b01 << 24);

  TPM2->MOD = TPM2_CH0_MOD;

  TPM2->SC = (0 << 5) |     // CPWMS=TPM2_SC[5]=0 (modo de contagem crescente)
             (0b01 << 3) |  // CMOD=TPM2_SC[4:3]=0b01 (incrementa a cada pulso do LPTPM)
             (0b111 << 0);

  TPM2->CONTROLS[0].CnSC = (0 << 5) |  // MSB =TPM2_C0SC[5]=0
                           (0 << 4) |  // MSA =TPM2_C0SC[4]=0
                           (0 << 3) |  // ELSB=TPM2_C0SC[3]=0
                           (0 << 2);   // ELSA=TPM2_C0SC[2]=0

  TPM2->CONTROLS[0].CnSC = (1 << 5) |  // MSB =TPM2_C0SC[5]=0
                           (0 << 4) |  // MSA =TPM2_C0SC[4]=0
                           (1 << 3) |  // ELSB=TPM2_C0SC[3]=0
                           (0 << 2);   // ELSA=TPM2_C0SC[2]=0
}

void sysPwmTPM2CH1(void) {
  // Enable clock TPM2
  SIM->SCGC6 |= (1 << 26);

  // 0 MCGFLLCLK clock
  SIM->SOPT2 &= ~(1 << 16);

  SIM->SOPT2 |= (0b01 << 24);

  TPM2->MOD = TPM2_CH1_MOD;

  TPM2->SC = (0 << 5) |     // CPWMS=TPM2_SC[5]=0 (modo de contagem crescente)
             (0b01 << 3) |  // CMOD=TPM2_SC[4:3]=0b01 (incrementa a cada pulso do LPTPM)
             (0b111 << 0);

  TPM2->CONTROLS[1].CnSC = (0 << 5) |  // MSB =TPM2_C0SC[5]=0
                           (0 << 4) |  // MSA =TPM2_C0SC[4]=0
                           (0 << 3) |  // ELSB=TPM2_C0SC[3]=0
                           (0 << 2);   // ELSA=TPM2_C0SC[2]=0

  TPM2->CONTROLS[1].CnSC = (1 << 5) |  // MSB =TPM2_C0SC[5]=0
                           (0 << 4) |  // MSA =TPM2_C0SC[4]=0
                           (1 << 3) |  // ELSB=TPM2_C0SC[3]=0
                           (0 << 2);   // ELSA=TPM2_C0SC[2]=0
}

void sysPwmTPM0CH1(void) {
  // Enable clock TPM0
  SIM->SCGC6 |= (1 << 24);
  // 0 MCGFLLCLK clock
  SIM->SOPT2 &= ~(1 << 16);
  SIM->SOPT2 |= (0b01 << 24);
  TPM0->MOD = TPM0_CH1_MOD;

  TPM0->SC = (0 << 5) |     // CPWMS=TPM2_SC[5]=0 (modo de contagem crescente)
             (0b01 << 3) |  // CMOD=TPM2_SC[4:3]=0b01 (incrementa a cada pulso do LPTPM)
             (0b111 << 0);

  TPM0->CONTROLS[1].CnSC = (0 << 5) |  // MSB =TPM2_C0SC[5]=0
                           (0 << 4) |  // MSA =TPM2_C0SC[4]=0
                           (0 << 3) |  // ELSB=TPM2_C0SC[3]=0
                           (0 << 2);   // ELSA=TPM2_C0SC[2]=0

  TPM0->CONTROLS[1].CnSC = (1 << 5) |  // MSB =TPM2_C0SC[5]=0
                           (0 << 4) |  // MSA =TPM2_C0SC[4]=0
                           (1 << 3) |  // ELSB=TPM2_C0SC[3]=0
                           (0 << 2);   // ELSA=TPM2_C0SC[2]=0
}

void ledRedOnWithPWM() {
  TPM2->CONTROLS[0].CnV = 0;
  TPM2->CONTROLS[1].CnV = TPM2_CH1_MOD;
  TPM0->CONTROLS[1].CnV = TPM0_CH1_MOD;
}

void ledGreenOnWithPWM() {
  TPM2->CONTROLS[0].CnV = TPM2_CH0_MOD;
  TPM2->CONTROLS[1].CnV = 0;
  TPM0->CONTROLS[1].CnV = TPM0_CH1_MOD;
}

void ledBlueOnWithPWM() {
  TPM2->CONTROLS[0].CnV = TPM2_CH0_MOD;
  TPM2->CONTROLS[1].CnV = TPM2_CH1_MOD;
  TPM0->CONTROLS[1].CnV = 0;
}

void ledsOffPWM() {
  TPM2->CONTROLS[0].CnV = TPM2_CH0_MOD;
  TPM2->CONTROLS[1].CnV = TPM2_CH1_MOD;
  TPM0->CONTROLS[1].CnV = TPM0_CH1_MOD;
}

void pwmInitModule() {
  sysPwmTPM0CH1();
  sysPwmTPM2CH0();
  sysPwmTPM2CH1();

  // initial status leds
  TPM2->CONTROLS[0].CnV = TPM2_CH0_MOD;
  TPM2->CONTROLS[1].CnV = TPM2_CH1_MOD;
  TPM0->CONTROLS[1].CnV = TPM0_CH1_MOD;
}
