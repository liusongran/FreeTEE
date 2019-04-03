/*
 * @Author: elk.songran 
 * @Date: 2019-03-26 09:41:26 
 * @Last Modified by: elk.songran
 * @Last Modified time: 2019-03-26 10:24:53
 */

# include "xparameters.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo includes. */
#include "ta_led_blink.h"

/* Xilinx includes. */
#include "xgpiops.h"

#define tablinkNUM_LEDS			( 1 )
#define tablinkDIRECTION_OUTPUT	( 1 )
#define tablinkOUTPUT_ENABLED	( 1 )
#define tablinkLED_OUTPUT		( 7 )	/**LD9(MIO7) on Zedboard */

/*-----------------------------------------------------------*/

static XGpioPs xGpio;

/*-----------------------------------------------------------*/

void vTaLEDBlinkInit( void )
{
XGpioPs_Config *pxConfigTaBlink;
BaseType_t xStatus;

	/* Initialise the GPIO driver. */
	pxConfigTaBlink = XGpioPs_LookupConfig( XPAR_XGPIOPS_0_DEVICE_ID );
	xStatus = XGpioPs_CfgInitialize( &xGpio, pxConfigTaBlink, pxConfigTaBlink->BaseAddr );
	configASSERT( xStatus == XST_SUCCESS );
	( void ) xStatus; /* Remove compiler warning if configASSERT() is not defined. */

	/* Enable outputs and set low. */
	XGpioPs_SetDirectionPin( &xGpio, tablinkLED_OUTPUT, tablinkDIRECTION_OUTPUT );
	XGpioPs_SetOutputEnablePin( &xGpio, tablinkLED_OUTPUT, tablinkOUTPUT_ENABLED );
	XGpioPs_WritePin( &xGpio, tablinkLED_OUTPUT, 0x0 );
}
/*-----------------------------------------------------------*/

void vTaLEDBlinkSetLED( BaseType_t xValue )
{
	XGpioPs_WritePin( &xGpio, tablinkLED_OUTPUT, xValue );
}
/*-----------------------------------------------------------*/

void vTaLEDBlinkToggleLED( void)
{
BaseType_t xLEDState;

	xLEDState = XGpioPs_ReadPin( &xGpio, tablinkLED_OUTPUT );
	XGpioPs_WritePin( &xGpio, tablinkLED_OUTPUT, !xLEDState );
}



