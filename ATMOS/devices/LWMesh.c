/*
 * LWMesh.c
 *
 * Created: 2/8/2016 12:39:53 PM
 *  Author: Carol
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "nwkRoute.h"
#include "nwkSecurity.h"
#include "sysTimer.h"
#include "common.h"
#include "commands.h"
#include "halBoard.h"
#include "halUart.h"
#include "halSleep.h"
#include "include/devices/LWMesh.h"


/*- Implementations -------------------------------------------------------------*/
static void appInit(void) {
	printf("State: APP_STATE_INITIAL\n");
	appMsg.nodeType			= APP_NODE_TYPE;
	appMsg.shortAddr		= APP_ADDR;
	appMsg.extAddr			= APP_ADDR; // TODO
	appMsg.parentShortAddr	= 0;
	appMsg.panId			= APP_PANID;
	appMsg.workingChannel	= APP_CHANNEL;
	appMsg.lqi				= 0;
	appMsg.rssi				= 0;
	
	appMsg.sensors.type		= 1; // TODO
	appMsg.sensors.size		= sizeof(int32_t) * 3; // TODO
	appMsg.sensors.temperature = 88; // TODO
	
	appMsg.caption.type		= 32;
	appMsg.caption.size		= APP_CAPTION_SIZE;
	memcpy(appMsg.caption.text, APP_CAPTION, APP_CAPTION_SIZE);

	HAL_BoardInit();
	//HAL_LedInit();
	
	/* Network Configuration */
	NWK_SetAddr(APP_ADDR);
	NWK_SetPanId(APP_PANID);
	
	PHY_SetChannel(APP_CHANNEL);
	//PHY_SetBand(APP_BAND); // only for sub-GHz radios
	//PHY_SetModulation(APP_MODULATION); // only for sub-GHz radios
	//PHY_SetTxPower(APP_POWER); // radio-dependent
	PHY_SetRxState(true);
	
	#ifdef NWK_ENABLE_SECURITY
	NWK_SetSecurityKey((uint8_t *)APP_SECURITY_KEY);
	#endif

	NWK_OpenEndpoint(APP_ENDPOINT, appDataInd);
	
	appDataSendingTimer.interval = APP_SENDING_INTERVAL;
	appDataSendingTimer.mode = SYS_TIMER_INTERVAL_MODE;
	appDataSendingTimer.handler = appDataSendingTimerHandler;
	
	#if defined(APP_ROUTER) || defined(APP_ENDDEVICE) // TODO
	appNetworkStatus				= false;
	
	appNetworkStatusTimer.mode		= SYS_TIMER_PERIODIC_MODE;
	appNetworkStatusTimer.interval	= 500; //ms
	appNetworkStatusTimer.handler	= appNetworkStatusTimerHandler;
	SYS_TimerStart(&appNetworkStatusTimer);
	
	appCommandWaitTimer.mode		= SYS_TIMER_INTERVAL_MODE;
	appCommandWaitTimer.interval	= NWK_ACK_WAIT_TIME;
	appCommandWaitTimer.handler		= appCommandWaitTimerHandler;
	
	#endif
	//APP_CommandsInit(); // TODO
	//appState = APP_STATE_SEND; // TODO
	
	printf("State: appInit end.\n");
}



//====  Receive Data =====
static bool appDataInd(NWK_DataInd_t *ind)
{
	/* TODO
	if (!appReadyToReceive)
	return false;
	*/
	AppMessage_t *msg = (AppMessage_t *)ind->data;
	
	msg->lqi	= ind->lqi;
	msg->rssi	= ind->rssi;
	appUartSendMessage(ind->data, ind->size);
	
	if (APP_CommandsPending(ind->srcAddr)) {
		NWK_SetAckControl(APP_COMMAND_PENDING); // APP_DO_NOT_SLEEP
	}
	
	printf("Receiving: temperature is %ld (0x%x)\n", msg->sensors.temperature, (unsigned int)msg->sensors.temperature);
	printf("Receiving: battery is %ld (0x%x)\n", msg->sensors.battery, (unsigned int)msg->sensors.battery);
	
	return true;
}

/*************************************************************************//**
*****************************************************************************/
#if defined(APP_ROUTER) || defined(APP_ENDDEVICE)
static void appDataConf(NWK_DataReq_t *req)
{
	if (NWK_SUCCESS_STATUS == req->status) {
		// frame was sent successfully
		if (!appNetworkStatus) {
			SYS_TimerStop(&appNetworkStatusTimer);
			appNetworkStatus = true;
		}
	} else {
		// some error happened
		if (appNetworkStatus) {
			SYS_TimerStart(&appNetworkStatusTimer);
			appNetworkStatus = false;
		}
	}
	
	if (APP_COMMAND_PENDING == req->control) {
		SYS_TimerStart(&appCommandWaitTimer);
		appState = APP_STATE_WAIT_COMMAND_TIMER;
		printf("State: APP_STATE_WAIT_COMMAND_TIMER\n");
	} else {
		appState = APP_STATE_SENDING_DONE;
		printf("State: APP_STATE_SENDING_DONE\n");
	}
}
#endif

/******************************************************************************//**
**********************************************************************************/
static void appDataSendingTimerHandler(SYS_Timer_t *timer)
{
	if (APP_STATE_WAIT_SEND_TIMER == appState) {
		appState = APP_STATE_SEND;
		printf("State: APP_STATE_SEND\n");
	} else {
		SYS_TimerStart(&appDataSendingTimer);
	}
	
	(void)timer;
}

#if defined(APP_ROUTER) || defined(APP_ENDDEVICE)
/*************************************************************************//**
*****************************************************************************/
static void appNetworkStatusTimerHandler(SYS_Timer_t *timer)
{
	(void)timer;
}

/*************************************************************************//**
*****************************************************************************/
static void appCommandWaitTimerHandler(SYS_Timer_t *timer)
{	
	(void)timer;
}
#endif

/******************************************************************************//**
**********************************************************************************/
static void appSendData(void) {
	#ifdef NWK_ENABLE_ROUTING
		appMsg.parentShortAddr = NWK_RouteNextHop(0, 0);
	#else
		appMsg.parentShortAddr = 0;
	#endif
		
	appMsg.sensors.temperature = 111; // Dummy data
	appMsg.sensors.battery = 50; // Dummy data
	
	#if defined(APP_COORDINATOR)
		appUartSendMessage((uint8_t *)&appMsg, sizeof(appMsg)); // TODO
		SYS_TimerStart(&appDataSendingTimer);
		appState = APP_STATE_WAIT_SEND_TIMER;
		printf("State: APP_STATE_WAIT_SEND_TIMER\n");
	#else
		//TODO: replace with real settings: ENDPOINT, ADDR, etc.
		appNwkDataReq.dstAddr = 0; // Send to coordinator
		appNwkDataReq.dstEndpoint = APP_ENDPOINT;
		appNwkDataReq.srcEndpoint = APP_ENDPOINT; //??
		appNwkDataReq.options = (NWK_OPT_ACK_REQUEST | NWK_OPT_ENABLE_SECURITY);
		appNwkDataReq.data  = (uint8_t*)&appMsg;
		appNwkDataReq.size = sizeof(appMsg);
		appNwkDataReq.confirm = appDataConf;
		
		NWK_DataReq(&appNwkDataReq);
		
		printf("Sending: temperature %ld (0x%x)\n", appMsg.sensors.temperature, (unsigned int)appMsg.sensors.temperature);
		printf("Sending: battery %ld (0x%x)\n", appMsg.sensors.battery, (unsigned int)appMsg.sensors.battery);
		printf("State: APP_STATE_WAIT_CONF\n");
		
		appState = APP_STATE_WAIT_CONF;
	#endif
}

/*************************************************************************//**
*****************************************************************************/
static void appUartSendMessage(uint8_t *data, uint8_t size) {
	// TODO
	  uint8_t cs = 0;

	  HAL_UartWriteByte(0x10);
	  HAL_UartWriteByte(0x02);

	  for (uint8_t i = 0; i < size; i++)
	  {
		  if (data[i] == 0x10)
		  {
			  HAL_UartWriteByte(0x10);
			  cs += 0x10;
		  }
		  HAL_UartWriteByte(data[i]);
		  cs += data[i];
	  }

	  HAL_UartWriteByte(0x10);
	  HAL_UartWriteByte(0x03);
	  cs += 0x10 + 0x02 + 0x10 + 0x03;

	  HAL_UartWriteByte(cs);
}

/******************************************************************************//**
**********************************************************************************/
static void APP_TaskHandler(void) {
	//TODO: Put your application code here
	switch(appState) {
		case APP_STATE_INITIAL:
		{
			appInit();
			break;
		}		
		case APP_STATE_SEND:
		{
			appSendData();
			break;
		}
		/*
		case APP_STATE_WAIT_CONF:
			break;
		*/
		case APP_STATE_SENDING_DONE:
		{
			#if defined(APP_ENDDEVICE)
				appState = APP_STATE_PREPARE_TO_SLEEP;
			#else
				SYS_TimerStart(&appDataSendingTimer);
				appState = APP_STATE_WAIT_SEND_TIMER;
			#endif
			break;
		}
		/*
		case APP_STATE_WAIT_SEND_TIMER:
			break;
		case APP_STATE_WAIT_COMMAND_TIMER:
			break;
		*/
		case APP_STATE_PREPARE_TO_SLEEP:
		{
			if (!NWK_Busy()) {
				NWK_SleepReq();
				appState = APP_STATE_SLEEP;
			}
			break;
		}
		case APP_STATE_SLEEP:
		{
			//HAL_LedClose();
			HAL_Sleep(APP_SENDING_INTERVAL);
			appState = APP_STATE_WAKEUP;
			break;
		}
		case APP_STATE_WAKEUP:
		{
			NWK_WakeupReq();
			//HAL_LedInit();
			//HAL_LedOn(APP_LED_NETWORK);
				
			appState = APP_STATE_SEND;
			break;
		}
		default: 
			break;
	}
}


/******************************************************************************//**
**********************************************************************************/
int LWMesh(void) {
	printf("Light weight mesh started.\n");
	SYS_Init();
	HAL_UartInit(38400);
	
	while (1)
	{
		SYS_TaskHandler();
		//HAL_UartTaskHander();
		APP_TaskHandler();
	}
}
