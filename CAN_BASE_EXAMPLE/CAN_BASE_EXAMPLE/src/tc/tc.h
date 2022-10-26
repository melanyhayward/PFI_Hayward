/*
 * tc.h
 *
 * Created: 24/10/2022 21:25:47
 *  Author: Mel
 */ 


#ifndef TC_H_
#define TC_H_

#include <compiler.h>
#include <conf_clock.h>
#include <status_codes.h>

void tc_init(Tc *p_tc, uint32_t ul_channel, uint32_t ul_mode);
uint32_t tc_find_mck_divisor(uint32_t ul_freq, uint32_t ul_mck, uint32_t *p_uldiv, uint32_t *p_ultcclks, uint32_t ul_boardmck);
void tc_configure_to_no_divide_clock(Tc *p_tc, uint32_t ul_channel);

void tc_start(Tc *p_tc, uint32_t ul_channel);
void tc_stop(Tc *p_tc, uint32_t ul_channel);

uint32_t tc_read_cv(Tc *p_tc, uint32_t ul_channel);
uint32_t tc_read_ra(Tc *p_tc, uint32_t ul_channel);
uint32_t tc_read_rb(Tc *p_tc, uint32_t ul_channel);
uint32_t tc_read_rc(Tc *p_tc, uint32_t ul_channel);

void tc_write_ra(Tc *p_tc, uint32_t ul_channel, uint32_t ul_value);
void tc_write_rb(Tc *p_tc, uint32_t ul_channel, uint32_t ul_value);
void tc_write_rc(Tc *p_tc, uint32_t ul_channel, uint32_t ul_value);

void tc_enable_interrupt(Tc *p_tc, uint32_t ul_channel, uint32_t ul_sources);
void tc_disable_interrupt(Tc *p_tc, uint32_t ul_channel, uint32_t ul_sources);
uint32_t tc_get_interrupt_mask(Tc *p_tc, uint32_t ul_channel);
uint32_t tc_get_status(Tc *p_tc, uint32_t ul_channel);

void tc_set_writeprotect(Tc *p_tc, uint32_t ul_enable);

/*============================================================================*/

/** @brief Configuration for TC used by Ticker service */
typedef struct tc_ticker_t
{
    Tc *TC;              /**< Timer Counter instance */
    uint32_t TC_CHANNEL; /**< Timer Counter channel */
    /** Timer Counter ID (depends on combination of TC and channel)
     * Ex: SAMV71: ID_TC0>TC0_CH0, ID_TC1>TC0_CH1,ID_TC2>TC0_CH2,ID_TC3>TC1_CH0,ID_TC4>TC1_CH1,...
     * Ex: SMARH71: ID_TC1_CHANNEL2, ID_TC2_CHANNEL0...
     */
    uint32_t TC_ID;
    uint32_t TC_IRQn;         /**< System IRQ that handles the TC */
    uint32_t TC_IRQ_PRIORITY; /**< TC Irq Priority */

    uint32_t TC_CLOCK;        /**< Clock selection */
    uint32_t TC_DIVISOR;      /**< Clock divisor "if 1 => FORCED NO CLK DIVIDE" */
    uint32_t TC_PERIOD_IN_MS; /**< Desired tick period in ms */
} tc_ticker_t;

/** TC 16 bit counter max value for SAMV71 */
#define TC_COUNTER_MAX_VALUE 0xFFFF
/** TC counter invalid configuration */
#define TC_COUNTER_INVALID_VALUE 0

/**
 * @brief Helper macro to create ticker, check freq. config and load @ref tc_ticker_t parameters.
 *
 * @param _TICKER_NAME          Name for struct tc_ticker_t
 * @param _TC                   Timer Counter instance
 * @param _TC_CHANNEL           Timer Counter channel
 * @param _TC_ID                Timer Peripheral ID
 * @param _TC_IRQn              System IRQ that handles the TC
 * @param _TC_IRQ_PRIORITY      TC Irq Priority
 * @param _TC_CLOCK             Clock selection
 * @param _TC_DIVISOR           Clock divisor (bigger than 0,  "if 1 => FORCED NO CLK DIVIDE")
 * @param _TC_PERIOD_IN_MS      Desired tick period in ms (bigger than 0)
 */
#define tc_ticker_create(                                                                                             \
    _TICKER_NAME, _TC, _TC_CHANNEL, _TC_ID, _TC_IRQn, _TC_IRQ_PRIORITY, _TC_CLOCK, _TC_DIVISOR, _TC_PERIOD_IN_MS)     \
    /** _TICKER_NAME Ticker. Period in ms: TC_PERIOD_IN_MS */                                                         \
    tc_ticker_t _TICKER_NAME = {.TC = (_TC),                                                                          \
                                .TC_CHANNEL = (_TC_CHANNEL),                                                          \
                                .TC_ID = (_TC_ID),                                                                    \
                                .TC_IRQn = (_TC_IRQn),                                                                \
                                .TC_IRQ_PRIORITY = (_TC_IRQ_PRIORITY),                                                \
                                .TC_CLOCK = (_TC_CLOCK),                                                              \
                                .TC_DIVISOR = (_TC_DIVISOR),                                                          \
                                .TC_PERIOD_IN_MS = (_TC_PERIOD_IN_MS)};

void tc_ticker_init(const tc_ticker_t *ticker);
void tc_ticker_stop(const tc_ticker_t *ticker);
void tc_ticker_start(const tc_ticker_t *ticker);
void tc_ticker_disable_irq(const tc_ticker_t *ticker);
void tc_ticker_enable_irq(const tc_ticker_t *ticker);
void tc_ticker_clear_irq_flags(const tc_ticker_t *ticker);
uint32_t tc_ticker_get_value(const tc_ticker_t *ticker);
uint32_t tc_ticker_get_clock_freq_in_hz(const tc_ticker_t *ticker);
uint32_t tc_ticker_get_max_count(const tc_ticker_t *ticker);
void tc_ticker_kick(const tc_ticker_t *ticker);
status_code_t tc_ticker_update_period(tc_ticker_t *ticker, uint32_t period);
bool tc_ticker_check_valid_period(const tc_ticker_t *ticker, uint32_t period);
bool tc_ticker_is_running(const tc_ticker_t *ticker);

#endif /* TC_H_ */