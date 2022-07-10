#ifndef PWM_H_
#define PWM_H_

#define TPM2_CH0_MOD 819
#define TPM2_CH1_MOD 819
#define TPM0_CH1_MOD 819

extern void sysPwmTPM2CH0(void);

extern void sysPwmTPM2CH1(void);

extern void sysPwmTPM0CH1(void);

extern void ledRedOnWithPWM(void);

extern void ledGreenOnWithPWM(void);

extern void ledBlueOnWithPWM(void);

extern void ledsOffPWM(void);

extern void pwmInitModule(void);

#endif /* PWM_H_ */
