/*
 * LWMesh.h
 *
 * Created: 2/8/2016 12:48:58 PM
 *  Author: Carol
 */ 


#ifndef LWMESH_H_
#define LWMESH_H_

/*- Definitions ------------------------- */
#if defined(APP_COORDINATOR)
#define APP_NODE_TYPE 0
#elif defined(APP_ROUTER)
#define APP_NODE_TYPE 1
#else
#define APP_NODE_TYPE 2
#endif

#if defined(APP_ROUTER) || defined(APP_ENDDEVICE)
static NWK_DataReq_t appNwkDataReq;
static SYS_Timer_t appNetworkStatusTimer;
static SYS_Timer_t appCommandWaitTimer;
static bool appNetworkStatus;
#endif

#define APP_CAPTION_SIZE (sizeof(APP_CAPTION) - 1)
#define APP_COMMAND_PENDING 0x01
#define APP_ENDPOINT 1

/*- Type -------------------------------- */
typedef struct PACK {
	uint8_t		nodeType;
	uint16_t	shortAddr;
	uint64_t	extAddr;
	uint16_t	parentShortAddr;
	uint16_t	panId;
	uint8_t		workingChannel;
	uint8_t		lqi; //filled in by the coordinator
	uint8_t		rssi; // filled in by the coordinator
	
	struct PACK
	{
		uint8_t	type;
		uint8_t	size;
		int32_t battery;
		int32_t temperature;
		//int32_t light;		
	} sensors;
	
	struct PACK
	{
		uint8_t type;
		uint8_t size;
		char text[APP_CAPTION_SIZE];
	} caption;	
} AppMessage_t;


typedef enum AppState_t
{
	APP_STATE_INITIAL,
	APP_STATE_SEND,
	APP_STATE_WAIT_CONF,
	APP_STATE_SENDING_DONE,
	APP_STATE_WAIT_SEND_TIMER,
	APP_STATE_WAIT_COMMAND_TIMER,
	APP_STATE_PREPARE_TO_SLEEP,
	APP_STATE_SLEEP,
	APP_STATE_WAKEUP,
} AppState_t;


/*- Variables -------------------------------------------------------------------*/
static AppState_t appState = APP_STATE_INITIAL;
static AppMessage_t appMsg;
static SYS_Timer_t appDataSendingTimer;

#if defined(APP_ROUTER) || defined(APP_ENDDEVICE)
static NWK_DataReq_t appNwkDataReq;
static SYS_Timer_t appNetworkStatusTimer;
static SYS_Timer_t appCommandWaitTimer;
static bool appNetworkStatus;
#endif


/*- Functions -------------------------------- */
static bool appDataInd(NWK_DataInd_t *ind);
static void appDataConf(NWK_DataReq_t *req);
static void APP_TaskHandler(void);
static void appInit(void);
static void appSendData(void);
static void appUartSendMessage(uint8_t *data, uint8_t size);
static void appDataSendingTimerHandler(SYS_Timer_t *timer);
static void appNetworkStatusTimerHandler(SYS_Timer_t *timer);
static void appCommandWaitTimerHandler(SYS_Timer_t *timer);
//bool APP_CommandsPending(uint16_t addr);


int LWMesh(void);


#endif /* LWMESH_H_ */