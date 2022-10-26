#include <asf.h>
#include <string.h>
#include "commands.h"
#include "commands_module.h"
#include "can/can_controller.h"
#include "conf_subsystem_1.h"
#include "telemetry.h"

/**
 * \brief Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.charlength = CONF_UART_CHAR_LENGTH,
		.paritytype = CONF_UART_PARITY,
		.stopbits = CONF_UART_STOP_BITS,
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief display configuration menu.
 */
static void display_commands_menu(void)
{
	printf("Menu :\r\n"
			"  -- Select the command:\r\n"
			"  0: Download telemetry. \r\n"
			"  1: Get CPU temperature. \r\n"
			"  2: System 1: turn ON actuator (LED). \r\n"
			"  3: System 1: turn OFF actuator (LED). \r\n"
			"  4: System 1: get actuator status (LED). \r\n"
			"  5: System 1: get temperature read. \r\n"
			"  6: System 2: turn ON actuator (LED). \r\n"
			"  7: System 2: turn OFF actuator (LED). \r\n"
			"  8: System 2: get actuator status (LED). \r\n"
			"  9: System 2: get temperature read. \r\n"
			"  a: Get commands counters values. \r\n"
			"  h: Display menu \r\n\r\n");
}

static void system_init(void)
{
	sysclk_init();
	board_init();
	configure_console();
	can_init();
	can_start();
	display_commands_menu();
	cmd_init();
	//telemetry_init();
}

int main(void)
{
	uint8_t key;
	uint8_t data_1[6] = {0x5, 0x4, 0x3, 0x2, 0x1, 0x0};
	
	system_init();

	while(1) {
		scanf("%c", (char *)&key);

		switch (key) {
		case 'h':
			display_commands_menu();
			break;

		case '0':
			printf(" 0: Download telemetry. \r\n");
			telemetry_get();
			break;

		case '1':
			printf(" 1: Get CPU temperature. \r\n");
			break;

		case '2':
			printf(" 2: System 1: turn ON actuator (LED). \r\n");
			commands_module_execute(0x01, 0x123456);
			break;

		case '3':
			printf(" 3: System 1: turn OFF actuator (LED). \r\n");
			commands_module_execute(0x02, 0x123456);
			break;

		case '4':
			printf(" 4: System 1: get actuator status (LED). \r\n");
			commands_module_execute(0x03, 0x123456);
			break;

		case '5':
			printf(" 5: System 1: get temperature read. \r\n");
			commands_module_execute(0x04, 0x123456);
			break;

		case '6':
			printf(" 6: System 2: turn ON actuator (LED). \r\n");
			commands_module_execute(0x05, 0x123456);
			break;

		case '7':
			printf(" 7: System 2: turn OFF actuator (LED). \r\n");
			commands_module_execute(0x06, 0x123456);
			break;

		case '8':
			printf(" 8: System 2: get actuator status (LED). \r\n");
			commands_module_execute(0x07, 0x123456);
			break;

		case '9':
			printf(" 9: System 2: get temperature read. \r\n");
			commands_module_execute(0x08, 0x123456);
			break;
			
		case 'a':
			printf("Commands counters\r\n");
			printf(" Executed commands: %u \n\r", cmd_get_executed_count());
			printf(" Aborted commands: %u \n\r", cmd_get_aborted_count());
			break;

		default:
			break;
		}
	}
}
