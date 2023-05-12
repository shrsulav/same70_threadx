/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <atmel_start.h>
#include <peripheral_clk_config.h>
#include <lwip/netif.h>
#include <lwip/dhcp.h>
#if LWIP_VERSION_MAJOR > 1
#include <lwip/timeouts.h>
#else
#include <lwip/timers.h>
#endif
#include "lwip_demo_config.h"
#include "lwip_macif_config.h"
#include <string.h>

#include "tx_api.h"
#include "tx_port.h"

/* Saved total time in mS since timer was enabled */
volatile static u32_t systick_timems;
volatile static bool recv_flag = false;
static bool link_up = false;

u32_t sys_now(void)
{
	return systick_timems;
}

// void SysTick_Handler(void)
// {
// 	systick_timems++;
// }

void systick_enable(void)
{
	systick_timems = 0;
	SysTick_Config((CONF_CPU_FREQUENCY) / 1);
}

void mac_receive_cb(struct mac_async_descriptor *desc)
{
	recv_flag = true;
}
static void print_ipaddress(void)
{
	static char tmp_buff[16];
	printf("IP_ADDR    : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *)&(LWIP_MACIF_desc.ip_addr), tmp_buff, 16));
	printf("NET_MASK   : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *)&(LWIP_MACIF_desc.netmask), tmp_buff, 16));
	printf("GATEWAY_IP : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *)&(LWIP_MACIF_desc.gw), tmp_buff, 16));
}

static void read_macaddress(u8_t *mac)
{
#if CONF_AT24MAC_ADDRESS != 0
	uint8_t addr = 0x9A;

	i2c_m_sync_enable(&I2C_AT24MAC);
	i2c_m_sync_set_slaveaddr(&I2C_AT24MAC, CONF_AT24MAC_ADDRESS, I2C_M_SEVEN);
	io_write(&(I2C_AT24MAC.io), &addr, 1);
	io_read(&(I2C_AT24MAC.io), mac, 6);
#else
	/* set mac to 0x11 if no EEPROM mounted */
	memset(mac, 0x11, 6);
#endif
}

int main(void)
{
	int32_t ret;
	u8_t mac[6];

	atmel_start_init();

	/* Read MacAddress from EEPROM */
	read_macaddress(mac);

	// systick_enable();

	printf("\r\nHello ATMEL World!\r\n");

	printf("\r\nStarting ThreadX kernel.");

	tx_kernel_enter();

	mac_async_register_callback(&MACIF, MAC_ASYNC_RECEIVE_CB, (FUNC_PTR)mac_receive_cb);

	eth_ipstack_init();
	do
	{
		ret = ethernet_phy_get_link_status(&MACIF_PHY_desc, &link_up);
		if (ret == ERR_NONE && link_up)
		{
			break;
		}
	} while (true);
	printf("Ethernet Connection established\n");
	LWIP_MACIF_init(mac);

/* To use DHCP, read the document at documentation/lwip_raw.rst*/
#if LWIP_DHCP
	dhcp_start(&LWIP_MACIF_desc);
	printf("DHCP Started\n");
#else
	netif_set_up(&LWIP_MACIF_desc);
#endif

	mac_async_enable(&MACIF);

	while (true)
	{
		if (recv_flag)
		{
			recv_flag = false;
			ethernetif_mac_input(&LWIP_MACIF_desc);
		}
		/* LWIP timers - ARP, DHCP, TCP, etc. */
		sys_check_timeouts();

/* Print IP address info */
#if LWIP_VERSION_MAJOR == 2 && LWIP_VERSION_MINOR == 0
		if (link_up && LWIP_MACIF_desc.ip_addr.u_addr.ip4.addr)
#else
		if (link_up && LWIP_MACIF_desc.ip_addr.addr)
#endif
		{
			link_up = false;
			print_ipaddress();
		}
	}
}
