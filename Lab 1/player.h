#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"

#ifndef __PLAYER_H__
#define __PLAYER_H__

typedef enum { USE_C, USE_ASSEMBLY } code_t;

typedef struct player_s{
  uint16_t nAttemps, nHits, points;
  uint8_t pieces_hit[4][4];

  char (*player_done)(struct player_s *player);
  int  (*player_get_num_pieces_destroyed) (struct player_s *player);
  int  (*update_points) (struct player_s *player, uint8_t target_info);
} player_t;

void player_init(player_t * player, code_t type);
void player_display_status(player_t * player);

char player_done_c(player_t * player);
int  player_get_num_pieces_destroyed_c (player_t * player);
int  update_points_c (player_t * player, uint8_t target_info);

extern char player_done_s(player_t * player);
extern int  player_get_num_pieces_destroyed_s (player_t * player);
extern int  update_points_s (player_t * player, uint8_t target_info);

#endif
