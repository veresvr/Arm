/*

pinout:
PA3 - MODE_BTN (EXTR INTERRUPT)
PD4 - JOYSTICK_BTN
PD6 - SCND_BTN
PB4 - A_PLUS
PB5 - A_MINUS
PD2 - B_ANALOG
PD3 - E_ANALOG
PC3 - C_PLUS
PC4 - C_MINUS
PC5 - D_PLUS
PC6 - D_MINUS
PC7 - 

PD5 -> UART1-TX
PD1 -> SWIM

*/

#include <stm8s.h>
#include <stm8s_gpio.h>
#include <stm8s_clk.h>
#include "veres_UART_stm8.h"
#include "veres_timer2_stm8.h"
#include "veres_err_list_stm8.h"
#include "veres_adc1_stm8.h"
//#include "veres_ds18b20_stm8.h"



#define	DELAY_OF_TRANSMISSION	20		// (milliseconds: 1 - 999) time of delay between data packets
#define	LENGHT	                2		// value of array
#define	DATA_INC_READY          TRUE		// ready for parsing packet
#define	DATA_INC_NOREADY        FALSE		// not ready for parsing packet

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

// value edges 
#define	VALUE_PLUS_MAX            255		//
#define	VALUE_PLUS_MEDIUM         216		// 
#define	VALUE_PLUS_LOW            182		//
#define	VALUE_STOP                144		//
#define	VALUE_MINUS_LOW           109		//
#define	VALUE_MINUS_MEDIUM        72		// 
#define	VALUE_MINUS_MAX           36		//

// button 2bit variants
#define	BUTTON_MOD0               0x0		//
#define	BUTTON_MOD1               0x1		//
#define	BUTTON_MOD2               0x2		// reserved
#define	BUTTON_MOD3               0x3		// reserved

// variables
uint8_t lenghtOfDataPacket = 0,
	receive_array[LENGHT],
        transmit_array[LENGHT],
	status = 0,
        statusData = DATA_INC_NOREADY,
        err_code = NO_ERROR,
        lenghtData = 0,
        adcValue = 0;

// structures
struct flag{
  unsigned changeStatus: 1; 
  unsigned soundAtEnd: 1;
  unsigned soundAtError: 1;  
  unsigned dataOutReadySend: 1; 
} statusFlag;

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
// unions



// interrupt definitions
 INTERRUPT_HANDLER( UART1_RX, 0x12 );
 INTERRUPT_HANDLER( TIM2_OVF, 13 );

// prototypes
 void init_GPIO(void);
 uint8_t valueToVariant(uint8_t value);
  
int main( void )
{
  CLK->CKDIVR &= ~(CLK_CKDIVR_HSIDIV);          // fHSI= fHSI RC output (configure to 16 MHz)
  
  UART_Init();
  TIMER2_Init();
  init_GPIO();
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
  
  statusFlag.dataOutReadySend = FALSE;
  
  while(1){
    
    // check all buttons
    
    
    
    /*
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
        if ((receive_array[3] == 'E'))  sound_flag.changeStatus = 1;
        if ((receive_array[3] == 'D'))  sound_flag.changeStatus = 0;
          
        continue;
      }   
      
      if((receive_array[1] == 'B') && (receive_array[2] == 'C'))   // 
      {
        continue;
      }      
      
      if((receive_array[1] == 'S') && (receive_array[2] == 'E'))   // errors occur sound        ok
      {
        if ((receive_array[3] == 'E'))  sound_flag.soundAtError = 1;
        if ((receive_array[3] == 'D'))  sound_flag.soundAtError = 0;
           
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

      
    }  */
    
    // here we collect all button states
    
    // check 2 analog pins
      
    while (statusFlag.dataOutReadySend != TRUE);       

    UART_sendData(firstByte.data);
    UART_sendData(secondByte.data);
    UART_sendData(thirdByte.data);
    
    statusFlag.dataOutReadySend = FALSE;
    TIMER2_wait_msec(DELAY_OF_TRANSMISSION);
  
    
    
    
    

  
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
	
  TIMER2_wait_msec(20);  
}

__interrupt void TIM2_OVF( void )
{
  TIM2->SR1 &= ~TIM2_SR1_UIF;                           // clear an interrupt flag
  
  TIMER2_stop();
  statusFlag.dataOutReadySend = TRUE;

}

void init_GPIO(void)
{
	
// pins Floating input without interrupt
//  GPIOD->ODR &= ~(GPIO_PIN_5);                // output data register
  
  GPIOB->DDR &= ~(GPIO_PIN_4|GPIO_PIN_5);         // data direction register
  GPIOC->DDR &= ~(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);
  GPIOD->DDR &= ~(GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6);
  
  GPIOB->CR1 &= ~(GPIO_PIN_4|GPIO_PIN_5);         // control register 1 Floating input
  GPIOC->CR1 &= ~(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);
  GPIOD->CR1 &= ~(GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6);
  
  GPIOB->CR2 &= ~(GPIO_PIN_4|GPIO_PIN_5);         // control register 2 External interrupt disabled
  GPIOC->CR2 &= ~(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);
  GPIOD->CR2 &= ~(GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6);
  
// MODE_BTN
  GPIOA->DDR &= ~(GPIO_PIN_3);         // data direction register
  GPIOA->CR1 &= ~(GPIO_PIN_3);         // control register 1 Floating input
  GPIOA->CR2 |= (GPIO_PIN_3);         // control register 2 External interrupt enabled


  
}

 uint8_t valueToVariant(uint8_t value)
 {
   if (value<=VALUE_STOP) {
     
     if (value<=VALUE_MINUS_MEDIUM) {
       if (value<=VALUE_MINUS_MAX) return VALUE_MINUS_MAX;
         else return VALUE_MINUS_MEDIUM;
     }else{
       if (value<=VALUE_MINUS_LOW) return VALUE_MINUS_LOW;
         else return VALUE_STOP;
     }
   }else{
     
     if (value<=VALUE_PLUS_MEDIUM) {
       if (value<=VALUE_PLUS_LOW) return VALUE_PLUS_LOW;
         else return VALUE_PLUS_MEDIUM;
     }else{
         return VALUE_PLUS_MAX;
     } 
   }
 }











