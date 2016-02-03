
#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#ifndef SET_PORT_BIT
#define SET_PORT_BIT(port, bit)		GPIO_WriteBit(port, bit, (BitAction)1)	//(port->ODR) |= bit
#define RESET_PORT_BIT(port, bit)	GPIO_WriteBit(port, bit, (BitAction)0)	//(port->ODR) &= ((~bit)&0xffff)
#endif

#define LED1_ON(n)		GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)(n))
#define LED2_ON(n)		GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)(n))
#define LED3_ON(n)		GPIO_WriteBit(GPIOB, GPIO_Pin_14, (BitAction)(n))
#define LED4_ON(n)		GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction)(n))
#define LED5_ON(n)		GPIO_WriteBit(GPIOC, GPIO_Pin_6, (BitAction)(n))
#define LED6_ON(n)		GPIO_WriteBit(GPIOC, GPIO_Pin_7, (BitAction)(n))
#define LED7_ON(n)		GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(n))
#define LED8_ON(n)		GPIO_WriteBit(GPIOB, GPIO_Pin_4, (BitAction)(n))

void delay(uint32_t dly);

void NVIC_Configuration(void);

void UART_Configuration(void);

void LED_Configuration(void);
void TurnOnLEDs(int8_t LEDs);
void ToggleLEDs(void);
void AllLedON(void);
void AllLedOFF(void);
void SwitchLed(int);

int8_t Button_SW1(void);
int8_t Button_SW2(void);

void Buzzer_Configuration(void);
void BuzzerSound(int8_t x);

int ReadMotorEncoder(uint8_t dir);
void ResetMotorEncoders(void);
void Encoder_Configuration(void);

void Sensor_Configuration(void);
void ReadSensor(int16_t *adc);

void Motor_Configuration(void);
void activateMotor(void);
void deactivateMotor(void);
void setLeftPWM(int16_t pwm);
void setRightPWM(int16_t pwm);
void setLeftPWM3(int16_t pwm);
void setRightPWM3(int16_t pwm);

void inputMaleHeader_Configuration(void) ;

void PWM3_Configuration(void);

void USART1_IRQHandler(void);
void USART_write(USART_TypeDef* USARTx, volatile char *s);

void ResetButtonStatusFlag(void);

//___________TIM7______________
void InitializeTimer7(void);
void EnableTimerInterrupt_TIM7(void);
volatile int oldTIM2,newTIM2,difTIM2,oldTIM4,newTIM4,difTIM4,CountingRight,CountingLeft,positionRight,positionLeft;

//___________PID______________
void InitializeTimer5(void);
void EnableTimerInterrupt_TIM5(void);
void regulate_machine_speed(void);
float RTvelocity_l,RTvelocity_r;
int button1J,button2L,button3M,button4goL,button5goR;

void Controller(int, int);
