#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"
#include "transmission_ctrl.h"


typedef enum transmission__states { PARKING, REVERSE, NEUTRAL, DRIVING } transmission_state_e; 	


typedef struct stimulus_gen_s {
  float                 shaping_param;
  float                 *speed;     // vehicle speed Km/hr
  uint32_t              *rpm;       // revolutions per minute
  uint32_t              time_step;
  transmission_state_e  transmission_state;
  transmission_ctrl_t   *ctrl;
} stimulus_gen_t;

void stimulus_update(stimulus_gen_t *stim_gen);
void stimulus_init(stimulus_gen_t *stim_gen, uint32_t seed);
void display_status(stimulus_gen_t *stim);


