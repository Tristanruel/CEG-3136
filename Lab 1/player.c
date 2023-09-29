#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"

#include "player.h"


int  player_get_num_pieces_destroyed_c (player_t * player){
  int i, j, n;
  n = 0;
  for (i=0; i<4; i++) { // piece type: size = type+1
    for (j=0; j<4-i; j++) { // piece index: count = 4-type
      if (player->pieces_hit[i][j] == i+1) n++;
    }
  }
  return n;
}

char player_done_c(player_t * player){
  char won = 1;
  int i, j;
  for (i=0; i<4; i++) { // piece type: size = type+1
    for (j=0; j<4-i; j++) { // piece index: count = 4-type
      if (player->pieces_hit[i][j] < i+1) {
        won = 0;
        break;
      }
    }
  }
  return won;
}

int update_points_c (player_t * player, uint8_t target_info){
  int points_urned = 0;
  player->nAttemps++;
  if (target_info & 0xFC) { // mask bit 0 & 1, which are used to flage game state
    if ((target_info & 0x2) == 0) { // not a spacer, i.e. on target
      // this is a target, get piece type & index
      int type, indx;
      player->nHits++;  // Hit
      type = (target_info & 0x1C) >> 2;
      indx = (target_info & 0x60) >> 5;
      player->pieces_hit[type-1][indx] ++;
      points_urned++;
      if (player->pieces_hit[type-1][indx] == type) { // the piece was fully destroyed!
        points_urned += type; // bonus points for destrying the piece
      }
    }    
  }
  player->points += points_urned;
  return points_urned;  
}

void player_init(player_t * player, code_t type){
  int i,j;
  player->nAttemps = 0;
  player->nHits = 0;
  player->points = 0;
  for (i=0; i<4; i++){
    for (j=0; j<4; j++){
      player->pieces_hit[i][j] = 0;
    }
  }
  switch (type) {
    case USE_C:         player->player_done = player_done_c; 
                        player->update_points = update_points_c;
                        player->player_get_num_pieces_destroyed = player_get_num_pieces_destroyed_c;
                        break;
    case USE_ASSEMBLY:  player->player_done = player_done_s; 
                        player->update_points = update_points_s;
                        player->player_get_num_pieces_destroyed = player_get_num_pieces_destroyed_s;
                        break;
  }    
}

void player_display_status(player_t * player) {
  printf ("Attempts %d, Hits %d, pieces destroyed %d, Points %d\n",
          player->nAttemps, player->nHits, player->player_get_num_pieces_destroyed(player), player->points);
}
