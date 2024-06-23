// Module includes
#include "main.h"
#include "keyboard.h"

// GPIO configuration
GPIOInitConfig_t gpio_config[] = {
    // Column configuration
    GPIO_INIT_OUTPUT(COL_Port, 0,  GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 1,  GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 2,  GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 3,  GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 4,  GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 5,  GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 6,  GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 7,  GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 8,  GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 9,  GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 10, GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 11, GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 12, GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 13, GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 14, GPIO_OUTPUT_ULTRA_SPEED),
    GPIO_INIT_OUTPUT(COL_Port, 15, GPIO_OUTPUT_ULTRA_SPEED),

    // Row configuration
    GPIO_INIT_INPUT(ROW_Port,  0,  GPIO_INPUT_OPEN_DRAIN),
    GPIO_INIT_INPUT(ROW_Port,  1,  GPIO_INPUT_OPEN_DRAIN),
    GPIO_INIT_INPUT(ROW_Port,  2,  GPIO_INPUT_OPEN_DRAIN),
    GPIO_INIT_INPUT(ROW_Port,  3,  GPIO_INPUT_OPEN_DRAIN),
    GPIO_INIT_INPUT(ROW_Port,  4,  GPIO_INPUT_OPEN_DRAIN),
};

// Clock configuration
ClockRateConfig_t clock_config = {
    .system_source              =  SYSTEM_CLOCK_SRC_PLL,
    .pll_src                    =  PLL_SRC_MSI,
    .msi_output_rate_target_hz  =  16000000,
    .vco_output_rate_target_hz  =  160000000,
    .system_clock_target_hz     =  TargetCoreClockrateHz,
    .ahb_clock_target_hz        = (TargetCoreClockrateHz / 1),
    .apb1_clock_target_hz       = (TargetCoreClockrateHz / 4),
    .apb2_clock_target_hz       = (TargetCoreClockrateHz / 4),
};

// Clock rates
extern uint32_t APB1ClockRateHz;
extern uint32_t APB2ClockRateHz;
extern uint32_t AHBClockRateHz;
extern uint32_t PLLClockRateHz;

// Function prototypes
extern void hard_fault_handler();

// main()
//
// Entry point for the application
void main(void)
{
    // Configure clocks
    if(!PHAL_configureClockRates(&clock_config))
        hard_fault_handler();

    if(!PHAL_initGPIO(gpio_config, sizeof(gpio_config) / sizeof(GPIOInitConfig_t)))
        hard_fault_handler();

    // Initialize keyboard
    keyboard_init();

    // Initialize scheduler
    schedInit(APB1ClockRateHz);

    // Create foreground task(s)
    taskCreate(keyboard_task, 1);

    // Create background task(s)
    taskCreateBackground(keyboard_bg_task);

    // Start the scheduler (will not return)
    schedStart();
}

// hard_fault_handler()
//
// This is where we end up if we do anything stupid
void hard_fault_handler()
{
    // Wait forever for a hard reset
    while(true) __asm__("nop");
}