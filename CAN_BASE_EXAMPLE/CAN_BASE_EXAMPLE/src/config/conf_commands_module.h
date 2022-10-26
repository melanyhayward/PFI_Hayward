/*
 * conf_commands.h
 *
 * Created: 23/10/2022 20:09:40
 *  Author: Mel
 */ 


#ifndef CONF_COMMANDS_H_
#define CONF_COMMANDS_H_

#define CMD_MODULE_DEF_TABLE(X_MACRO) \
	X_MACRO(CMD_GET_CPU_TEMP                     , 0x00, command_get_cpu_temp)       \
	X_MACRO(CMD_SS1_TURN_ON_LED                  , 0x01, command_ss1_turn_on_led)    \
    X_MACRO(CMD_SS1_TURN_OFF_LED                 , 0x02, command_ss1_turn_off_led)   \
	X_MACRO(CMD_SS1_GET_LED_STATUS               , 0x03, command_ss1_get_led_status) \
	X_MACRO(CMD_SS1_GET_TEMP                     , 0x04, command_ss1_get_temp)       \
	X_MACRO(CMD_SS2_TURN_ON_LED                  , 0x05, command_ss2_turn_on_led)    \
	X_MACRO(CMD_SS2_TURN_OFF_LED                 , 0x06, command_ss2_turn_off_led)   \
	X_MACRO(CMD_SS2_GET_LED_STATUS               , 0x07, command_ss2_get_led_status) \
	X_MACRO(CMD_SS2_GET_TEMP                     , 0x08, command_ss2_get_temp)

#endif /* CONF_COMMANDS_H_ */