/*
 * @Author: elk.songran 
 * @Date: 2019-03-26 01:19:23 
 * @Last Modified by: elk.songran
 * @Last Modified time: 2019-03-26 01:27:55
 */

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


/* Xilinx includes */
#include "xil_printf.h"
#include "xparameters.h"

/** TA library includes */
#include "ta_led_blink.h"
/**FIXME: second half.
 * 	#include "ta_hello_world.h"
 * 	#include "ta_watering_led.h"
 */

/**TODO: Need LTZVISOR_MON_EXIT_WITH_ARG!!! */
/**
 * Entry of TA selection. (worked in sys mode and in task not baremetal.)
 *
 * @param
 *
 * @retval Return TRUE if success or False if not
 */
int main_monitor( unsigned int xTaSelection )
{
int ret;

	#if( xTaSelection == 0 )
	{
		/**TODO: go to supervisor mode first */
        ret = main_blinky();
        xil_printf( "		->|SFreeRTOS|: Return the result and go back to Monitor mode... \r\n\t");
        return ret;
	}
	#elif( xTaSelection == 1 )
	{
		ret = main_blinky();
        xil_printf( "		->|SFreeRTOS|: Return the result and go back to Monitor mode... \r\n\t");
        return ret;
	}
	#elif( xTaSelection == 2)
	{
		ret = main_blinky();
        xil_printf( "		->|SFreeRTOS|: Return the result and go back to Monitor mode... \r\n\t");
        return ret;
	}
    else
    {
        /**return error. */
    	xil_printf( "		->|SFreeRTOS|: Return the result and go back to Monitor mode... \r\n\t");
    	return FALSE;
    }  
	#endif

}
