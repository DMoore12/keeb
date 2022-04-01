#ifndef MAIN_H_
#define MAIN_H_

//STM32L432KCU6

/* Status LEDs */
#define ERR_LED_GPIO_Port   (GPIOB)
#define ERR_LED_Pin         (3)
#define HEARTBEAT_GPIO_Port (GPIOB)
#define HEARTBEAT_Pin       (5)
#define CONN_LED_GPIO_Port  (GPIOB)
#define CONN_LED_Pin        (3)
#define CONN_LED_MS_THRESH  (500)

/* Shock Pots */
#define POT_AMP_LEFT_GPIO_Port  (GPIOA)
#define POT_AMP_LEFT_Pin        (0)
#define POT_AMP_LEFT_ADC_CHNL   (5)
#define POT_AMP_RIGHT_GPIO_Port (GPIOA)
#define POT_AMP_RIGHT_Pin       (1)
#define POT_AMP_RIGHT_ADC_CHNL  (6)

/* Motor Controllers */
#define USART_L (USART2)
#define USART_R (USART1)

#define M_INVERT_FRONT_RIGHT 0
#define M_INVERT_FRONT_LEFT  0
#define M_INVERT_REAR_RIGHT  1
#define M_INVERT_REAR_LEFT   1

#if (FTR_DRIVELINE_FRONT)
#define M_INVERT_RIGHT M_INVERT_FRONT_RIGHT
#define M_INVERT_LEFT  M_INVERT_FRONT_LEFT
#elif (FTR_DRIVELINE_REAR)
#define M_INVERT_RIGHT M_INVERT_REAR_RIGHT
#define M_INVERT_LEFT  M_INVERT_REAR_LEFT
#endif

/* Wheel Speed */
#define WSPEEDL_GPIO_Port (GPIOA)
#define WSPEEDL_Pin       (8)
#define WHEELSPEEDL_AF    (1)
#define WSPEEDR_GPIO_Port (GPIOA)
#define WSPEEDR_Pin       (15)
#define WHEELSPEEDR_AF    (1)

/* EEPROM */
#define WC_GPIO_Port (GPIOA)
#define WC_Pin       (7)
#define I2C          (I2C1)

#endif

