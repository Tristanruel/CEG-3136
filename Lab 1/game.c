#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stm32f4xx.h"

#include "player.h"
#include "game.h"


char game_target_is_new(game_t *game, int row, int col){  // return 1 if valid, 0 otherwise
  char valid;
  if (game->battle_field_state[row][col] & 1) valid = 0;
  else valid = 1;
  return valid;
}

void game_init(game_t *game, uint32_t rnd){
  uint8_t battle0[8][8] = {
    2,100,2,2,2,2,2,2,
    2,2,2,12,12,12,2,16,
    8,8,2,2,2,2,2,16,
    2,2,2,44,2,2,2,16,
    2,2,44,2,2,68,2,16,
    2,44,2,2,2,2,2,2,
    2,2,2,40,2,72,2,36,
    4,2,40,2,2,72,2,2  };
  uint8_t battle1[8][8] = {
    100,2,2,12,2,2,2,72,
    2,2,12,2,2,16,2,72,
    2,12,2,2,16,2,2,2,
    2,2,2,16,2,2,2,40,
    8,2,16,2,2,2,40,2,
    8,2,2,2,44,2,2,2,
    2,2,2,44,2,2,2,2,
    68,2,44,2,2,36,2,4};
  uint8_t battle2[8][8] = {
    2,2,2,40,2,100,2,44,
    2,16,2,40,2,2,2,44,
    2,16,2,2,2,68,2,44,
    2,16,2,4,2,2,2,2,
    2,16,2,2,2,8,2,2,
    2,2,2,2,8,2,2,12,
    2,72,2,2,2,2,12,2,
    72,2,2,36,2,12,2,2};
  uint8_t battle3[8][8] = {
    100,2,16,16,16,16,2,4,
    2,2,2,2,2,2,2,2,
    2,40,2,2,44,44,44,2,
    2,40,2,2,2,2,2,2,
    2,2,2,68,2,36,2,0,
    8,2,2,2,2,2,2,2,
    8,2,12,12,12,2,72,2,
    2,2,2,2,2,2,2,72};
  
  uint32_t battle = rnd & 3;
  printf ("Playing Battle%d, rnd = %d\n", battle, rnd);
  game->textbuffer[0] = '\0';
  game->player = &game->players[0];
  game->checker = &game->players[1];

    switch (battle) {
    case 0: memcpy(game->battle_field_state, battle0, 64); break;
    case 1: memcpy(game->battle_field_state, battle1, 64); break;
    case 2: memcpy(game->battle_field_state, battle2, 64); break;
    case 3: memcpy(game->battle_field_state, battle3, 64); break;
  }
  

  player_init(game->player, USE_C);  // the main game use C
  player_init(game->checker, USE_ASSEMBLY); // the checker use Assembly
}
void game_get_target(game_t *game){
  char done = 0;
  printf("Enter target coordinates in the range A0 ~ H7 ");
  do {
    fgets(game->textbuffer, (sizeof(game->textbuffer)-1), stdin);	
    if (game->textbuffer[0] < 'A' || game->textbuffer[0] > 'H' || game->textbuffer[1] < '0' || game->textbuffer[1] > '7') {
      printf ("Illegal target, targets must be in the range A0 ~ H7 \n Enter target coordinates in the range A0 ~ H7 ");
    }
    else if (game_target_is_new(game, game->textbuffer[0] - 'A', game->textbuffer[1] - '0') == 0) {
      printf ("This target was hit already, pick another target! \n Enter target coordinates in the range A0 ~ H7 ");
    }
    else done = 1;
  } while (!done);
}
void game_set_target(game_t *game){
  int row = game->textbuffer[0] - 'A';
  int col = game->textbuffer[1] - '0';
  int points_c, points_s;
  game->battle_field_state[row][col] |= 1;  // set played bit
  points_c = game->player->update_points(game->player, game->battle_field_state[row][col]);
  points_s = game->checker->update_points(game->checker, game->battle_field_state[row][col]);
  if (points_c != points_s) {
    printf ("Error: attempt points mismatch [c] %d, [s] %d --===XXXX===--\n", points_c, points_s);
  }
  if (points_c > 1) {
    int i, j, m, n;
    for (i=0; i<8; i++){
      for (j=0; j<8; j++){
        if (game->battle_field_state[i][j] == game->battle_field_state[row][col]) { // on the same piece
          for (m=i-1; m<=i+1; m++){
            if (m < 0 || m > 7) continue;
            for (n=j-1; n<=j+1; n++){
              if (n < 0 || n > 7) continue;
              if (game->battle_field_state[m][n] == 2) {  // unplayed filler cell
                game->battle_field_state[m][n] = 3; // played filler cell
              }
            }
          }
        }
      }
    }
  }
}

void game_display_battle_field(game_t *game){
  int i, j;
  uint8_t state;
  printf ("  7 6 5 4 3 2 1 0 \n");
   for(i=0; i<8; i++) {
     printf ("%c ", 'A' + i);
      for (j=7; j>=0; j--){
        state = game->battle_field_state[i][j];
        if (state & 0x1) { // played
          state = state >> 1; // git rid of played bit
          if (state == 0) { // no target
            printf ("- ");
          }
          else if (state & 0x1) { // spacer
            printf ("o ");
          }
          else {  // target
            printf ("x ");
          }
        }
        else printf(". ");
      }
      printf ("\n");
   }
   i = game->player->player_get_num_pieces_destroyed(game->player);
   j = game->checker->player_get_num_pieces_destroyed(game->checker);
   if (i != j) printf ("Error: player_get_num_pieces_destroyed check failed [c] %d, [s] %d --===XXXX===--\n", i, j);
    player_display_status(game->player);
}

char game_done(game_t *game){
  char c, s;
  c = game->player->player_done(game->player);
  s = game->checker->player_done(game->checker);
  if (c != s) printf ("Error: game_done mismatch [c] %d, [s] %d --===XXXX===--\n", c, s);
  return c;
}

