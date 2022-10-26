/*
 * telemetry.c
 *
 * Created: 23/10/2022 23:46:51
 *  Author: Mel
 */ 
#include "telemetry.h"
#include "commands.h"
#include "subsystem_1.h"
#include "subsystem_2.h"
#include "tc/tc.h"

#include <asf.h>
#include <string.h>

extern tc_ticker_t TELEMETRY_TICKER;

/* Static struct containing all system telemetry data */
static struct telemetry {
	ss1_info_t subsystem_1;
	ss2_info_t subsystem_2;
	cmd_counters_t commands;
} telemetry;

void telemetry_init(void)
{
	memset(&telemetry, 0, sizeof(telemetry));
	tc_ticker_init(&TELEMETRY_TICKER);
	tc_ticker_start(&TELEMETRY_TICKER);
}

void telemetry_update(void)
{
	telemetry.subsystem_1.led_status = ss1_get_LED_status();
	telemetry.subsystem_1.temperature = ss1_get_temp();
	
	telemetry.subsystem_2.led_status = ss2_get_LED_status();
	telemetry.subsystem_2.temperature = ss2_get_temp();
	
	telemetry.commands.executed_cmds = cmd_get_executed_count();
	telemetry.commands.last_executed_id = cmd_get_last_executed_id();
	telemetry.commands.aborted_cmds = cmd_get_aborted_count();
	telemetry.commands.last_aborted_id = cmd_get_last_aborted_id();
}

void telemetry_get(void)
{
	telemetry_update();

	printf("SS1 LED status: ");
	printf(telemetry.subsystem_1.led_status ? "ON \n\r" : "OFF \n\r");
	printf("SS1 Temperature: %u \n\r", telemetry.subsystem_1.temperature);

	printf("SS2 LED status: ");
	printf(telemetry.subsystem_2.led_status ? "ON \n\r" : "OFF \n\r");
	printf("SS2 Temperature: %u \n\r", telemetry.subsystem_2.temperature);
	
	printf("Executed commands count: %u \n\r", telemetry.commands.executed_cmds);
	printf("Aborted commands count: %u \n\r", telemetry.commands.aborted_cmds);
	printf("Last executed command ID: %u \n\r", telemetry.commands.last_executed_id);
	printf("Last aborted command ID: %u \n\r", telemetry.commands.last_aborted_id);
}