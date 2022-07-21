/*	discription:
	library makes sound.
		 		 
	controller:
	stm8s103
				
	was create:		21.11.17
							
	status: 		WORK

	need make:		NONE	

	other notes:  
	Vladimir Tyrkin, ru.veres@yandex.ru, veres.pcb@gmail.com .
*/
//#include "stdint.h"
//#include "stm32f10x.h"


typedef enum {	S_AT_END_PROGR = 0, 
		S_AT_START_PROGR = 1,  
		S_ERROR,
                S_CHANGE_STATUS} Sound_type;


void makeSound(uint8_t Sound_type);
void initSound(void);
void delay(void);

void makeSound(uint8_t Sound_type)
{
  uint8_t prescaler = 0,
          i = 0;
  
  if (Sound_type == S_AT_START_PROGR);
  
  if (Sound_type == S_CHANGE_STATUS){
        prescaler=6;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay();
        BEEP->CSR = 0;   
  }
  
  if (Sound_type == S_ERROR){
      for(i = 0; i<3; i++){
        prescaler = 4;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay(); 
        prescaler=6;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay();
        BEEP->CSR = 0;
      }
  }

  if (Sound_type == S_AT_END_PROGR){
        prescaler = 8;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay(); 
        prescaler=8;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay();   
 
        prescaler = 6;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay(); 
        prescaler=8;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay();
        
        prescaler = 7;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay(); 
        prescaler=6;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay();
        
        prescaler = 8;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay(); 
        prescaler=6;
        BEEP->CSR = prescaler|BEEP_CSR_BEEPEN;
        delay();
        delay();
        BEEP->CSR = 0;
  }
}	

void initSound(void)
{
  GPIOD->ODR &= ~(GPIO_PIN_4);  
  GPIOD->DDR |= (GPIO_PIN_4); 
  GPIOD->CR1 |= (GPIO_PIN_4);
  GPIOD->CR2 |= (GPIO_PIN_4);

//  BEEP->CSR = 0x0e;
//  BEEP->CSR |= 0x00|BEEP_CSR_BEEPEN;  // Beeper control/status register. beep enable
}

void delay(void)
{
  unsigned long int i = 0;
  for(i=0;i<200000;i++){  }; 
}