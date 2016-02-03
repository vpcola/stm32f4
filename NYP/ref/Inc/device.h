
#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#ifndef SET_PORT_BIT
#define SET_PORT_BIT(port, bit)		GPIO_WriteBit(port, bit, (BitAction)1)	//(port->ODR) |= bit
#define RESET_PORT_BIT(port, bit)	GPIO_WriteBit(port, bit, (BitAction)0)	//(port->ODR) &= ((~bit)&0xffff)
#endif

#define LED1_ON(n)		GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)(n))
#define LED2_ON(n)		GPIO_WriteBit(GPIOA, GPIO_Pin_12, (BitAction)(n))
#define LED3_ON(n)		GPIO_WriteBit(GPIOA, GPIO_Pin_13, (BitAction)(n))
#define LED4_ON(n)		GPIO_WriteBit(GPIOA, GPIO_Pin_14, (BitAction)(n))
#define LED5_ON(n)		GPIO_WriteBit(GPIOA, GPIO_Pin_15, (BitAction)(n))
#define LED6_ON(n)		GPIO_WriteBit(GPIOC, GPIO_Pin_10, (BitAction)(n))
#define LED7_ON(n)		GPIO_WriteBit(GPIOC, GPIO_Pin_11, (BitAction)(n))
#define LED8_ON(n)		GPIO_WriteBit(GPIOC, GPIO_Pin_12, (BitAction)(n))

void delay(uint32_t dly);

void NVIC_Configuration(void);

void UART_Configuration(void);

void LED_Configuration(void);
void TurnOnLEDs(int8_t LEDs);

int8_t Button_SW1(void);
int8_t Button_SW2(void);

void Buzzer_Configuration(void);
void BuzzerSound(int8_t x);

int16_t ReadMotorEncoder(uint8_t dir);
void ResetMotorEncoders(void);
void Encoder_Configuration(void);

void Sensor_Configuration(void);
void ReadSensor(int16_t *adc);

void Motor_Configuration(void);
void activateMotor(void);
void deactivateMotor(void);
void setLeftPWM(int16_t pwm);
void setRightPWM(int16_t pwm);
