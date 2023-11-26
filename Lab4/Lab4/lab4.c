#include "stm32f4xx.h"
#include "stdio.h"
#include <string.h>
#include "svc.h"
#include "stock_exchange.h"

char textbuffer[34] = ""; // Text buffer
uint32_t volatile msTicks;                       // Counter for millisecond Interval

void SysTick_Handler (void) {                    // SysTick Interrupt Handler
  msTicks++;                                     // Increment Counter
  //__NVIC_SetPendingIRQ(EXTI0_IRQn);              // induce External Interrupt0
  //if ((msTicks & 0x100) == 0x100) __NVIC_SetPendingIRQ(EXTI1_IRQn); // induce External Interrupt1
  //if ((msTicks & 0x280) == 0x280) __NVIC_SetPendingIRQ(EXTI2_IRQn); // induce External Interrupt2
}


void sys_clock_timer_init(int period) {                
  if (SysTick_Config (SystemCoreClock / period)) { // SysTick period in seconds
    printf("\nError SysTick_Config!\n");  // Handle Error 
  }
}
 
void Device_Initialization (void)  {             // Configure & Initialize MCU
  sys_clock_timer_init (1000);            // setup device-specific timer
}

void EXTI0_IRQHandler (void) {
	printf("EXTI0_IRQHandler clk cnt 0x%x \n", msTicks);
}

void EXTI1_IRQHandler (void) {
	printf("EXTI1_IRQHandler clk cnt 0x%x \n", msTicks);
}

void EXTI2_IRQHandler (void) {
	printf("EXTI2_IRQHandler clk cnt 0x%x \n", msTicks);
}







int main(void)
{
  uint32_t num_transactions = 1000;
  SCB->CCR |= SCB_CCR_STKALIGN_Msk; // Enable double word stack alignment 
  //(recommended in Cortex-M3 r1p1, default in Cortex-M3 r2px and Cortex-M4)
  printf("\nHello world!\n");
  printf ("textbuffer address = 0x%x \n", (unsigned int) textbuffer);
	printf("String to pass to SVC handler zero \n");
	printf("String to pass to a different OS function \n");
	
  Device_Initialization ();
  
  //svc_zero("String to pass to SVC handler zero");
  //svc_one("String to pass to a different OS function"); 
	
  printf("Press Enter to initialize Stock Exchange:");
  fgets(textbuffer, (sizeof(textbuffer)-1), stdin);		
  stock_exchange_init( msTicks);
  
	NVIC_EnableIRQ	(	EXTI0_IRQn	);
	NVIC_EnableIRQ	(	EXTI1_IRQn	);
	NVIC_EnableIRQ	(	EXTI2_IRQn	);

  __set_CONTROL(1);                       // set thread mode as unprivillaged mode
  printf ("Stock Exchange is now open\n");
  while (num_transactions > 0) {
    stock_exchange_update(msTicks);
    num_transactions--;
  }
  printf ("Stock Exchange is now closed\n");
  stock_exchange_display_stats();
  printf ("Bye!\n");

	NVIC_DisableIRQ	(	EXTI0_IRQn	);
	NVIC_DisableIRQ	(	EXTI1_IRQn	);
	NVIC_DisableIRQ	(	EXTI2_IRQn	);
}
