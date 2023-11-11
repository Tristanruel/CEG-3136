#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"

typedef enum driving__gears { FIRST_GEAR, SECOND_GEAR, THIRD_GEAR, FOURTH_GEAR, FIFTH_GEAR } driving__gear_e; 	


typedef struct transmission_ctrl_s {
  float           *speed;  // vehicle speed Km/hr
  float           GRT;   // gear ratio transmission 
  float           GRD[5];  // gear ratio differential
  float           CT;  // wheal circumference in meters
  uint32_t        *rpm;   // revolutions per minute
  driving__gear_e gear;   // current drive gear
	float data[5][9];
} transmission_ctrl_t;


void transmission_ctrl_update_state(transmission_ctrl_t *ctrl);
void transmission_ctrl_init(transmission_ctrl_t *ctrl);
uint32_t speed_2_rpm(float grd, float speed);


