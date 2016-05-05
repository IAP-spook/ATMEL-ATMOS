/*
 * config.h
 *
 * Created: 10/6/2014 11:11:17 AM
 *  Author: Camden
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

//#define APP_ADDR 0x8001	// end-device
#define APP_ADDR 0x0000	// coordinator
#define APP_PANID 0x1234 // Network Identifier; Cannot use "0xffff" is reserved
#define APP_CHANNEL 0x0f // Frequency Channel; valid range is 2.4GHz radios (0x0b-0x1a) or refer to docs
#define APP_BAND 2
#define APP_MODULATION 0x35
#define APP_POWER 0x00
#define APP_SECURITY_KEY "SENSESecurityKey0"
#define APP_ENDPOINT 1

#if APP_ADDR == 0
	#define APP_CAPTION		"Coordinator"
	#define APP_COORDINATOR
	#define APP_SENDING_INTERVAL 1000
#elif APP_ADDR < 0x8000
	#define APP_CAPTION		"Router"
	#define APP_ROUTER
	#define APP_SENDING_INTERVAL 1000
#else
	#define APP_CAPTION		"End Device"
	#define APP_ENDDEVICE
	#define APP_SENDING_INTERVAL 10000
#endif

//#define NWK_ENABLE_ROUTING



#endif /* CONFIG_H_ */