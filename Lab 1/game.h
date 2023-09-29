#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"

#include "player.h"

typedef struct game_s {
  player_t players[2]; 
  player_t *player; 
  player_t *checker; // checker for asm code
  char textbuffer[64]; // Text buffer
  uint8_t battle_field_state[8][8]; // game state
} game_t;

char game_attempt_valid(game_t *game, int row, int col);  // return 1 if valid, 0 otherwise
void game_init(game_t *game, uint32_t rnd);
void game_display_state(game_t *game);
char game_done(game_t *game);
char game_target_is_new(game_t *game, int row, int col);
void game_get_target(game_t *game);
void game_set_target(game_t *game);
void game_display_battle_field(game_t *game);



