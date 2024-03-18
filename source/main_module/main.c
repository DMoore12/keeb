/* System Includes */
#include "common/bootloader/bootloader_common.h"
#include "common/faults/faults.h"
// #include "common/modules/wheel_speeds/wheel_speeds.h"
#include "common/phal_F4_F7/adc/adc.h"
#include "common/phal_F4_F7/can/can.h"
#include "common/phal_F4_F7/dma/dma.h"
#include "common/phal_F4_F7/gpio/gpio.h"
#include "common/phal_F4_F7/rcc/rcc.h"
#include "common/phal_F4_F7/usart/usart.h"
#include "common/plettenberg/plettenberg.h"
#include "common/phal_F4_F7/usart/usart.h"
#include "common/plettenberg/plettenberg.h"
#include "common/psched/psched.h"
#include "common/queue/queue.h"

/* Module Includes */
#include "car.h"
#include "can_parse.h"
#include "cooling.h"
#include "daq.h"
#include "main.h"


GPIOInitConfig_t gpio_config[] = {
    // Internal Status Indicators
    GPIO_INIT_OUTPUT(ERR_LED_GPIO_Port, ERR_LED_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_OUTPUT(CONN_LED_GPIO_Port, CONN_LED_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_OUTPUT(HEARTBEAT_GPIO_Port, HEARTBEAT_Pin, GPIO_OUTPUT_LOW_SPEED),

    // External Status Indicators
    GPIO_INIT_OUTPUT(BRK_LIGHT_GPIO_Port, BRK_LIGHT_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_OUTPUT(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_INPUT(BRK_BUZZER_STAT_GPIO_Port, BRK_BUZZER_STAT_Pin, GPIO_INPUT_OPEN_DRAIN),
    GPIO_INIT_INPUT(TSAL_LVAL_STAT_GPIO_Port, TSAL_LVAL_STAT_Pin, GPIO_INPUT_OPEN_DRAIN),

    // CAN
    GPIO_INIT_CANRX_PA11,
    GPIO_INIT_CANTX_PA12,

    GPIO_INIT_CAN2RX_PB12,
    GPIO_INIT_CAN2TX_PB13,

    //SPI Peripherals
    GPIO_INIT_SPI1_SCK_PA5,
    GPIO_INIT_SPI1_MISO_PA6,
    GPIO_INIT_SPI1_MOSI_PA7,
    GPIO_INIT_OUTPUT(SDC_CTRL_GPIO_Port, SDC_CTRL_Pin, GPIO_OUTPUT_LOW_SPEED),

    GPIO_INIT_OUTPUT(EEPROM_nWP_GPIO_Port, EEPROM_nWP_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_OUTPUT(EEPROM_NSS_GPIO_Port, EEPROM_NSS_Pin, GPIO_OUTPUT_LOW_SPEED),

    // Shutdown Circuits
    GPIO_INIT_OUTPUT(SDC_CTRL_GPIO_Port, SDC_CTRL_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_OUTPUT(SDC_MUX_S0_GPIO_Port, SDC_MUX_S0_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_OUTPUT(SDC_MUX_S1_GPIO_Port, SDC_MUX_S1_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_OUTPUT(SDC_MUX_S2_GPIO_Port, SDC_MUX_S2_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_OUTPUT(SDC_MUX_S3_GPIO_Port, SDC_MUX_S3_Pin, GPIO_OUTPUT_LOW_SPEED),

    GPIO_INIT_INPUT(SDC_MUX_DATA_GPIO_Port, SDC_MUX_DATA_Pin, GPIO_INPUT_OPEN_DRAIN),

    // HV Bus Information
    GPIO_INIT_ANALOG(V_MC_SENSE_GPIO_Port, V_MC_SENSE_Pin),
    GPIO_INIT_ANALOG(V_BAT_SENSE_GPIO_Port, V_BAT_SENSE_Pin),
    GPIO_INIT_INPUT(BMS_STAT_GPIO_Port, BMS_STAT_Pin, GPIO_INPUT_OPEN_DRAIN),
    GPIO_INIT_INPUT(PRCHG_STAT_GPIO_Port, PRCHG_STAT_Pin, GPIO_INPUT_OPEN_DRAIN),

    // Motor Controllers
    GPIO_INIT_USART2TX_PA2,
    GPIO_INIT_USART2RX_PA3,
    GPIO_INIT_USART1TX_PA9,
    GPIO_INIT_USART1RX_PA10,

    // Wheel Speed
    GPIO_INIT_AF(MOTOR_R_WS_GPIO_Port, MOTOR_R_WS_Pin, MOTOR_R_WS_AF, GPIO_OUTPUT_HIGH_SPEED, GPIO_OUTPUT_OPEN_DRAIN, GPIO_INPUT_PULL_DOWN),
    GPIO_INIT_AF(MOTOR_L_WS_GPIO_Port, MOTOR_L_WS_Pin, MOTOR_L_WS_AF, GPIO_OUTPUT_HIGH_SPEED, GPIO_OUTPUT_OPEN_DRAIN, GPIO_INPUT_PULL_DOWN),

    // Shock Pots
    GPIO_INIT_ANALOG(SHOCK_POT_L_GPIO_Port, SHOCK_POT_L_Pin),
    GPIO_INIT_ANALOG(SHOCK_POT_R_GPIO_Port, SHOCK_POT_R_Pin),

    // Load Sensor
    GPIO_INIT_ANALOG(LOAD_L_GPIO_Port, LOAD_L_Pin),
    GPIO_INIT_ANALOG(LOAD_R_GPIO_Port, LOAD_R_Pin),

    // Thermistor Analog Multiplexer
    GPIO_INIT_OUTPUT(THERM_MUX_S0_GPIO_Port, THERM_MUX_S0_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_OUTPUT(THERM_MUX_S1_GPIO_Port, THERM_MUX_S1_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_OUTPUT(THERM_MUX_S2_GPIO_Port, THERM_MUX_S2_Pin, GPIO_OUTPUT_LOW_SPEED),
    GPIO_INIT_ANALOG(THERM_MUX_D_GPIO_Port, THERM_MUX_D_Pin)
};

/* USART Configuration */
// Left Motor Controller UART
dma_init_t usart_l_tx_dma_config = USART1_TXDMA_CONT_CONFIG(NULL, 1);
dma_init_t usart_l_rx_dma_config = USART1_RXDMA_CONT_CONFIG(NULL, 2);
char usart_l_rx_array[MC_MAX_RX_LENGTH] = {'\0'};
volatile usart_rx_buf_t huart_l_rx_buf = {
    .last_msg_time = 0, .msg_size = MC_MAX_TX_LENGTH,
    .last_msg_loc  = 0, .last_rx_time = 0,
    .rx_buf_size   = MC_MAX_RX_LENGTH, .rx_buf = usart_l_rx_array
};
usart_init_t huart_l = {
   .baud_rate   = 115200,
   .word_length = WORD_8,
   .stop_bits   = SB_ONE,
   .parity      = PT_NONE,
   .hw_flow_ctl = HW_DISABLE,
   .ovsample    = OV_16,
   .obsample    = OB_DISABLE,
   .periph      = USART1,
   .wake_addr = false,
   .usart_active_num = USART1_ACTIVE_IDX,
   .tx_dma_cfg = &usart_l_tx_dma_config,
   .rx_dma_cfg = &usart_l_rx_dma_config
};

// Right Motor Controller UART
dma_init_t usart_r_tx_dma_config = USART2_TXDMA_CONT_CONFIG(NULL, 1);
dma_init_t usart_r_rx_dma_config = USART2_RXDMA_CONT_CONFIG(NULL, 2);
usart_init_t huart_r = {
   .baud_rate   = 115200,
   .word_length = WORD_8,
   .stop_bits   = SB_ONE,
   .parity      = PT_NONE,
   .hw_flow_ctl = HW_DISABLE,
   .ovsample    = OV_16,
   .obsample    = OB_DISABLE,
   .periph      = USART2,
   .wake_addr = false,
   .usart_active_num = USART2_ACTIVE_IDX,
   .tx_dma_cfg = &usart_r_tx_dma_config,
   .rx_dma_cfg = &usart_r_rx_dma_config
};
char usart_r_rx_array[MC_MAX_RX_LENGTH] = {'\0'};
volatile usart_rx_buf_t huart_r_rx_buf = {
    .last_msg_time = 0, .msg_size = MC_MAX_TX_LENGTH,
    .last_msg_loc  = 0, .last_rx_time = 0,
    .rx_buf_size   = MC_MAX_RX_LENGTH, .rx_buf = usart_r_rx_array
};

/* ADC Configuration */
ADCInitConfig_t adc_config = {
    .clock_prescaler = ADC_CLK_PRESC_6,
    .resolution      = ADC_RES_12_BIT,
    .data_align      = ADC_DATA_ALIGN_RIGHT,
    .cont_conv_mode  = true,
    .dma_mode        = ADC_DMA_CIRCULAR,
    .adc_number      = 1
};

// TODO: Update this comment with the correct sample time
/* With 11 items, 16 prescaler, and 640 sample time, each channel gets read every 1.4ms */
volatile ADCReadings_t adc_readings;
ADCChannelConfig_t adc_channel_config[] = {
    {.channel=V_MC_SENSE_ADC_CHNL,     .rank=1,  .sampling_time=ADC_CHN_SMP_CYCLES_480},
    {.channel=V_BAT_SENSE_ADC_CHNL,    .rank=2,  .sampling_time=ADC_CHN_SMP_CYCLES_480},
    {.channel=SHOCK_POT_L_ADC_CHNL,    .rank=3,  .sampling_time=ADC_CHN_SMP_CYCLES_480},
    {.channel=SHOCK_POT_R_ADC_CHNL,    .rank=4,  .sampling_time=ADC_CHN_SMP_CYCLES_480},
    {.channel=THERM_MUX_D_ADC_CHNL,    .rank=5,  .sampling_time=ADC_CHN_SMP_CYCLES_480},
    {.channel=LOAD_L_ADC_CHNL,         .rank=6,  .sampling_time=ADC_CHN_SMP_CYCLES_480},
    {.channel=LOAD_R_ADC_CHNL,         .rank=7,  .sampling_time=ADC_CHN_SMP_CYCLES_480},
    {.channel=INTERNAL_THERM_ADC_CHNL, .rank=8,  .sampling_time=ADC_CHN_SMP_CYCLES_480},
};
dma_init_t adc_dma_config = ADC1_DMA_CONT_CONFIG((uint32_t) &adc_readings,
            sizeof(adc_readings) / sizeof(adc_readings.v_mc), 0b01);

/* SPI Configuration */
// dma_init_t spi_rx_dma_config = SPI1_RXDMA_CONT_CONFIG(NULL, 2);
// dma_init_t spi_tx_dma_config = SPI1_TXDMA_CONT_CONFIG(NULL, 1);

// SPI_InitConfig_t spi_config = {
//     .data_len  = 8,
//     .nss_sw = false,
//     .nss_gpio_port = EEPROM_NSS_GPIO_Port,
//     .nss_gpio_pin = EEPROM_NSS_Pin,
//     .rx_dma_cfg = &spi_rx_dma_config,
//     .tx_dma_cfg = &spi_tx_dma_config,
//     .periph = SPI1
// };




extern uint32_t APB1ClockRateHz;
extern uint32_t APB2ClockRateHz;
extern uint32_t AHBClockRateHz;
extern uint32_t PLLClockRateHz;

#define TargetCoreClockrateHz 144000000
ClockRateConfig_t clock_config = {
    .system_source              =SYSTEM_CLOCK_SRC_PLL,
    .pll_src                    =PLL_SRC_HSI16,
    .vco_output_rate_target_hz  =288000000,
    .system_clock_target_hz     =TargetCoreClockrateHz,
    .ahb_clock_target_hz        =(TargetCoreClockrateHz / 1),
    .apb1_clock_target_hz       =(TargetCoreClockrateHz / 4),
    .apb2_clock_target_hz       =(TargetCoreClockrateHz / 4),
};

/* Function Prototypes */
void preflightAnimation(void);
void preflightChecks(void);
void heartBeatLED();
void usartTxUpdate(void);
void usartIdleIRQ(volatile usart_init_t *huart, volatile usart_rx_buf_t *rx_buf);
void canTxUpdate(void);
void send_fault(uint16_t, bool);
extern void HardFault_Handler();


q_handle_t q_tx_can;
q_handle_t q_rx_can;
uint8_t can_tx_fails; // number of CAN messages that failed to transmit
q_handle_t q_tx_usart_l;
q_handle_t q_tx_usart_r;

uint16_t num_failed_msgs_r;
uint16_t num_failed_msgs_l;


int main(void){
    /* Data Struct Initialization */
    qConstruct(&q_tx_can, sizeof(CanMsgTypeDef_t));
    qConstruct(&q_rx_can, sizeof(CanMsgTypeDef_t));
    can_tx_fails = 0;
    qConstruct(&q_tx_usart_l, MC_MAX_TX_LENGTH);
    qConstruct(&q_tx_usart_r, MC_MAX_TX_LENGTH);

    /* HAL Initialization */
    if(0 != PHAL_configureClockRates(&clock_config))
    {
        HardFault_Handler();
    }
    if(!PHAL_initGPIO(gpio_config, sizeof(gpio_config)/sizeof(GPIOInitConfig_t)))
    {
        HardFault_Handler();
    }
    PHAL_writeGPIO(SDC_CTRL_GPIO_Port, SDC_CTRL_Pin, 1);


    /* Task Creation */
    schedInit(APB1ClockRateHz);
    configureAnim(preflightAnimation, preflightChecks, 60, 750);

    taskCreate(coolingPeriodic, 100);
    taskCreate(heartBeatLED, 500);
    taskCreate(monitorSDCPeriodic, 200);
    taskCreate(carHeartbeat, 100);
    taskCreate(carPeriodic, 15);
    // taskCreate(updateSDCFaults, 400);
    // taskCreate(wheelSpeedsPeriodic, 15);
    taskCreate(heartBeatTask, 100);
    taskCreate(parseMCDataPeriodic, MC_LOOP_DT);
    taskCreate(daqPeriodic, DAQ_UPDATE_PERIOD);
    // taskCreate(memFg, MEM_FG_TIME);
    taskCreateBackground(canTxUpdate);
    taskCreateBackground(canRxUpdate);
    taskCreateBackground(usartTxUpdate);
    // taskCreateBackground(memBg);

    // calibrateSteeringAngle(&i);
    // SEND_LWS_CONFIG(q_tx_can, 0x05, 0, 0); // reset cal
    // SEND_LWS_CONFIG(q_tx_can, 0x03, 0, 0); // start new

    schedStart();

    return 0;
}

void preflightChecks(void) {
    static uint8_t state;

    switch (state++)
    {
        case 0:
            // huart_l.rx_dma_cfg->circular = true;
            if(!PHAL_initUSART(&huart_l, APB2ClockRateHz))
            {
                HardFault_Handler();
            }
            // huart_r.rx_dma_cfg->circular = true;
            if(!PHAL_initUSART(&huart_r, APB1ClockRateHz))
            {
                HardFault_Handler();
            }
            break;
        case 1:
            if(!PHAL_initCAN(CAN1, false))
            {
                HardFault_Handler();
            }
            NVIC_EnableIRQ(CAN1_RX0_IRQn);
            // spi_config.data_rate = APB2ClockRateHz / 16; // 5 MHz
            // if (!PHAL_SPI_init(&spi_config))
            //     HardFault_Handler();
            // if (initMem(EEPROM_nWP_GPIO_Port, EEPROM_nWP_Pin, &spi_config, 1, 1) != E_SUCCESS)
            //     HardFault_Handler();
           break;
        case 2:
            if(!PHAL_initADC(ADC1, &adc_config, adc_channel_config,
                            sizeof(adc_channel_config)/sizeof(ADCChannelConfig_t)))
            {
                HardFault_Handler();
            }
            if(!PHAL_initDMA(&adc_dma_config))
            {
                HardFault_Handler();
            }
            PHAL_startTxfer(&adc_dma_config);
            PHAL_startADC(ADC1);
           break;
        case 3:
            // initial rx request
            PHAL_usartRxDma(&huart_r,
                            (uint16_t *) huart_r_rx_buf.rx_buf,
                            huart_r_rx_buf.rx_buf_size, 1);
            PHAL_usartRxDma(&huart_l,
                            (uint16_t *) huart_l_rx_buf.rx_buf,
                            huart_l_rx_buf.rx_buf_size, 1);
            break;
        case 4:
           /* Module Initialization */
           carInit();
           coolingInit();
           break;
       case 5:
           initCANParse(&q_rx_can);
           if(daqInit(&q_tx_can))
               HardFault_Handler();
            initFaultLibrary(FAULT_NODE_NAME, &q_tx_can, ID_FAULT_SYNC_MAIN_MODULE);
           break;
        default:
            registerPreflightComplete(1);
            state = 255; // prevent wrap around
    }
}

void preflightAnimation(void) {
    static uint32_t time;

    PHAL_writeGPIO(HEARTBEAT_GPIO_Port, HEARTBEAT_Pin, 0);
    PHAL_writeGPIO(ERR_LED_GPIO_Port, ERR_LED_Pin, 0);
    PHAL_writeGPIO(CONN_LED_GPIO_Port, CONN_LED_Pin, 0);

    switch (time++ % 6)
    {
        case 0:
        case 5:
            PHAL_writeGPIO(HEARTBEAT_GPIO_Port, HEARTBEAT_Pin, 1);
            break;
        case 1:
        case 4:
            PHAL_writeGPIO(CONN_LED_GPIO_Port, CONN_LED_Pin, 1);
            break;
        case 2:
        case 3:
            PHAL_writeGPIO(ERR_LED_GPIO_Port, ERR_LED_Pin, 1);
            break;
    }
}

void heartBeatLED(void)
{
    static uint8_t trig;
    // TODO: fix HB LED
    PHAL_toggleGPIO(HEARTBEAT_GPIO_Port, HEARTBEAT_Pin);
    if ((sched.os_ticks - last_can_rx_time_ms) >= CONN_LED_MS_THRESH)
         PHAL_writeGPIO(CONN_LED_GPIO_Port, CONN_LED_Pin, 0);
    else PHAL_writeGPIO(CONN_LED_GPIO_Port, CONN_LED_Pin, 1);

    // Send every other time (1000 ms)
    if (trig) {
        SEND_MCU_STATUS(q_tx_can, sched.skips, (uint8_t) sched.fg_time.cpu_use,
                                           (uint8_t) sched.bg_time.cpu_use,
                                           sched.error, can_tx_fails);
    }
    trig = !trig;
}

/* USART Message Handling */
uint8_t tmp_left[MC_MAX_TX_LENGTH] = {'\0'};
uint8_t tmp_right[MC_MAX_TX_LENGTH] = {'\0'};
void usartTxUpdate(void)
{
    if (!PHAL_usartTxBusy(&huart_l) &&
        qReceive(&q_tx_usart_l, tmp_left) == SUCCESS_G)
    {
        PHAL_usartTxDma(&huart_l, (uint16_t *) tmp_left, strlen(tmp_left));
    }
    if (!PHAL_usartTxBusy(&huart_r) &&
        qReceive(&q_tx_usart_r, tmp_right) == SUCCESS_G)
    {
        PHAL_usartTxDma(&huart_r, (uint16_t *) tmp_right, strlen(tmp_right));
    }
}

void usart_recieve_complete_callback(usart_init_t *handle)
{
    if (handle == &huart_r)
    {
        if (handle->rx_errors.noise_detected)
        {
            num_failed_msgs_r++;
            return;
        }
        usartIdleIRQ(&huart_r, &huart_r_rx_buf);
    }
    else if (handle == &huart_l)
    {
        if (handle->rx_errors.noise_detected)
        {
            num_failed_msgs_l++;
            return;
        }
        usartIdleIRQ(&huart_l, &huart_l_rx_buf);
    }
}



void usartIdleIRQ(volatile usart_init_t *huart, volatile usart_rx_buf_t *rx_buf)
{
    // TODO: check for overruns, framing errors, etc
    uint16_t new_loc = 0;
    rx_buf->last_rx_time = sched.os_ticks;
    new_loc = rx_buf->rx_buf_size - huart->rx_dma_cfg->stream->NDTR;      // extract last location from DMA
    if (new_loc == rx_buf->rx_buf_size) new_loc = 0;                        // should never happen
    else if (new_loc < rx_buf->last_rx_loc) new_loc += rx_buf->rx_buf_size; // wrap around
    if (new_loc - rx_buf->last_rx_loc > rx_buf->msg_size)                   // status msg vs just an echo
    {
        rx_buf->last_msg_time = sched.os_ticks;
        rx_buf->last_msg_loc = (rx_buf->last_rx_loc + 1) % rx_buf->rx_buf_size;
    }
    rx_buf->last_rx_loc = new_loc % rx_buf->rx_buf_size;
}

/* CAN Message Handling */
void canTxUpdate(void)
{
    CanMsgTypeDef_t tx_msg;
    if (qReceive(&q_tx_can, &tx_msg) == SUCCESS_G)    // Check queue for items and take if there is one
    {
        if (!PHAL_txCANMessage(&tx_msg)) ++can_tx_fails;
    }
}

void CAN1_RX0_IRQHandler()
{
    if (CAN1->RF0R & CAN_RF0R_FOVR0) // FIFO Overrun
        CAN1->RF0R &= !(CAN_RF0R_FOVR0);
        CAN1->RF0R &= !(CAN_RF0R_FOVR0);

    if (CAN1->RF0R & CAN_RF0R_FULL0) // FIFO Full
        CAN1->RF0R &= !(CAN_RF0R_FULL0);
        CAN1->RF0R &= !(CAN_RF0R_FULL0);

    if (CAN1->RF0R & CAN_RF0R_FMP0_Msk) // Release message pending
    {
        CanMsgTypeDef_t rx;
        rx.Bus = CAN1;

        // Get either StdId or ExtId
        rx.IDE = CAN_RI0R_IDE & CAN1->sFIFOMailBox[0].RIR;
        if (rx.IDE)
        {
          rx.ExtId = ((CAN_RI0R_EXID | CAN_RI0R_STID) & CAN1->sFIFOMailBox[0].RIR) >> CAN_RI0R_EXID_Pos;
        }
        else
        {
          rx.StdId = (CAN_RI0R_STID & CAN1->sFIFOMailBox[0].RIR) >> CAN_RI0R_STID_Pos;
        }

        rx.DLC = (CAN_RDT0R_DLC & CAN1->sFIFOMailBox[0].RDTR) >> CAN_RDT0R_DLC_Pos;

        rx.Data[0] = (uint8_t) (CAN1->sFIFOMailBox[0].RDLR >> 0)  & 0xFF;
        rx.Data[1] = (uint8_t) (CAN1->sFIFOMailBox[0].RDLR >> 8)  & 0xFF;
        rx.Data[2] = (uint8_t) (CAN1->sFIFOMailBox[0].RDLR >> 16) & 0xFF;
        rx.Data[3] = (uint8_t) (CAN1->sFIFOMailBox[0].RDLR >> 24) & 0xFF;
        rx.Data[4] = (uint8_t) (CAN1->sFIFOMailBox[0].RDHR >> 0)  & 0xFF;
        rx.Data[5] = (uint8_t) (CAN1->sFIFOMailBox[0].RDHR >> 8)  & 0xFF;
        rx.Data[6] = (uint8_t) (CAN1->sFIFOMailBox[0].RDHR >> 16) & 0xFF;
        rx.Data[7] = (uint8_t) (CAN1->sFIFOMailBox[0].RDHR >> 24) & 0xFF;

        CAN1->RF0R |= (CAN_RF0R_RFOM0);
        CAN1->RF0R |= (CAN_RF0R_RFOM0);

        qSendToBack(&q_rx_can, &rx); // Add to queue (qSendToBack is interrupt safe)
    }
}

void main_module_bl_cmd_CALLBACK(CanParsedData_t *msg_data_a)
{
    if (can_data.main_module_bl_cmd.cmd == BLCMD_RST)
        Bootloader_ResetForFirmwareDownload();
}

void HardFault_Handler()
{
    PHAL_writeGPIO(ERR_LED_GPIO_Port, ERR_LED_Pin, 1);
    while(1)
    {
        __asm__("nop");
    }
}