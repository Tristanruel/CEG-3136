#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"
#include "stimulus_gen.h"

uint32_t volatile msTicks;                       // Counter for millisecond Interval
char state_names[3][20] = {"OFF", "ON_HIGH", "ON_LOW"};

transmission_ctrl_t ctrl;
stimulus_gen_t      stim;
float               speed;
uint32_t            rpm;


void SysTick_Handler (void) {                    // SysTick Interrupt Handler
  msTicks++;                                     // Increment Counter
  stimulus_update(&stim);
  display_status(&stim);
}

void sys_clock_timer_init(int period) {                
  if (SysTick_Config (SystemCoreClock / period)) { // SysTick period in seconds
    printf("\nError SysTick_Config!\n");  // Handle Error 
  }
}
 
void Device_Initialization (void)  {             // Configure & Initialize MCU
  sys_clock_timer_init (1000);            // setup device-specific timer
  stim.speed = &speed;
  stim.rpm = &rpm;
  stim.ctrl = &ctrl;
  ctrl.speed = &speed;
  ctrl.rpm = &rpm;
  
}


int main(void)
{
  char textbuffer[40]; // Text buffer

  SCB->CCR |= SCB_CCR_STKALIGN_Msk; // Enable double word stack alignment 
  //(recommended in Cortex-M3 r1p1, default in Cortex-M3 r2px and Cortex-M4)
  printf("\nHello world!\n");
  printf("\n SystemCoreClock = %d \n", SystemCoreClock);
  printf ("Hit Enter to start the game: ");
  fgets(textbuffer, 64, stdin);	
  stimulus_init(&stim, msTicks);
  

  Device_Initialization ();
	
  while (1) {
  }
}


