#include <stm8s.h>
#include <stm8s_gpio.h>
#include <stm8s_clk.h>
#include <veres_UART_stm8.h>
#include <veres_timer2_stm8.h>
#include <veres_timer4_stm8.h>
#include <veres_err_list_stm8.h>

//#define DEBUG

#define	DELAY_OF_DATA		20		// time of delay before next data packet
#define	LENGHT	                30		// value of array
#define	DATA_INC_READY          1		// ready for parsing packet
#define	DATA_INC_NOREADY        0		// not ready for parsing packet

// mode variants (00b , 11b - reserved)
#define	MODE_HAND               0x1		// 
#define	MODE_WRIST              0x2		// 

// speed variants
#define	SPEED_STOP                0x0		// 
#define	SPEED_PLUS_LOW            0x1		//
#define	SPEED_PLUS_MEDIUM         0x2		// 
#define	SPEED_PLUS_MAX            0x3		//
#define	SPEED_MINUS_LOW           0x4		//
#define	SPEED_MINUS_MEDIUM        0x5		// 
#define	SPEED_MINUS_MAX           0x6		//

// button 2bit variants
#define	BUTTON_MOD0               0x0		//
#define	BUTTON_MOD1               0x1		//
#define	BUTTON_MOD2               0x2		// reserved
#define	BUTTON_MOD3               0x3		// reserved

// variables
uint8_t lenghtOfDataPacket = 0,
	receive_array[LENGHT],
	status = 0,
        statusData = DATA_INC_NOREADY,
        err_code = NO_ERROR,
        lenghtData = 0;

// structures
struct byte1{
  uint8_t data;
  unsigned mode: 2; 
  unsigned directionA: 3;
  unsigned directionB: 3;  
} firstByte;

struct byte2{
  uint8_t data;
  unsigned directionC: 3;
  unsigned directionD: 3;  
  unsigned buttonF1: 2;
} secondByte;

struct byte3{
  uint8_t data;
  unsigned directionE: 3;
  unsigned directionF: 3;               // reserved
  unsigned buttonF2: 2;                 // reserved
} thirdByte;

// interrupt definitions
 INTERRUPT_HANDLER( UART1_RX, 0x12 );
 INTERRUPT_HANDLER( TIM2_OVF, 13 );
  
int main( void )
{
  CLK->CKDIVR &= ~(CLK_CKDIVR_HSIDIV);    // fHSI= fHSI RC output (configure to 16 MHz)
  
  UART_Init();
  TIMER2_Init();
  rim();  
  
//  for led
  GPIOB->ODR |= (GPIO_PIN_5);
  GPIOB->DDR |= (GPIO_PIN_5);
  GPIOB->CR1 |= (GPIO_PIN_5);  
  GPIOB->CR2 |= (GPIO_PIN_5);
  
// default states
  firstByte.mode = MODE_HAND;
  firstByte.directionA = SPEED_STOP;
  firstByte.directionB = SPEED_STOP;
  secondByte.directionC = SPEED_STOP;
  secondByte.directionD = SPEED_STOP;
  secondByte.buttonF1 = BUTTON_MOD0;
  thirdByte.directionE = SPEED_STOP; 
  
  
  
  while(1){
    
    if(statusData == DATA_INC_READY){
      
      statusData = DATA_INC_NOREADY;
      // UART interrupt must be OFF, for parsing reseived data packet
      if(lenghtData < 3)                                          // check for minimal lenght of data   ok
      { 
        err_code = COMMAND_LESS_3_CHAR;
        UART_sendString("COMMAND_LESS_3_CHAR  ");	          // these and next transmissions only for debug

        continue;
      }
			
      if((receive_array[0] != '[') || (receive_array[lenghtData-1] != ']'))   //check first and last char of data   ok
      {
        err_code = START_STOP_UNCORR; 
        UART_sendString("START_STOP_UNCORR  ");                   // only for debug, clear before release!

        continue;
      } 
      

      if((receive_array[1] == 'R') && (receive_array[2] == 'T'))   // read temperature command        half
      {
        // code for read temperature

        continue;
      }
      
      if((receive_array[1] == 'S') && (receive_array[2] == 'A'))   // change status sound        ok
      {
        if ((receive_array[3] == 'E'))  ;
        if ((receive_array[3] == 'D'))  ;
        

        continue;
      }   
      
      if((receive_array[1] == 'B') && (receive_array[2] == 'C'))   // 
      {
 
        continue;
      }      
      
      if((receive_array[1] == 'S') && (receive_array[2] == 'E'))   // errors occur sound        ok
      {
        if ((receive_array[3] == 'E'))  ;
        if ((receive_array[3] == 'D'))  ;
        

        continue;
      }

      if((receive_array[1] == 'B') && (receive_array[2] == 'E'))   // blow state toggle        half
      {
        // code for blow
 
        continue;
      }    
      
      if((receive_array[1] == 'I') && (receive_array[2] == 'D'))   // check ID of device        half
      {
        UART_sendString("[l9]");

        continue;
      }

      
    }  
      
      
//  UART_sendOnlyNumber(receive_array[lenghtData-1]);
//  delay();
//  delay();  
  
    
    
    
    

  
  }  
  return 0;
}
__interrupt void UART1_RX( void )
{  
  uint8_t received_data = 0;
	
  TIMER2_stop();
  received_data = UART1->DR;
  receive_array[lenghtOfDataPacket] = received_data;
  lenghtOfDataPacket++;
	
  TIMER2_wait_msec(DELAY_OF_DATA);  
}

__interrupt void TIM2_OVF( void )
{
  TIM2->SR1 &= ~TIM2_SR1_UIF;                           // clear an interrupt flag
  
  TIMER2_stop();
  #ifdef DEBUG
    UART_sendString("Time is gone. Data receive is ");    // only for debug, clear before release!
  #endif //DEBUG
    
  if (lenghtOfDataPacket == 0){
    #ifdef DEBUG
      UART_sendString("zero");                            // only for debug, clear before release!
  #endif //DEBUG
      statusData = DATA_INC_NOREADY;
  } else {
    UART_sendOnlyNumber(lenghtOfDataPacket+48);         // only for debug, clear before release!
    statusData = DATA_INC_READY;
  }
  
  lenghtData = lenghtOfDataPacket;
  lenghtOfDataPacket = 0;

}