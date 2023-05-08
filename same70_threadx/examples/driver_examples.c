/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

void I2C_AT24MAC_example(void)
{
	struct io_descriptor *I2C_AT24MAC_io;

	i2c_m_sync_get_io_descriptor(&I2C_AT24MAC, &I2C_AT24MAC_io);
	i2c_m_sync_enable(&I2C_AT24MAC);
	i2c_m_sync_set_slaveaddr(&I2C_AT24MAC, 0x12, I2C_M_SEVEN);
	io_write(I2C_AT24MAC_io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using TARGET_IO to write "Hello World" using the IO abstraction.
 */
void TARGET_IO_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&TARGET_IO, &io);
	usart_sync_enable(&TARGET_IO);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

void MACIF_example(void)
{
	mac_async_enable(&MACIF);
	mac_async_write(&MACIF, (uint8_t *)"Hello World!", 12);
}
