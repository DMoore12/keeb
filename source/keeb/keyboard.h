#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

// System includes
#include "stm32l432xx.h"
#include "common/phal_L4/gpio/gpio.h"
#include "common/queue/queue.h"
#include "common/common_defs/common_defs.h"

typedef enum {
    POLL_STATE_IDLE,                                                    // Idle preparation state
    POLL_STATE_WRITE,                                                   // Write column
    POLL_STATE_READ,                                                    // Read rows
    POLL_STATE_CLEAR,                                                   // Clear column
    // POLL_STATE_CNT MUST COME LAST
    POLL_STATE_CNT                                                      // Number of poll states
} poll_state_t;

// Layer enumeration
typedef enum {
    LAYER_DEFAULT,                                                      // Default layer
    // LAYER_CNT MUST COME LAST!
    LAYER_CNT                                                           // Number of layers
} layer_t;

// Column enumeration
typedef enum {
    COL_0,                                                              // Column 0
    COL_1,                                                              // Column 1
    COL_2,                                                              // Column 2
    COL_3,                                                              // Column 3
    COL_4,                                                              // Column 4
    COL_5,                                                              // Column 5
    COL_6,                                                              // Column 6
    COL_7,                                                              // Column 7
    COL_8,                                                              // Column 8
    COL_9,                                                              // Column 9
    COL_10,                                                             // Column 10
    COL_11,                                                             // Column 11
    COL_12,                                                             // Column 12
    COL_13,                                                             // Column 13
    COL_14,                                                             // Column 14
    COL_15,                                                             // Column 15
    // COL_CNT MUST COME LAST!
    COL_CNT                                                             // Number of columns
} column_t;

// Row enumeration
typedef enum {
    ROW_0,                                                              // Row 0
    ROW_1,                                                              // Row 1
    ROW_2,                                                              // Row 2
    ROW_3,                                                              // Row 3
    ROW_4,                                                              // Row 4
    // ROW_CNT MUST COME LAST!
    ROW_CNT                                                             // Number of rows

    // NOTE: ROW_CNT must not exceed 32 or keymapping will not function
} row_t;

// Keymappings
static const char*    keymap[LAYER_CNT][COL_CNT][ROW_CNT] = {
                                                                {   // Layer 0
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 0
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 1
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 2
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 3
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 4
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 5
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 6
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 7
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 8
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 9
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 10
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 11
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 12
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 13
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 14
                                                                    { "A", "B", "C", "D", "E"}, // Keys in column 15
                                                                },
                                                            };

// Input states
static bool     results_fresh;                                          // Indicates the results in pressed_column and pressed_rows are fresh
static uint32_t missed_inputs;                                          // Number of inputs the processing loop couldn't get to
static column_t pressed_column;                                         // Current search column
static uint32_t pressed_rows[COL_CNT];                                  // Current row states
static uint32_t pressed_rows_last[COL_CNT];                             // Last row states
static uint32_t pressed_keys[COL_CNT];                                  // Currently pressed keys
static layer_t  layer;                                                  // Currently selected layer

// Output queue
static q_handle_t presses;
static q_handle_t press_states;

// Function prototypes
void keyboard_init(void);
void keyboard_task(void);
void keyboard_bg_task(void);

#endif