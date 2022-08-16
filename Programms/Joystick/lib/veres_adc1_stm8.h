/*	discription:
	library using for send data via UART. stm8
				
	was create:	  28.07.14
	rewrite to stm8	  16.11.17
	update:		  19.11.17
	status: 	  WORK

	need make:	  none	

	other notes:      use as #include "stm8s_adc1.h"

For using interrupts, u must write next code:

  rim(); 

  INTERRUPT_HANDLER( TIM2_OVF, 13 );

  __interrupt void TIM2_OVF( void )
  {
    ChgBit(GPIOB->ODR,5);
    TIM2->SR1 &= ~TIM2_SR1_UIF;       // clear an interrupt flag 
  }

*/
#include <stm8s_gpio.h>
#include <stm8s_adc1.h>
#include <stm8s_clk.h>


void adcInit(void);
uint8_t adcGetValue(uint8_t axis);							


/*
    EXAMPLE OF USE
#define debugOnUART

#ifdef  debugOnUART
UART_sendString(message);
#endif

*/


void adcInit(void)
{	
	// clock 
  CLK->PCKENR1 |= CLK_PCKENR1_TIM2;   // set clock to timer
	
  TIM2->PSCR = TIM2_PRESCALER_1024;
  TIM2->IER |= TIM2_IER_UIE;          // Update interrupt enable
  
//  TIM2->ARRH = 0x3D;                  // 15625 tics = 1 sec
//  TIM2->ARRL = 0x09;
  
  TIM2->CR1 |= TIM2_CR1_URS;          // Update interrupt enable
}

uint8_t adcGetValue(uint8_t axis)  // maximum delay value = 0..4194 msec (0..65531 number)
{




  
  return 0;
}

/*void TIMER2_start(void)
{
  TIM2->CR1 &=  ~TIM2_CR1_CEN;      // stop counter
  TIM2->CNTRL = 0;                  // write 0 to counter
  TIM2->CNTRH = 0;                  //
     
  TIM2->CR1 |=  TIM2_CR1_CEN;       // start counter!
}		

void TIMER2_stop(void)
{
  TIM2->CR1 &=  ~TIM2_CR1_CEN;      // stop counter
}
*/

/*


*/
