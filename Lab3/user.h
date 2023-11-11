#include "stdio.h"
#include "stm32f4xx.h"


#ifndef __USER_H__
#define __USER_H__

typedef enum {LOGGED_OUT, LOGGED_IN} user_state_e; 

typedef struct user_s {
  uint16_t  id;
  user_state_e state;
  char pass_code[8];
  char name[51];
  char is_super_user;
	uint32_t logged_in_timestamp;
  uint32_t current_timestamp;
} user_t;

void user_init (user_t* user, uint16_t id, char* pass_code, char* name);

#endif // __USER_H__
