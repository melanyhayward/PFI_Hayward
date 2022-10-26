/*
 * can_controller.h
 *
 * Created: 22/10/2022 20:14:36
 *  Author: Mel
 */ 


#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <asf.h>
#include <status_codes.h>

/* mcan_filter_setting */
#define MCAN_RX_STANDARD_FILTER_INDEX_0    0
#define MCAN_RX_STANDARD_FILTER_INDEX_1    1
#define MCAN_RX_STANDARD_FILTER_ID_0     0x45A
#define MCAN_RX_STANDARD_FILTER_ID_0_BUFFER_INDEX     2
#define MCAN_RX_STANDARD_FILTER_ID_1     0x469
#define MCAN_RX_EXTENDED_FILTER_INDEX_0    0
#define MCAN_RX_EXTENDED_FILTER_INDEX_1    1
#define MCAN_RX_EXTENDED_FILTER_ID_0     0x100000A5
#define MCAN_RX_EXTENDED_FILTER_ID_0_BUFFER_INDEX     1
#define MCAN_RX_EXTENDED_FILTER_ID_1     0x10000096

#define SUBSYSTEM_1_ID	0x45A
#define SUBSYSTEM_2_ID	0x46A

/* mcan_transfer_message_setting */
#define MCAN_TX_BUFFER_INDEX    0

void can_init(void);
void can_start(void);
void can_stop(void);
void can_enable_irq(void);
void can_disable_irq(void);
void can_set_rx_standard_buffer_filter(uint32_t id_value, uint32_t buffer_idx, uint32_t idx);
void can_set_rx_standard_fifo_filter(uint32_t id_value, uint32_t idx);
void can_set_rx_extended_buffer_filter(uint32_t id_value, uint32_t buffer_idx, uint32_t idx);
void can_set_rx_extended_fifo_filter(uint32_t id_value, uint32_t idx);
void can_tx_standard_message(uint32_t id_value, uint8_t *data);
status_code_t can_tx_message(uint32_t id_value, uint8_t length, uint8_t *data);
void can_fd_tx_standard_message(uint32_t id_value, uint8_t *data);
void can_fd_tx_extended_message(uint32_t id_value, uint8_t *data);

#endif /* CAN_CONTROLLER_H_ */