/*
 * @Author: elk.songran 
 * @Date: 2019-03-26 09:47:10 
 * @Last Modified by: elk.songran
 * @Last Modified time: 2019-03-26 10:24:41
 */

#ifndef TA_LED_BLINK_H
#define TA_LED_BLINK_H
#include "portmacro.h"

#define partstDEFAULT_PORT_ADDRESS		( ( uint16_t ) 0x378 )

void vTaLEDBlinkInit( void );
void vTaLEDBlinkSetLED( BaseType_t xValue );
void vTaLEDBlinkToggleLED(void );

/**TA*/
int main_blinky( void );

#endif /** TA_LED_BLINK_H */


