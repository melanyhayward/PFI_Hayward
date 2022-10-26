/*
 * commands.c
 *
 * Created: 23/10/2022 19:41:52
 *  Author: Mel
 */
#include "commands.h"
#include "can/can_controller.h"

/* CMD counters instance */
cmd_counters_t counters;

void cmd_init(void)
{
	counters.executed_cmds = 0;
	counters.last_aborted_id = 0;
	counters.aborted_cmds = 0;
	counters.last_executed_id = 0;
}

uint32_t cmd_get_executed_count(void)
{
	return counters.executed_cmds;
}

uint8_t cmd_get_last_executed_id(void)
{
	return counters.last_executed_id;
}

static void cmd_set_last_executed_id(uint8_t cmd_id)
{
	counters.last_executed_id = cmd_id;
}

uint32_t cmd_get_aborted_count(void)
{
	return counters.aborted_cmds;
}

uint8_t cmd_get_last_aborted_id(void)
{
	return counters.last_aborted_id;
}

static void cmd_set_last_aborted_id(uint8_t cmd_id)
{
	counters.last_aborted_id = cmd_id;
}

status_code_t cmd_process(const command_list_t *cmd_list, uint8_t cmd_id, uint32_t cmd_payload)
{
	status_code_t status = OPERATION_IN_PROGRESS;
	if ((cmd_list->START <= cmd_id) && (cmd_id <= cmd_list->END))
	{
		uint32_t cmd_list_index = cmd_id - cmd_list->START;
		const command_ops_t *cmd = &cmd_list->ELEMENTS[cmd_list_index];
		status = cmd->cmd_func(cmd_payload);
	}
	if (status == STATUS_OK)
	{
		cmd_set_last_executed_id(cmd_id);
		counters.executed_cmds++;
	}
	else
	{
		cmd_set_last_aborted_id(cmd_id);
		counters.aborted_cmds++;
	}

	return status;
}
