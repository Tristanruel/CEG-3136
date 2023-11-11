#include "stimulus_gen.h"
#include "math.h"

void stimulus_init(stimulus_gen_t *stim_gen, uint32_t seed){
  *stim_gen->speed = 0;
  stim_gen->time_step = 0;
  stim_gen->transmission_state = PARKING;
  stim_gen->shaping_param = (seed & 0xFF) / 0xFF; 
}
void stimulus_update(stimulus_gen_t *stim_gen){
  int i,j;
	int k;
	char rows[5][20] = {"speed1","speed2","speed3","speed4","speed5"};
	char columns[9][20] = {"speed1","speed2","speed3","speed4","speed5","min speed","max speed","min rpm","max rpm"};
	*stim_gen->rpm = 0;
  switch(stim_gen->transmission_state) {
    case PARKING: {
      if (stim_gen->time_step < 10) stim_gen->time_step++;
      else {
        stim_gen->time_step = 0;
        stim_gen->transmission_state = REVERSE;
      }
      break;
    }
    case REVERSE: {
      if (stim_gen->time_step < 10) {
        stim_gen->time_step++;
        *stim_gen->speed = (stim_gen->time_step) * (-1.0f);
      }
      else if (stim_gen->time_step < 20) {
        stim_gen->time_step++;
        *stim_gen->speed = *stim_gen->speed + 1; 
      }
      else {
        stim_gen->time_step = 0;
        stim_gen->transmission_state = NEUTRAL;
      }
      *stim_gen->rpm = speed_2_rpm(1, *stim_gen->speed * -1);  // reverse has differential gear ration = 1
      break;
    }
    case NEUTRAL: {
      if (stim_gen->time_step < 3) stim_gen->time_step++;
      else {
        stim_gen->time_step = 0;
        stim_gen->transmission_state = DRIVING;
        transmission_ctrl_init(stim_gen->ctrl); // set to first gear
      }
      break;
    }
    case DRIVING: { 
      float tt;
      if (stim_gen->time_step < 62) {
        stim_gen->time_step++;
        tt = stim_gen->time_step * 0.05f;
        *stim_gen->speed = 100 * (sin(tt) + stim_gen->shaping_param * sin(3*tt));
        transmission_ctrl_update_state(stim_gen->ctrl);
      }
      else {
        stim_gen->time_step = 0;
        stim_gen->transmission_state = PARKING;
        stim_gen->shaping_param += 0.3f;
        if (stim_gen->shaping_param > 1) stim_gen->shaping_param = stim_gen->shaping_param - 1;
				
				// printing table to console 
				for(i=0; i<=4; i++)
				{
					for(j=0; j<=8; j++)
					{
						if (i==0 && j==0)
						{
							printf("from/to  ");
							for(k=0; k<=8; k++)
							{
								printf("%s\t      ", columns[k]);
							}
							printf("\n");
						}
						if(j==0)
						{
							printf("%s\t", rows[i]);
						}
						printf("%f\t ",(*(stim_gen->ctrl)).data[i][j]);
					}
					printf("\n");			
					}
				}
      }
      break;
    }
  }

void display_status(stimulus_gen_t *stim){
  char state_names[4][20] = { "PARKING", "REVERSE", "NEUTRAL", "DRIVING" };
  char sub_state_names[5][20] = { "FIRST_GEAR", "SECOND_GEAR", "THIRD_GEAR", "FOURTH_GEAR", "FIFTH_GEAR" };
  printf ("speed %3.2f RPM %4d ", *stim->speed, *stim->rpm);
  switch (stim->transmission_state) {
    case PARKING:
    case REVERSE:
    case NEUTRAL:
      printf ("state %s", state_names[stim->transmission_state]);
      break;
    case DRIVING:
      printf ("state %s", sub_state_names[stim->ctrl->gear]);
		break;
	}
  printf ("\n");
}

