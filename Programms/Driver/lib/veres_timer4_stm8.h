/*	discription:
	library using for send data via UART. stm8
				
	was create:	  28.07.14
	rewrite to stm8	  16.11.17
	update:		  19.11.17
	status: 	  WORK

	need make:	  none	

	other notes:      use as #include "veres_timer2_stm8.h"

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
#include <stm8s_tim4.h>
#include <stm8s_clk.h>


void TIMER4_Init(void);
void TIMER4_wait_msec(uint16_t msec);
void TIMER4_start(void);
void TIMER4_stop(void);								


/*
    EXAMPLE OF USE
#define debugOnUART

#ifdef  debugOnUART
UART_sendString(message);
#endif

*/


void TIMER4_Init(void)
{	
	// clock 
  CLK->PCKENR1 |= CLK_PCKENR1_TIM4;   // set clock to timer
	
  TIM4->PSCR = TIM4_PRESCALER_128;
  TIM4->IER |= TIM4_IER_UIE;          // Update interrupt enable
  
//  TIM4->ARRH = 0x3D;                  // 15625 tics = 1 sec
//  TIM4->ARRL = 0x09;
  
  TIM4->CR1 |= TIM4_CR1_URS;          // Update interrupt enable
}

void TIMER4_wait_msec(uint16_t msec)  // maximum delay value = 0..4194 msec (0..65531 number)
{
  TIM4->CR1 &=  ~TIM4_CR1_CEN;      // stop counter 
  uint16_t q = (uint16_t)((msec * 15.625)+0.5);

  TIM4->ARR = (uint8_t)q;  
//  TIM4->ARRH = q>>8;                  // 
  TIM4->CR1 |=  TIM4_CR1_CEN;       // start counter!
}

void TIMER4_start(void)
{
  TIM4->CR1 &=  ~TIM4_CR1_CEN;      // stop counter
  TIM4->CNTR = 0;                  // write 0 to counter
//  TIM4->CNTRH = 0;                  //
     
  TIM4->CR1 |=  TIM4_CR1_CEN;       // start counter!
}		

void TIMER4_stop(void)
{
  TIM4->CR1 &=  ~TIM4_CR1_CEN;      // stop counter
}


/*


*/
