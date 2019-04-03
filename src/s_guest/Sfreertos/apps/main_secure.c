/*
 * @Author: elk.songran 
 * @Date: 2019-03-26 00:21:37 
 * @Last Modified by: elk.songran
 * @Last Modified time: 2019-03-26 16:00:33
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Xilinx includes. */
#include "xil_printf.h"
#include "xparameters.h"

/** TA includes */
#include "ta_led_blink.h"

/** SMC includes */
#include "secure_smc.h"

/** Variables & function definition.*/
#define TIMER_ID	1
#define DELAY_10_SECONDS	10000UL
#define DELAY_1_SECOND		1000UL
#define TIMER_CHECK_THRESHOLD	3

#define LED_BLINK_FREQUENCY	1000000UL
#define	LED_BLINK_NUM		35

/* The Tx and Rx tasks as described at the top of this file. */
static void prvTxTask( void *pvParameters );
static void prvRxTask( void *pvParameters );
static void vTimerCallback( TimerHandle_t pxTimer );
static void vTestLED( void );

/* The queue used by the Tx and Rx tasks, as described at the top of this
file. */
static TaskHandle_t xTxTask;
static TaskHandle_t xRxTask;
static QueueHandle_t xQueue = NULL;
static TimerHandle_t xTimer = NULL;
char HWstring[15] = "Hello World";
long RxtaskCntr = 0;

/*-----------------------------------------------------------*/
int main_secure( void )
{
	const TickType_t x10seconds = pdMS_TO_TICKS( DELAY_10_SECONDS );

	/** 1. Print some messages. */
	xil_printf("----------------------------------------------------------\n\n");
	xil_printf( " ***We are in Sfreertos now!*** \r\n\t" );
	xil_printf( " 		->|SFreeRTOS|: Hello guys... \r\n\t" );
	xil_printf( "       ->|SFreeRTOS|: Blinking LED for a while... \r\n\t" );
	
	TickType_t	xCpuMode;
	__asm ("mrs %[result], cpsr"
	 : [result] "=r" (xCpuMode)
	 : :
	 );
	xil_printf( "       ->|SFreeRTOS|: Current CPU mode 0x%08x... \r\n\t", xCpuMode );
	/** 2. Blink LED for a while. */
	vTestLED ();
	/**test MMU*/
	//YIELD();
	/** 3. install vector table. */
	/* The Xilinx projects use a BSP that do not allow the start up code to be
		altered easily.  Therefore the vector table used by FreeRTOS is defined in
		FreeRTOS_asm_vectors.S, which is part of this project.  Switch to use the
		FreeRTOS vector table. */
	vPortInstallFreeRTOSVectorTable();
	xil_printf( "		->|SFreeRTOS|: Installing vector table. DONE! \r\n\t" );
	/** 4. Test Sfreertos. */
	xil_printf( "		->|SFreeRTOS|: Testing Sfreertos... \r\n\t" );
	/* Create the two tasks.  The Tx task is given a lower priority than the
	Rx task, so the Rx task will leave the Blocked state and pre-empt the Tx
	task as soon as the Tx task places an item in the queue. */
	xTaskCreate( 	prvTxTask, 					/* The function that implements the task. */
					( const char * ) "Tx", 		/* Text name for the task, provided to assist debugging only. */
					configMINIMAL_STACK_SIZE, 	/* The stack allocated to the task. */
					NULL, 						/* The task parameter is not used, so set to NULL. */
					tskIDLE_PRIORITY,			/* The task runs at the idle priority. */
					&xTxTask );

	xTaskCreate( prvRxTask,
				 ( const char * ) "GB",
				 configMINIMAL_STACK_SIZE,
				 NULL,
				 tskIDLE_PRIORITY + 1,
				 &xRxTask );

	/* Create the queue used by the tasks.  The Rx task has a higher priority
	than the Tx task, so will preempt the Tx task and remove values from the
	queue as soon as the Tx task writes to the queue - therefore the queue can
	never have more than one item in it. */
	xQueue = xQueueCreate( 	1,						/* There is only one space in the queue. */
							sizeof( HWstring ) );	/* Each space in the queue is large enough to hold a uint32_t. */

	/* Check the queue was created. */
	configASSERT( xQueue );

	/* Create a timer with a timer expiry of 10 seconds. The timer would expire
	 after 10 seconds and the timer call back would get called. In the timer call back
	 checks are done to ensure that the tasks have been running properly till then.
	 The tasks are deleted in the timer call back and a message is printed to convey that
	 the example has run successfully.
	 The timer expiry is set to 10 seconds and the timer set to not auto reload. */
	xTimer = xTimerCreate( (const char *) "Timer",
							x10seconds,
							pdFALSE,
							(void *) TIMER_ID,
							vTimerCallback);
	/* Check the timer was created. */
	configASSERT( xTimer );

	/* start the timer with a block time of 0 ticks. This means as soon
	   as the schedule starts the timer will start running and will expire after
	   10 seconds */
	xTimerStart( xTimer, 0 );

	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}


/*-----------------------------------------------------------*/
static void prvTxTask( void *pvParameters )
{
const TickType_t x1second = pdMS_TO_TICKS( DELAY_1_SECOND );

	for( ;; )
	{
		/* Delay for 1 second. */
		vTaskDelay( x1second );

		/* Send the next value on the queue.  The queue should always be
		empty at this point so a block time of 0 is used. */
		xQueueSend( xQueue,			/* The queue being written to. */
					HWstring, /* The address of the data being sent. */
					0UL );			/* The block time. */
	}
}

/*-----------------------------------------------------------*/
static void prvRxTask( void *pvParameters )
{
char Recdstring[15] = "";

	for( ;; )
	{
		/* Block to wait for data arriving on the queue. */
		xQueueReceive( 	xQueue,				/* The queue being read. */
						Recdstring,	/* Data is read into this address. */
						portMAX_DELAY );	/* Wait without a timeout for data. */

		/* Print the received data. */
		xil_printf( "		->|Rx task|: received string from Tx task: %s\r\n\t", Recdstring );
		RxtaskCntr++;
	}
}

/*-----------------------------------------------------------*/
static void vTimerCallback( TimerHandle_t pxTimer )
{
	long lTimerId;
	configASSERT( pxTimer );

	lTimerId = ( long ) pvTimerGetTimerID( pxTimer );

	if (lTimerId != TIMER_ID) {
		xil_printf("		->|SFreeRTOS|: Hello World Example FAILED!\r\n\t");
	}

	/* If the RxtaskCntr is updated every time the Rx task is called. The
	 Rx task is called every time the Tx task sends a message. The Tx task
	 sends a message every 1 second.
	 The timer expires after 10 seconds. We expect the RxtaskCntr to at least
	 have a value of 9 (TIMER_CHECK_THRESHOLD) when the timer expires. */
	if (RxtaskCntr >= TIMER_CHECK_THRESHOLD) {
		xil_printf("		->|SFreeRTOS|: Hello World Example PASSED!\r\n\t");
	} else {
		xil_printf("		->|SFreeRTOS|: Hello World Example FAILED!\r\n\t");
	}
	vTaskDelete( xRxTask );
	vTaskDelete( xTxTask );

	/**
	 * Sfreertos Yield. Go back to Monitor mode
	 * 	and switch to Nfreertos executing.
	 */	
	xil_printf( "		->|SFreeRTOS|: We are going back to Monitor mode... \r\n\t" );
	/**TODO: */
	YIELD();

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
				/* code: dealy for a while.*/
			}
			vTaLEDBlinkSetLED( xInitLEDValue );
		}
		else
		{
			for(UBaseType_t i = 0; i < LED_BLINK_FREQUENCY; i++)
			{
				/* code: dealy for a while.*/
			}
			vTaLEDBlinkSetLED( !xInitLEDValue );
		}
	}
	
}
