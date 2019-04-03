
/**FreeRTOS includes*/
#include "FreeRTOS.h"

/**Xilinx includes*/
#include "xil_printf.h"
#include "xparameters.h"

/** TA test includes */
# include "ta_led_blink.h"

/** Variables and functions definition.*/
#define LED_BLINK_FREQUENCY	5000000UL
#define	LED_BLINK_NUM		14

static void vTestLED( void );

/**---------------------------------------------------------------------*/
int main_blinky( void )
{

	/** 1. Print some messages. */
	xil_printf("----------------------------------------------------------\n\n");
	xil_printf( " ***Here is the TA_LED_BLINK!*** \r\n\t" );
	xil_printf( "		->|TA|: Blinking LED for a while... \r\n\t" );

	/**TODO: miss a function telling the mode.*/
	TickType_t	xCpuMode;
	__asm ("mrs %[result], cpsr"
	 : [result] "=r" (xCpuMode)
	 : :
	 );
	xil_printf( "		->|TA|: Current CPU mode 0x%08x... \r\n\t", xCpuMode );
	/** 2. Blink LED for a while. */
	vTestLED ();
	xil_printf( "		->|TA|: TA_LED_BLINK is done! \r\n\t", xCpuMode );

	return TRUE;
}

/**----------------------------------------------------------*/
static void vTestLED ( void )
{
	BaseType_t xInitLEDValue = 0x01;

	vTaLEDBlinkInit();
	vTaLEDBlinkSetLED( xInitLEDValue );
	for( UBaseType_t xTemp = 0; xTemp < LED_BLINK_NUM; xTemp++)
	{
		if (xTemp%2 == 0)
		{
			for(UBaseType_t i = 0; i < LED_BLINK_FREQUENCY; i++)
			{
				/* code: delay for a while.*/
			}
			vTaLEDBlinkSetLED( xInitLEDValue );
		}
		else
		{
			for(UBaseType_t i = 0; i < LED_BLINK_FREQUENCY; i++)
			{
				/* code: delay for a while.*/
			}
			vTaLEDBlinkSetLED( !xInitLEDValue );
		}
	}

}
