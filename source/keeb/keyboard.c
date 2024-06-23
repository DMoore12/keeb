// Module includes
#include "keyboard.h"
#include "main.h"

// Static prototypes
static void update_key_state(column_t col, row_t row, bool set);
static void empty_presses(void);

// keyboard_init()s
//
// The keyboard initialization task. Sets the layer to the default and
// prepares for the first processing iteration
void keyboard_init(void)
{
    layer = LAYER_DEFAULT;
    results_fresh = false;
    missed_inputs = 0;
    qConstruct(&presses, sizeof(char*));
    qConstruct(&press_states, sizeof(bool));
}

// keyboard_task()
//
// The keyboard polling foreground task. Polls the keyboard, generating
// new results for later processing
void keyboard_task(void)
{
    // Locals
    poll_state_t next_state;

    static column_t     search_col;
    static poll_state_t current_state;

    // Prepare next state as current state
    next_state = current_state;

    // Run the poll state machine
    switch(current_state) {
        default:                                                        // Default to idle state
        POLL_STATE_IDLE:                                                // Idle state - prepare for polling
            search_col = 0;                                             // Default to column 0
            // Fall through
        
        POLL_STATE_WRITE:                                               // Write state - turn on column
            PHAL_writeGPIO(COL_Port, (uint8_t) search_col, true);       // Turn on column's outputs
            next_state = POLL_STATE_READ;                               // Move to the read state
            break;

        POLL_STATE_READ:                                                // Read state - read row inputs
            pressed_rows[search_col] = PHAL_readGPIOBank(ROW_Port);     // Read row inputs
            pressed_column = search_col;                                // Track search column for the processing loop/diagnostics
            if(results_fresh) ++missed_inputs;                          // Tracked a missed input
            else              results_fresh = true;                     // Mark the results as fresh for the processing loop, if necessary
            next_state = POLL_STATE_CLEAR;                              // Move to the clear state
            break;

        POLL_STATE_CLEAR:                                               // Clear state - turn off column
            PHAL_writeGPIOBank(COL_Port, 0UL);                          // Turn off column's outputs
            COUNTER(search_col, COL_CNT);                               // Move to the next column
            next_state = POLL_STATE_WRITE;                              // Move to the write state
            break;
    }

    // Refresh state
    current_state = next_state;
}

// keyboard_bg_task()
//
// The keyboard polling background task. Processes keypresses to update
// the current keymap states
void keyboard_bg_task(void)
{
    // Locals
    column_t search_col;

    // Skip processing when the results aren't fresh
    if(!results_fresh) return;

    // Process column by column
    // TODO (DWM): This can be much better
    for(search_col = COL_0; search_col < COL_CNT; search_col++) {
        row_t    search_row;
        uint32_t inputs_new = pressed_rows[search_col];
        uint32_t inputs_old = pressed_rows_last[search_col];
        uint32_t changes = inputs_new ^ inputs_old;

        // Search for rows with a new state and 
        for(search_row = ROW_0; search_row < ROW_CNT; search_row++) {
            if(changes & (1UL << search_row))
                update_key_state(search_col, search_row, inputs_new & (1U << search_row));
        }

        // Write current state to old state
        pressed_rows_last[search_col] = pressed_rows[search_col];
    }

    // Clear the fresh flag
    results_fresh = false;
}

// update_key_state()
//
// Updates a key's state by adding a key-string to the queue
//
// Args:
//     col - column to update
//     row - row to update
//     set - set/clear indication
static void update_key_state(column_t col, row_t row, bool set)
{
    // Make sure there is space for the update
    if(qIsFull(&presses))
        empty_presses();

    const char* key_string = keymap[layer][col][row];

    qSendToBack(&presses, &key_string);
    qSendToBack(&press_states, &set);
}

// empty_presses()
//
// Empties a few presses out of the queue to ensure we can process new
// inputs
static void empty_presses(void)
{
    // Locals
    uint8_t cnt;
    void*   buff;

    // Remove 10 items from the queue
    while(cnt++ < 10) {
        qReceive(&presses, buff);
        qReceive(&press_states, buff);
    }
}