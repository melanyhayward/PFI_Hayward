/*
 * commands_module.c
 *
 * Created: 23/10/2022 20:35:29
 *  Author: Mel
 */ 
#include "config/conf_commands_module.h"
#include "commands.h"
#include "subsystem_1.h"
#include "subsystem_2.h"

static status_code_t command_get_cpu_temp(void)
{
	return STATUS_OK;
}

static status_code_t command_ss1_turn_on_led(void)
{
	return ss1_turn_on_LED();
}

static status_code_t command_ss1_turn_off_led(void)
{
	return ss1_turn_off_LED();
}

static status_code_t command_ss1_get_led_status(void)
{
	return ss1_get_LED_status();
}

static status_code_t command_ss1_get_temp(void)
{
	return ss1_get_temp();
}

static status_code_t command_ss2_turn_on_led(void)
{
	return ss2_turn_on_LED();
}

static status_code_t command_ss2_turn_off_led(void)
{
	return ss2_turn_off_LED();
}

static status_code_t command_ss2_get_led_status(void)
{
	return ss2_get_LED_status();
}

static status_code_t command_ss2_get_temp(void)
{
	return ss2_get_temp();
}

/* Commands Identifiers */
enum CMD_ID_MODULE
{
	CMD_MODULE_DEF_TABLE(CMD_GENERATE_ENUM_ITEM)
};

/* Create CMD_MODULE_LIST */
CREATE_CMD_LIST(MODULE, CMD_GET_CPU_TEMP, CMD_SS2_GET_TEMP);

/* USAR ESTA */
status_code_t commands_module_execute(uint8_t cmd_id, uint32_t cmd_payload)
{
	return cmd_process(&CMD_MODULE_LIST, cmd_id, cmd_payload);
}