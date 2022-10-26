/*
 * uart_controller.h
 *
 * Created: 25/10/2022 20:27:37
 *  Author: Mel
 */ 


#ifndef UART_CONTROLLER_H_
#define UART_CONTROLLER_H_

#include <asf.h>
#include <status_codes.h>

uint32_t uart_controller_init(void);
void uart_controller_enable(void);
void uart_controller_disable(void);
void uart_controller_reset(void);
void uart_controller_enable_irq(void);
void uart_controller_disable_irq(void);
uint32_t uart_controller_write(const uint8_t uc_data);
uint32_t uart_controller_read(uint8_t *puc_data);



#endif /* UART_CONTROLLER_H_ */