/*
 *  @file subsystem_2.h
 *  @brief Subsystem 2 controller
 *  @author: Melany Hayward
 */ 

#ifndef SUBSYSTEM_2_H_
#define SUBSYSTEM_2_H_

#include <status_codes.h>
#include <compiler.h>

typedef struct ss2_info
{
	int32_t temperature;
	bool led_status;
} ss2_info_t;

status_code_t ss2_turn_on_LED(void);
status_code_t ss2_turn_off_LED(void);

bool ss2_get_LED_status(void);
int32_t ss2_get_temp(void);

#endif /* SUBSYSTEM_2_H_ */