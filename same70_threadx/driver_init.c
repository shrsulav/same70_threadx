/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_usart_base.h>

struct i2c_m_sync_desc I2C_AT24MAC;

struct usart_sync_descriptor TARGET_IO;

struct mac_async_descriptor MACIF;

void I2C_AT24MAC_PORT_init(void)
{

	gpio_set_pin_function(PA4, MUX_PA4A_TWIHS0_TWCK0);

	gpio_set_pin_function(PA3, MUX_PA3A_TWIHS0_TWD0);
}

void I2C_AT24MAC_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_TWIHS0);
}

void I2C_AT24MAC_init(void)
{
	I2C_AT24MAC_CLOCK_init();

	i2c_m_sync_init(&I2C_AT24MAC, TWIHS0);

	I2C_AT24MAC_PORT_init();
}

void TARGET_IO_PORT_init(void)
{

	gpio_set_pin_function(PA21, MUX_PA21A_USART1_RXD1);

	gpio_set_pin_function(PB4, MUX_PB4D_USART1_TXD1);
}

void TARGET_IO_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_USART1);
}

void TARGET_IO_init(void)
{
	TARGET_IO_CLOCK_init();
	TARGET_IO_PORT_init();
	usart_sync_init(&TARGET_IO, USART1, _usart_get_usart_sync());
}

void MACIF_PORT_init(void)
{

	gpio_set_pin_function(PD8, MUX_PD8A_GMAC_GMDC);

	gpio_set_pin_function(PD9, MUX_PD9A_GMAC_GMDIO);

	gpio_set_pin_function(PD5, MUX_PD5A_GMAC_GRX0);

	gpio_set_pin_function(PD6, MUX_PD6A_GMAC_GRX1);

	gpio_set_pin_function(PD4, MUX_PD4A_GMAC_GRXDV);

	gpio_set_pin_function(PD7, MUX_PD7A_GMAC_GRXER);

	gpio_set_pin_function(PD2, MUX_PD2A_GMAC_GTX0);

	gpio_set_pin_function(PD3, MUX_PD3A_GMAC_GTX1);

	gpio_set_pin_function(PD0, MUX_PD0A_GMAC_GTXCK);

	gpio_set_pin_function(PD1, MUX_PD1A_GMAC_GTXEN);
}

void MACIF_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_GMAC);
}

void MACIF_init(void)
{
	MACIF_CLOCK_init();
	mac_async_init(&MACIF, GMAC);
	MACIF_PORT_init();
}

void system_init(void)
{
	init_mcu();

	_pmc_enable_periph_clock(ID_PIOC);

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	/* GPIO on PC10 */

	gpio_set_pin_level(PHY_RESET_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(PHY_RESET_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PHY_RESET_PIN, GPIO_PIN_FUNCTION_OFF);

	I2C_AT24MAC_init();

	TARGET_IO_init();

	MACIF_init();
}
