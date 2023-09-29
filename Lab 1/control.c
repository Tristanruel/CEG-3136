#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"

#include "player.h"
#include "game.h"

game_t the_game;
uint32_t volatile msTicks;                       // Counter for millisecond Interval

void SysTick_Handler (void) {                    // SysTick Interrupt Handler
  msTicks++;                                     // Increment Counter
}

void sys_clock_timer_init(int period) {                
  if (SysTick_Config (SystemCoreClock / period)) { // SysTick period in seconds
    printf("\nError SysTick_Config!\n");  // Handle Error 
  }
}
 
int main(void)

{
  SCB->CCR |= SCB_CCR_STKALIGN_Msk; // Enable double word stack alignment 
  //(recommended in Cortex-M3 r1p1, default in Cortex-M3 r2px and Cortex-M4)
  sys_clock_timer_init (1000);            // setup device-specific timer
  printf("\nWelcome to the Battle Ship game!\n");
  // printf ("The game struct address is 0x%x \n",  &the_game);
  printf ("Hit Enter to start the game: ");
  fgets(the_game.textbuffer, 64, stdin);	
	game_init(&the_game, msTicks);
  game_display_battle_field(&the_game);
  do {
    game_get_target(&the_game);
    game_set_target(&the_game);
    game_display_battle_field(&the_game);
  } while (!game_done(&the_game));
  printf ("Game finished, you won! \n");
}
