/*
 * conf_telemetry.c
 *
 * Created: 24/10/2022 22:17:34
 *  Author: Mel
 */ 
#include "conf_telemetry.h"
#include "tc/tc.h"
#include "telemetry.h"

#define TELEMETRY_TC_DIVISOR 128

tc_ticker_create(TELEMETRY_TICKER,
				 TC0,
				 0,
				 ID_TC0,
				 TC0_IRQn,
				 5,
				 TC_CMR_TCCLKS_TIMER_CLOCK4,
				 TELEMETRY_TC_DIVISOR,
				 TELEMETRY_UPDATE_PERIOD);
				 
/* forward declaration private function */
void telemetry_update(void);

void TC0_Handler(void)
{
	tc_ticker_clear_irq_flags(&TELEMETRY_TICKER);
	telemetry_update();
	printf("\n\rHola");
}