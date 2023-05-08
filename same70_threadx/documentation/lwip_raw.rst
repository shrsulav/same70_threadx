=====================
LwIP Raw Data Example
=====================

The LwIP Raw Data example demonstrates the raw data process of LwIP TCP/IP
Stack. The user should connect the board to the network or computer by the
unshielded twisted pair(UTP) cable.

The example sends "Hello ATMEL World!" to the debug console by usart port, and
once the ethernet connection was established, the debug console will print
the static IP address and netmask address. The user can ping the board from
the host computer.

This example supports DHCP. To use DHCP
- when using LwIP v1.4.0, make sure to eanble the macros LWIP_DHCP (for LwIP stack) and CONF_LWIP_MACIF_DHCP (for MAC Interface)
- when using LwIP v2.0.3 and v2.1.2, make sure to enable macro LWIP_DHCP (for LwIP stack)

Drivers
-------
* TCPIP Stack
* TCPIP Stack Interface
* Ethernet PHY
* STDIO Redirect
* Ethernet MAC
* USART

Default interface settings
--------------------------
* USART

  * 9600 baud-rate
