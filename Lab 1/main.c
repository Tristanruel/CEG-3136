#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"

#define PUMP_PIPE_DIAMETER      3	// 3 inch
#define COSUMER_PIPE_DIAMETER   2 // 2 inch
#define TANK_DIAMETER          50 // 

#define ALMOST_EMPTY      5// 3 inch
#define ALMOST_FULL       25 // 2 inch
#define TANK_FULL         30 // 

uint32_t volatile msTicks;                       // Counter for millisecond Interval
float pump_flow = 1;
float tank_height = 0;
float pump_flow_to_height_rate, consumer_flow_to_height_rate;


enum pump_States { OFF, ON_HIGH, ON_LOW } pump_state; 	// declare variable as pump state enum
char state_names[3][20] = { "OFF", "ON_HIGH", "ON_LOW" };

void update_tank_height(void);
void TickFct(void);

void SysTick_Handler (void) {                    // SysTick Interrupt Handler
  msTicks++;                                     // Increment Counter
  update_tank_height();
	TickFct();
}

void sys_clock_timer_init(int period) {                
  if (SysTick_Config (SystemCoreClock / period)) { // SysTick period in seconds
    printf("\nError SysTick_Config!\n");  // Handle Error 
  }
}
 
void Device_Initialization (void)  {             // Configure & Initialize MCU
  sys_clock_timer_init (1000);            // setup device-specific timer
  pump_flow_to_height_rate = 1.0 * PUMP_PIPE_DIAMETER * PUMP_PIPE_DIAMETER / (TANK_DIAMETER * TANK_DIAMETER);
  consumer_flow_to_height_rate = 1.0 * COSUMER_PIPE_DIAMETER * COSUMER_PIPE_DIAMETER / (TANK_DIAMETER * TANK_DIAMETER);
	pump_state = OFF;
}

void display_status(void){
    printf(" pump is %s \t new_height = %f\n", state_names[pump_state], tank_height);
}

int main(void)
{
  //char textbuffer[40]; // Text buffer

  SCB->CCR |= SCB_CCR_STKALIGN_Msk; // Enable double word stack alignment 
  //(recommended in Cortex-M3 r1p1, default in Cortex-M3 r2px and Cortex-M4)
  printf("\nHello world!\n");
  printf("\n SystemCoreClock = %d \n", SystemCoreClock);

  Device_Initialization ();
	
  while (1) {
    display_status();
  }
}

void update_tank_height(){
  float new_height =  tank_height = tank_height + 
                      pump_flow * pump_flow_to_height_rate - 
	                    rand() * consumer_flow_to_height_rate / RAND_MAX; 
	if (new_height >= 0) tank_height = new_height;
	else                 tank_height = 0;
}

void TickFct(void){
  switch (pump_state) {
    case(OFF): if (tank_height <= ALMOST_EMPTY) {
      pump_state = ON_HIGH;
      pump_flow = 1;
    }
    break;
    case(ON_HIGH): if (tank_height > ALMOST_FULL) {
      pump_state = ON_LOW;
      pump_flow = 0.5;
    }
    break;
    case(ON_LOW): if (tank_height >= TANK_FULL) {
      pump_state = OFF;
      pump_flow = 0;
    }
    break;
  }
}
