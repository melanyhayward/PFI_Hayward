/*
 * tc.c
 *
 * Created: 24/10/2022 21:37:17
 *  Author: Mel
 */ 
#include "tc.h"
#include <conf_board.h>
#include <samv71_xplained_ultra/samv71_xplained_ultra.h>
#include <sysclk.h>

#ifndef TC_WPMR_WPKEY_PASSWD
#define TC_WPMR_WPKEY_PASSWD TC_WPMR_WPKEY((uint32_t)0x54494D)
#endif

/** TC divisor used to find the lowest acceptable timer frequency */
#define TC_DIV_FACTOR 65536

void tc_init(Tc *p_tc, uint32_t ul_channel, uint32_t ul_mode)
{
	TcChannel *tc_channel;

	/* TODO: Validate inputs. */

	tc_channel = &p_tc->TC_CHANNEL[ul_channel];

	/*  Disable TC clock. */
	tc_channel->TC_CCR = TC_CCR_CLKDIS;

	/*  Disable interrupts. */
	tc_channel->TC_IDR = 0xFFFFFFFF;

	/*  Clear status register. */
	tc_channel->TC_SR;

	/*  Set mode. */
	tc_channel->TC_CMR = ul_mode;
}

void tc_configure_to_no_divide_clock(Tc *p_tc, uint32_t ul_channel)
{
	p_tc->TC_CHANNEL[ul_channel].TC_EMR |= TC_EMR_NODIVCLK;
}

void tc_start(Tc *p_tc, uint32_t ul_channel)
{
	p_tc->TC_CHANNEL[ul_channel].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void tc_stop(Tc *p_tc, uint32_t ul_channel)
{
	p_tc->TC_CHANNEL[ul_channel].TC_CCR = TC_CCR_CLKDIS;
}

uint32_t tc_read_cv(Tc *p_tc, uint32_t ul_channel)
{
	return p_tc->TC_CHANNEL[ul_channel].TC_CV;
}

uint32_t tc_read_ra(Tc *p_tc, uint32_t ul_channel)
{
	return p_tc->TC_CHANNEL[ul_channel].TC_RA;
}

uint32_t tc_read_rb(Tc *p_tc, uint32_t ul_channel)
{
    return p_tc->TC_CHANNEL[ul_channel].TC_RB;
}

uint32_t tc_read_rc(Tc *p_tc, uint32_t ul_channel)
{
    return p_tc->TC_CHANNEL[ul_channel].TC_RC;
}

void tc_write_ra(Tc *p_tc, uint32_t ul_channel, uint32_t ul_value)
{
    p_tc->TC_CHANNEL[ul_channel].TC_RA = ul_value;
}

void tc_write_rb(Tc *p_tc, uint32_t ul_channel, uint32_t ul_value)
{
    p_tc->TC_CHANNEL[ul_channel].TC_RB = ul_value;
}

void tc_write_rc(Tc *p_tc, uint32_t ul_channel, uint32_t ul_value)
{
    p_tc->TC_CHANNEL[ul_channel].TC_RC = ul_value;
}

void tc_enable_interrupt(Tc *p_tc, uint32_t ul_channel, uint32_t ul_sources)
{
    p_tc->TC_CHANNEL[ul_channel].TC_IER = ul_sources;
}

void tc_disable_interrupt(Tc *p_tc, uint32_t ul_channel, uint32_t ul_sources)
{
    p_tc->TC_CHANNEL[ul_channel].TC_IDR = ul_sources;
}

uint32_t tc_get_interrupt_mask(Tc *p_tc, uint32_t ul_channel)
{
    return p_tc->TC_CHANNEL[ul_channel].TC_IMR;
}

uint32_t tc_get_status(Tc *p_tc, uint32_t ul_channel)
{
    return p_tc->TC_CHANNEL[ul_channel].TC_SR;
}

uint32_t tc_find_mck_divisor(uint32_t ul_freq, uint32_t ul_mck, uint32_t *p_uldiv, uint32_t *p_ultcclks, uint32_t ul_boardmck)
{
    const uint32_t divisors[5] = {2, 8, 32, 128, ul_boardmck / BOARD_FREQ_SLCK_XTAL};
    uint32_t ul_index;
    uint32_t ul_high, ul_low;

    /*  Satisfy frequency bound. */
    for (ul_index = 0; ul_index < (sizeof(divisors) / sizeof(divisors[0])); ul_index++)
    {
        ul_high = ul_mck / divisors[ul_index];
        ul_low = ul_high / TC_DIV_FACTOR;
        if (ul_freq > ul_high)
        {
            return 0;
        }
        else if (ul_freq >= ul_low)
        {
            break;
        }
    }
    if (ul_index >= (sizeof(divisors) / sizeof(divisors[0])))
    {
        return 0;
    }

    /*  Store results. */
    if (p_uldiv)
    {
        *p_uldiv = divisors[ul_index];
    }

    if (p_ultcclks)
    {
        *p_ultcclks = ul_index;
    }

    return 1;
}

void tc_set_writeprotect(Tc *p_tc, uint32_t ul_enable)
{
    if (ul_enable)
    {
        p_tc->TC_WPMR = TC_WPMR_WPKEY_PASSWD | TC_WPMR_WPEN;
    }
    else
    {
        p_tc->TC_WPMR = TC_WPMR_WPKEY_PASSWD;
    }
}

/*============================================================================*/
#define TC_CMR_CAPTURE_CPCTRG_Msk TC_CMR_CPCTRG
#define TC_IER_CPCS_Msk           TC_IER_CPCS

__always_inline uint32_t tc_ticker_get_clock_freq_in_hz(const tc_ticker_t *ticker)
{
    return (BOARD_MCK / ticker->TC_DIVISOR);
}

__always_inline static uint32_t tc_ticker_get_max_count_for_period(const tc_ticker_t *ticker, uint32_t period_in_ms)
{
    uint64_t result = ((uint64_t)((BOARD_MCK / ticker->TC_DIVISOR) * ((uint64_t)period_in_ms)) / 1000U);

    if (result > TC_COUNTER_MAX_VALUE)
    {
        result = TC_COUNTER_INVALID_VALUE;
    }

    return result;
}

__always_inline bool tc_ticker_check_valid_period(const tc_ticker_t *ticker, uint32_t period)
{
    return (tc_ticker_get_max_count_for_period(ticker, period) != TC_COUNTER_INVALID_VALUE);
}

__always_inline uint32_t tc_ticker_get_max_count(const tc_ticker_t *ticker)
{
    return tc_ticker_get_max_count_for_period(ticker, ticker->TC_PERIOD_IN_MS);
}

__always_inline void tc_ticker_init(const tc_ticker_t *ticker)
{
    uint32_t counts_max;

    sysclk_enable_peripheral_clock(ticker->TC_ID);

    if (ticker->TC_DIVISOR == 1)
    {
        tc_configure_to_no_divide_clock(ticker->TC, ticker->TC_CHANNEL);
    }

    counts_max = tc_ticker_get_max_count(ticker);

    tc_init(ticker->TC, ticker->TC_CHANNEL, ticker->TC_CLOCK | TC_CMR_CAPTURE_CPCTRG_Msk);
    tc_write_rc(ticker->TC, ticker->TC_CHANNEL, counts_max);
    tc_enable_interrupt(ticker->TC, ticker->TC_CHANNEL, TC_IER_CPCS_Msk);
}

__always_inline void tc_ticker_stop(const tc_ticker_t *ticker)
{
    NVIC_DisableIRQ(ticker->TC_IRQn);
    NVIC_ClearPendingIRQ(ticker->TC_IRQn);
    sysclk_disable_peripheral_clock(ticker->TC_ID);
    tc_stop(ticker->TC, ticker->TC_CHANNEL);
}

__always_inline void tc_ticker_start(const tc_ticker_t *ticker)
{
    NVIC_DisableIRQ(ticker->TC_IRQn);
    NVIC_ClearPendingIRQ(ticker->TC_IRQn);
    NVIC_SetPriority(ticker->TC_IRQn, ticker->TC_IRQ_PRIORITY);
    NVIC_EnableIRQ(ticker->TC_IRQn);
    sysclk_enable_peripheral_clock(ticker->TC_ID);
    tc_start(ticker->TC, ticker->TC_CHANNEL);
}

__always_inline void tc_ticker_disable_irq(const tc_ticker_t *ticker)
{
    NVIC_DisableIRQ(ticker->TC_IRQn);
}

__always_inline void tc_ticker_enable_irq(const tc_ticker_t *ticker)
{
    NVIC_EnableIRQ(ticker->TC_IRQn);
}

__always_inline void tc_ticker_clear_irq_flags(const tc_ticker_t *ticker)
{
    volatile uint32_t ul_dummy;
    /* Read to clear status bit to acknowledge interrupt */
    ul_dummy = tc_get_status(ticker->TC, ticker->TC_CHANNEL);
    /* Avoid compiler warning */
    UNUSED(ul_dummy);
}

__always_inline uint32_t tc_ticker_get_value(const tc_ticker_t *ticker)
{
    return ticker->TC->TC_CHANNEL[ticker->TC_CHANNEL].TC_CV;
}

__always_inline void tc_ticker_kick(const tc_ticker_t *ticker)
{
    tc_start(ticker->TC, ticker->TC_CHANNEL);
}

__always_inline status_code_t tc_ticker_update_period(tc_ticker_t *ticker, uint32_t period)
{
    status_code_t ret = ERR_INVALID_ARG;
    if (tc_ticker_check_valid_period(ticker, period))
    {
        bool irq_on = NVIC_GetEnableIRQ(ticker->TC_IRQn);
        tc_ticker_disable_irq(ticker);
        ticker->TC_PERIOD_IN_MS = period;
        tc_write_rc(ticker->TC, ticker->TC_CHANNEL, tc_ticker_get_max_count(ticker));
        if (irq_on)
        {
            tc_ticker_enable_irq(ticker);
        }
        ret = STATUS_OK;
    }

    return ret;
}

bool tc_ticker_is_running(const tc_ticker_t *ticker)
{
    return NVIC_GetEnableIRQ(ticker->TC_IRQn);
}