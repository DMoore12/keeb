/**
 * @file can_parse.c
 * @author Luke Oxley (lcoxley@purdue.edu)
 * @brief Parsing of CAN messages using auto-generated structures with bit-fields
 * @version 0.1
 * @date 2021-09-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "can_parse.h"

// prototypes
bool initCANFilter();

can_data_t can_data;
q_handle_t* q_rx_can_a;

void initCANParse(q_handle_t* rx_a)
{
    q_rx_can_a = rx_a;
    initCANFilter();
}

void canRxUpdate()
{
    CanMsgTypeDef_t msg_header;
    CanParsedData_t* msg_data_a;

    if(qReceive(q_rx_can_a, &msg_header) == SUCCESS_G)
    {
        msg_data_a = (CanParsedData_t *) &msg_header.Data;
        /* BEGIN AUTO CASES */
        switch(msg_header.ExtId)
        {
            case ID_MAINMODULE_BL_CMD:
                can_data.mainmodule_bl_cmd.cmd = msg_data_a->mainmodule_bl_cmd.cmd;
                can_data.mainmodule_bl_cmd.data = msg_data_a->mainmodule_bl_cmd.data;
                mainmodule_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_DASHBOARD_BL_CMD:
                can_data.dashboard_bl_cmd.cmd = msg_data_a->dashboard_bl_cmd.cmd;
                can_data.dashboard_bl_cmd.data = msg_data_a->dashboard_bl_cmd.data;
                dashboard_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_TORQUEVECTOR_BL_CMD:
                can_data.torquevector_bl_cmd.cmd = msg_data_a->torquevector_bl_cmd.cmd;
                can_data.torquevector_bl_cmd.data = msg_data_a->torquevector_bl_cmd.data;
                torquevector_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_DRIVELINE_F_BL_CMD:
                can_data.driveline_f_bl_cmd.cmd = msg_data_a->driveline_f_bl_cmd.cmd;
                can_data.driveline_f_bl_cmd.data = msg_data_a->driveline_f_bl_cmd.data;
                driveline_f_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_DRIVELINE_R_BL_CMD:
                can_data.driveline_r_bl_cmd.cmd = msg_data_a->driveline_r_bl_cmd.cmd;
                can_data.driveline_r_bl_cmd.data = msg_data_a->driveline_r_bl_cmd.data;
                driveline_r_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_PRECHARGE_BL_CMD:
                can_data.precharge_bl_cmd.cmd = msg_data_a->precharge_bl_cmd.cmd;
                can_data.precharge_bl_cmd.data = msg_data_a->precharge_bl_cmd.data;
                precharge_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_BMS_A_BL_CMD:
                can_data.bms_a_bl_cmd.cmd = msg_data_a->bms_a_bl_cmd.cmd;
                can_data.bms_a_bl_cmd.data = msg_data_a->bms_a_bl_cmd.data;
                bms_a_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_BMS_B_BL_CMD:
                can_data.bms_b_bl_cmd.cmd = msg_data_a->bms_b_bl_cmd.cmd;
                can_data.bms_b_bl_cmd.data = msg_data_a->bms_b_bl_cmd.data;
                bms_b_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_BMS_C_BL_CMD:
                can_data.bms_c_bl_cmd.cmd = msg_data_a->bms_c_bl_cmd.cmd;
                can_data.bms_c_bl_cmd.data = msg_data_a->bms_c_bl_cmd.data;
                bms_c_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_BMS_D_BL_CMD:
                can_data.bms_d_bl_cmd.cmd = msg_data_a->bms_d_bl_cmd.cmd;
                can_data.bms_d_bl_cmd.data = msg_data_a->bms_d_bl_cmd.data;
                bms_d_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_BMS_E_BL_CMD:
                can_data.bms_e_bl_cmd.cmd = msg_data_a->bms_e_bl_cmd.cmd;
                can_data.bms_e_bl_cmd.data = msg_data_a->bms_e_bl_cmd.data;
                bms_e_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_BMS_F_BL_CMD:
                can_data.bms_f_bl_cmd.cmd = msg_data_a->bms_f_bl_cmd.cmd;
                can_data.bms_f_bl_cmd.data = msg_data_a->bms_f_bl_cmd.data;
                bms_f_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_BMS_G_BL_CMD:
                can_data.bms_g_bl_cmd.cmd = msg_data_a->bms_g_bl_cmd.cmd;
                can_data.bms_g_bl_cmd.data = msg_data_a->bms_g_bl_cmd.data;
                bms_g_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_BMS_H_BL_CMD:
                can_data.bms_h_bl_cmd.cmd = msg_data_a->bms_h_bl_cmd.cmd;
                can_data.bms_h_bl_cmd.data = msg_data_a->bms_h_bl_cmd.data;
                bms_h_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_BMS_I_BL_CMD:
                can_data.bms_i_bl_cmd.cmd = msg_data_a->bms_i_bl_cmd.cmd;
                can_data.bms_i_bl_cmd.data = msg_data_a->bms_i_bl_cmd.data;
                bms_i_bl_cmd_CALLBACK(msg_data_a);
                break;
            case ID_BMS_J_BL_CMD:
                can_data.bms_j_bl_cmd.cmd = msg_data_a->bms_j_bl_cmd.cmd;
                can_data.bms_j_bl_cmd.data = msg_data_a->bms_j_bl_cmd.data;
                bms_j_bl_cmd_CALLBACK(msg_data_a);
                break;
            default:
                __asm__("nop");
        }
        /* END AUTO CASES */
    }

    /* BEGIN AUTO STALE CHECKS */
    /* END AUTO STALE CHECKS */
}

bool initCANFilter()
{
    CAN1->MCR |= CAN_MCR_INRQ;                // Enter back into INIT state (required for changing scale)
#ifdef CAN2
    CAN2->MCR |= CAN_MCR_INRQ; 
#endif /* CAN2 */             
    uint32_t timeout = 0;
    while( !(CAN1->MSR & CAN_MSR_INAK)
#ifdef CAN2
           !(CAN2->MSR & CAN_MSR_INAK)
#endif /* CAN2 */
           && ++timeout < PHAL_CAN_INIT_TIMEOUT)
        ;
    if (timeout == PHAL_CAN_INIT_TIMEOUT)
         return false;

    CAN1->FMR  |= CAN_FMR_FINIT;              // Enter init mode for filter banks
    CAN1->FM1R |= 0x07FFFFFF;                 // Set banks 0-27 to id mode
    CAN1->FS1R |= 0x07FFFFFF;                 // Set banks 0-27 to 32-bit scale
#ifdef CAN2
    CAN2->FMR  |= CAN_FMR_FINIT;              // Enter init mode for filter banks
    CAN2->FM1R |= 0x07FFFFFF;                 // Set banks 0-27 to id mode
    CAN2->FS1R |= 0x07FFFFFF;                 // Set banks 0-27 to 32-bit scale
#endif /* CAN2 */
    /* BEGIN AUTO FILTER */
    CAN1->FA1R |= (1 << 0);    // configure bank 0
    CAN1->sFilterRegister[0].FR1 = (ID_MAINMODULE_BL_CMD << 3) | 4;
    CAN1->sFilterRegister[0].FR2 = (ID_DASHBOARD_BL_CMD << 3) | 4;
    CAN1->FA1R |= (1 << 1);    // configure bank 1
    CAN1->sFilterRegister[1].FR1 = (ID_TORQUEVECTOR_BL_CMD << 3) | 4;
    CAN1->sFilterRegister[1].FR2 = (ID_DRIVELINE_F_BL_CMD << 3) | 4;
    CAN1->FA1R |= (1 << 2);    // configure bank 2
    CAN1->sFilterRegister[2].FR1 = (ID_DRIVELINE_R_BL_CMD << 3) | 4;
    CAN1->sFilterRegister[2].FR2 = (ID_PRECHARGE_BL_CMD << 3) | 4;
    CAN1->FA1R |= (1 << 3);    // configure bank 3
    CAN1->sFilterRegister[3].FR1 = (ID_BMS_A_BL_CMD << 3) | 4;
    CAN1->sFilterRegister[3].FR2 = (ID_BMS_B_BL_CMD << 3) | 4;
    CAN1->FA1R |= (1 << 4);    // configure bank 4
    CAN1->sFilterRegister[4].FR1 = (ID_BMS_C_BL_CMD << 3) | 4;
    CAN1->sFilterRegister[4].FR2 = (ID_BMS_D_BL_CMD << 3) | 4;
    CAN1->FA1R |= (1 << 5);    // configure bank 5
    CAN1->sFilterRegister[5].FR1 = (ID_BMS_E_BL_CMD << 3) | 4;
    CAN1->sFilterRegister[5].FR2 = (ID_BMS_F_BL_CMD << 3) | 4;
    CAN1->FA1R |= (1 << 6);    // configure bank 6
    CAN1->sFilterRegister[6].FR1 = (ID_BMS_G_BL_CMD << 3) | 4;
    CAN1->sFilterRegister[6].FR2 = (ID_BMS_H_BL_CMD << 3) | 4;
    CAN1->FA1R |= (1 << 7);    // configure bank 7
    CAN1->sFilterRegister[7].FR1 = (ID_BMS_I_BL_CMD << 3) | 4;
    CAN1->sFilterRegister[7].FR2 = (ID_BMS_J_BL_CMD << 3) | 4;
    /* END AUTO FILTER */

    CAN1->FMR  &= ~CAN_FMR_FINIT;             // Enable Filters (exit filter init mode)

    // Enter back into NORMAL mode
    CAN1->MCR &= ~CAN_MCR_INRQ;
#ifdef CAN2
    CAN2->MCR &= ~CAN_MCR_INRQ;
#endif /* CAN2 */
    while((CAN1->MSR & CAN_MSR_INAK)
#ifdef CAN2 
          (CAN2->MSR & CAN_MSR_INAK)
#endif /* CAN2 */
        && ++timeout < PHAL_CAN_INIT_TIMEOUT)
        ;

    return timeout != PHAL_CAN_INIT_TIMEOUT;
}


void canProcessRxIRQs(CanMsgTypeDef_t* rx)
{
    CanParsedData_t* msg_data_a;

    msg_data_a = (CanParsedData_t *) rx->Data;
    switch(rx->ExtId)
    {
        /* BEGIN AUTO RX IRQ */
        /* END AUTO RX IRQ */
        default:
            __asm__("nop");
    }
}
