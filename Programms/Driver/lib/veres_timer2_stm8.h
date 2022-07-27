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
#include <stm8s_tim2.h>
#include <stm8s_clk.h>


void TIMER2_Init(void);
void TIMER2_wait_msec(uint16_t msec);
void TIMER2_start(void);
void TIMER2_stop(void);								


/*
    EXAMPLE OF USE
#define debugOnUART

#ifdef  debugOnUART
UART_sendString(message);
#endif

*/


void TIMER2_Init(void)
{	
	// clock 
  CLK->PCKENR1 |= CLK_PCKENR1_TIM2;   // set clock to timer
	
  TIM2->PSCR = TIM2_PRESCALER_1024;
  TIM2->IER |= TIM2_IER_UIE;          // Update interrupt enable
  
//  TIM2->ARRH = 0x3D;                  // 15625 tics = 1 sec
//  TIM2->ARRL = 0x09;
  
  TIM2->CR1 |= TIM2_CR1_URS;          // Update interrupt enable
}

void TIMER2_wait_msec(uint16_t msec)  // maximum delay value = 0..4194 msec (0..65531 number)
{
  TIM2->CR1 &=  ~TIM2_CR1_CEN;      // stop counter 
  uint16_t q = (uint16_t)((msec * 15.625)+0.5);

  TIM2->ARRL = (uint8_t)q;  
  TIM2->ARRH = q>>8;                  // 
  TIM2->CR1 |=  TIM2_CR1_CEN;       // start counter!
}

void TIMER2_start(void)
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


/*


*/
