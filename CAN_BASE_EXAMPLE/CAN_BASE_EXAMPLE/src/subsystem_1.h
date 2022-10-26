/*
 *  @file subsystem_1.h
 *  @brief Subsystem 1 controller
 *  @author: Melany Hayward
 */ 


#ifndef SUBSYSTEM_1_H_
#define SUBSYSTEM_1_H_

#include <status_codes.h>
#include <compiler.h>

typedef struct ss1_info
{
	int32_t temperature;
	bool led_status;	
} ss1_info_t;

status_code_t ss1_turn_on_LED(void);
status_code_t ss1_turn_off_LED(void);

bool ss1_get_LED_status(void);
int32_t ss1_get_temp(void);

#endif /* SUBSYSTEM_1_H_ */