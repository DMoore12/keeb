#ifndef _MAIN_H_
#define _MAIN_H_

// System includes
#include "stm32l432xx.h"
#include "common/psched/psched.h"
#include "common/phal_L4/rcc/rcc.h"
#include "common/phal_L4/gpio/gpio.h"
#include "common/phal_L4/tim/tim.h"
#include "common/phal_L4/dma/dma.h"
#include <math.h>

// System constants
#define TargetCoreClockrateHz 80000000

// Columns
#define COL_Port (GPIOA)

// Rows
#define ROW_Port (GPIOB)

#endif