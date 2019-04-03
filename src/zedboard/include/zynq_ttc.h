
#ifndef __ZYNQ_TTC_H
#define __ZYNQ_TTC_H

#include"types.h"
#include"gic.h"
#include"platform_zynq.h"

/** TTC Interrupts IDs */
#define TTC0_TTCx_1_INTERRUPT	42
#define TTC0_TTCx_2_INTERRUPT	43
#define TTC0_TTCx_3_INTERRUPT	44
#define TTC1_TTCx_1_INTERRUPT	69
#define TTC1_TTCx_2_INTERRUPT	70
#define TTC1_TTCx_3_INTERRUPT	71

/** TTC Clock Control Register Flags */
#define TTC_CLK_CNTRL_EXT_EDGE 	(1 << 6) 
#define TTC_CLK_CNTRL_SRC     	(1 << 5) 
#define TTC_CLK_CNTRL_PS_EN  	(1 << 0)  
#define TTC_CLK_CNTRL_PS_VAL   	(4) 

/** TTC Counter Control Register Flags */
#define TTC_CNT_CNTRL_POL_WAVE	(1 << 6)        
#define TTC_CNT_CNTRL_EN_WAVE   (1 << 5)    
#define TTC_CNT_CNTRL_RST   	(1 << 4)   
#define TTC_CNT_CNTRL_MATCH 	(1 << 3) 
#define TTC_CNT_CNTRL_DEC  	(1 << 2) 
#define TTC_CNT_CNTRL_INT  	(1 << 1)   
#define TTC_CNT_CNTRL_DIS 	(1 << 0) 

/** TTC Interrupt Enable Register Flags */
#define TTC_INT_EN_EV    	(1 << 5) 
#define TTC_INT_EN_CNT_OVR 	(1 << 4)
#define TTC_INT_EN_MATCH3 	(1 << 3)  
#define TTC_INT_EN_MATCH2	(1 << 2)  
#define TTC_INT_EN_MATCH1 	(1 << 1) 
#define TTC_INT_EN_INTERVAL	(1 << 0)   

/** TTC Event Control Timer Register Flags */
#define TTC_EV_CNTRL_TIM_E_OV 	(1 << 2)
#define TTC_EV_CNTRL_TIM_E_LO	(1 << 1)
#define TTC_EV_CNTRL_TIM_E_EN 	(1 << 0)

/** TTC Reset Configuration value */
/*Output waveform enable & disable counter*/
#define TTC_RESET_CONFIG   	(0x21) 

/** TTC Values */
#define TTC_INT_VALUE    	(111)         
#define TTC_MATCH1_VALUE 	(111)     

/** Number of TTCs */
#define NUM_TTC    		2
/** Number of Timers per TTC */
#define NUM_TIM_PER_TTC  	3

/** TTC Number Definition */
#define TTC0      		0
#define TTC1        		1
/** TTC's Timer Number Definition */
#define TTCx_1      		0
#define TTCx_2      		1
#define TTCx_3      		2

/** TTC Modes */
#define INTERVAL       		0
#define MATCH          		1
#define FREE_RUNNING   		2

/** TTC registers structure */
typedef struct{
	/* Clock Control Register */
	volatile uint32_tee clk_cntrl[NUM_TIM_PER_TTC];
	/* Operational mode and reset */            
	volatile uint32_tee cnt_cntrl[NUM_TIM_PER_TTC];
	/* Current counter value */              
	volatile const uint32_tee cnt_value[NUM_TIM_PER_TTC];
	/* Interval value */       
	volatile uint32_tee interv_cnt[NUM_TIM_PER_TTC];
	/* Match values */            
	volatile uint32_tee match1_cnt[NUM_TIM_PER_TTC];
	volatile uint32_tee match2_cnt[NUM_TIM_PER_TTC];
	volatile uint32_tee match3_cnt[NUM_TIM_PER_TTC];
	/* Counter Interval, Match, Overflow and Event interrupts (RO) */          
	volatile const uint32_tee interrupt_reg[NUM_TIM_PER_TTC];
	/* */
	volatile uint32_tee interrupt_en[NUM_TIM_PER_TTC];
	/* */      
	volatile uint32_tee event_cntrl_tim[NUM_TIM_PER_TTC];
	/* */      
	volatile const uint32_tee event_reg[NUM_TIM_PER_TTC];
} Zynq_Ttc;

/**
 * TTC initialization
 *
 * @param 	ttc_num = TTC number
 * 		timer_num = TTC's timer number
 * 		mode = timer mode 	
 *
 * @retval	True for success and FALSE in case ERROR 	
 */
uint32_tee ttc_init(uint32_tee ttc_num, uint32_tee timer_num, uint32_tee mode);

/**
 * TTC enable
 *
 * @param 	ttc_num = TTC number
 * 		timer_num = TTC's timer number
 *
 * @retval	True for success and FALSE in case ERROR 	
 */
uint32_tee ttc_enable(uint32_tee ttc_num, uint32_tee timer_num);

/**
 * TTC disable
 *
 * @param 	ttc_num = TTC number
 * 		timer_num = TTC's timer number
 *
 * @retval	True for success and FALSE in case ERROR 	
 */
uint32_tee ttc_disable(uint32_tee ttc_num, uint32_tee timer_num);

/**
 * TTC timer request
 *
 * @param 	ttc_num = TTC number
 * 		timer_num = TTC's timer number
 *		useconds = time 
 *
 * @retval	True for success and FALSE in case ERROR 	
 */
uint32_tee ttc_request(uint32_tee ttc_num, uint32_tee timer_num, uint32_tee useconds);

/**
 * TTC interrupt clear
 *
 * @param 	interrupt = interrupt number 
 *
 * @retval	True for success and FALSE in case ERROR 	
 */
uint32_tee ttc_interrupt_clear(uint32_tee interrupt);

#endif /* __ZYNQ_TTC_H */
