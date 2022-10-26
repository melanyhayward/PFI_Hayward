/*
 * commands.h
 *
 * Created: 22/10/2022 21:48:27
 *  Author: Mel
 */ 

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <asf.h>

typedef struct cmd_counters
{
	uint32_t executed_cmds;
	uint8_t last_executed_id;
	uint32_t aborted_cmds;
	uint8_t last_aborted_id;
} cmd_counters_t;

#define CMD_GENERATE_ENUM_ITEM(_name_, _id_, _cmd_func_) _name_ = _id_,

#define CMD_GENERATE_LIST_COMMAND_DEF(_name_, _id_, _cmd_func_) \
	{                                                           \
		.cmd_func = _cmd_func_,                                 \
	},

typedef status_code_t (*command_processor_execute_func_t)(uint8_t cmd_id, uint32_t cmd_payload);
typedef void (*command_processor_init_func_t)(void);
typedef struct command_processor_ops_t
{
	command_processor_execute_func_t execute;
	command_processor_init_func_t init;
} command_processor_ops_t;

typedef status_code_t (*command_func_t)(uint32_t cmd_payload);
typedef struct command_ops_t
{
	command_func_t cmd_func;
} command_ops_t;

typedef struct command_list_t
{
	const uint32_t START;
	const uint32_t END;
	const command_ops_t *ELEMENTS;
} command_list_t;

#define CREATE_CMD_LIST(CMD_GROUP_NAME, CMD_START_ID, CMD_END_ID)       \
	static const command_ops_t CMD_##CMD_GROUP_NAME##_DEFINITIONS[] = { \
		CMD_##CMD_GROUP_NAME##_DEF_TABLE(CMD_GENERATE_LIST_COMMAND_DEF) \
	};                                                                  \
	                                                                    \
	const command_list_t CMD_##CMD_GROUP_NAME##_LIST = {                \
		.START = CMD_START_ID,                                          \
		.END = CMD_END_ID,                                              \
		.ELEMENTS = CMD_##CMD_GROUP_NAME##_DEFINITIONS,                 \
	}

void cmd_init(void);
status_code_t cmd_process(const command_list_t *cmd_list, uint8_t cmd_id, uint32_t cmd_payload);
void cmd_execute_and_update_status(uint8_t cmd_id, uint32_t payload);
uint32_t cmd_get_executed_count(void);
uint8_t cmd_get_last_executed_id(void);
uint32_t cmd_get_aborted_count(void);
uint8_t cmd_get_last_aborted_id(void);

#endif /* COMMANDS_H_ */