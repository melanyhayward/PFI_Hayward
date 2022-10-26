/*
 *  @file subsystem_2.c
 *  @brief Subsystem 2 controller
 *  @author: Melany Hayward
 */ 

#include "subsystem_2.h"
#include "can/can_controller.h"
#include "config/conf_subsystem_2.h"

#define CMD_PAYLOAD_GET_THIRD_BYTE(x) (((x)&0xFF0000) >> 16)
#define CMD_PAYLOAD_GET_SECOND_BYTE(x) (((x)&0xFF00) >> 8)
#define CMD_PAYLOAD_GET_LSB_BYTE(x)	((x)&0xFF)

#define CMD_PAYLOAD_UNUSED_VAL 0

static status_code_t ss2_send_can_command(uint8_t cmd_id, uint32_t payload)
{
	uint8_t can_message[4];

	can_message[0] = cmd_id;
	can_message[1] = CMD_PAYLOAD_GET_THIRD_BYTE(payload);
	can_message[2] = CMD_PAYLOAD_GET_SECOND_BYTE(payload);
	can_message[3] = CMD_PAYLOAD_GET_LSB_BYTE(payload);
	
	status_code_t ret = can_tx_message(SS2_CAN_ID, sizeof(can_message), can_message);
	return ret;
}

status_code_t ss2_turn_on_LED(void)
{
	return ss2_send_can_command(SS2_CMD_ID_TURN_ON_LED, CMD_PAYLOAD_UNUSED_VAL);
}

status_code_t ss2_turn_off_LED(void)
{
	return ss2_send_can_command(SS2_CMD_ID_TURN_OFF_LED, CMD_PAYLOAD_UNUSED_VAL);
}

bool ss2_get_LED_status(void)
{
	status_code_t ret = ss2_send_can_command(SS2_CMD_ID_GET_LED_STATUS, CMD_PAYLOAD_UNUSED_VAL);
	if (ret == STATUS_OK)
	{
		// recibir e interpretar respuesta
	}
	return false;
}

int32_t ss2_get_temp(void)
{
	status_code_t ret = ss2_send_can_command(SS2_CMD_ID_GET_TEMP, CMD_PAYLOAD_UNUSED_VAL);
	if (ret == STATUS_OK)
	{
		// recibir e interpretar respuesta
	}
	return 0;
}
