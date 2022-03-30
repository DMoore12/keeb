#ifndef __PLETTENBERG_H__
#define __PLETTENBERG_H__

#include "common/phal_L4/usart/usart.h"
#include "common/common_defs/common_defs.h"
#include "common/queue/queue.h"
#include "stm32l432xx.h"

#define MC_MAX_TX_LENGTH 25
#define MC_MAX_RX_LENGTH 77

#define MC_SERIAL_MODE    's'
#define MC_ANALOG_MODE    'p'
#define MC_FORWARD        'f'
#define MC_REVERSE        'r'
#define MC_BREAK          'b'
#define MC_MAX_POWER      'm'
#define MC_INCREASE_ONE   '+'
#define MC_DECREASE_ONE   '-'
#define MC_INCREASE_TENTH 'g'
#define MC_DECREASE_TENTH 'l'

// Motor Controller Constants:
#define CELL_MAX_V 4.2 //May be increased to 4.25 in the future
#define CELL_MIN_V 2.5

typedef struct 
{
    bool is_inverted;
    uint16_t curr_power_x10;
    q_handle_t *tx_queue;
    /* Values Read */
    float voltage;
    bool proper_voltage;
    float phase_current;
    bool is_over_powered;
    float controller_temp;
    float motor_temp;
    //These are integers, and the period of time per measurement is 1 second
    int motor_temp_slope;
    int con_temp_slope;
    uint32_t rpm;
    bool data_valid;
} motor_t;

/**
 * @brief Initializes the motor in serial mode
 */
void mc_init(motor_t *m, bool is_inverted, q_handle_t *tx_queue);
/**
 * @brief Positive power commands motor to move
 *        Negative power commands motor to break
 */
void mc_set_power(float power, motor_t *m);
/**
 * @brief If the motor is currently spinning, this function
 *  sends the command to stop the motor.
 */
void mc_stop(motor_t *m);
/**
 * @brief Reads the data being sent from the motor controller
 */
void mc_parse(char* data, motor_t *m);

#endif