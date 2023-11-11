#include "transmission_ctrl.h"

void transmission_ctrl_init(transmission_ctrl_t *ctrl){
	int i,j;
  ctrl->gear = FIRST_GEAR;
  ctrl->GRT = 3.42;   // gear ratio transmission 
  ctrl->GRD[0] = 2.97;  // gear ratio differential
  ctrl->GRD[1] = 2.07;  // gear ratio differential
  ctrl->GRD[2] = 1.43;  // gear ratio differential
  ctrl->GRD[3] = 1;  // gear ratio differential
  ctrl->GRD[4] = 0.84;  // gear ratio differential
  ctrl->CT = 1.613679;  // wheal circumference in meters
	// setting min speed and rpm values to arbitrarily high value, and gear change counts to zero as an initialization
  ctrl->data[0][5] = 10000000;
	ctrl->data[1][5] = 10000000;
	ctrl->data[2][5] = 10000000;
	ctrl->data[3][5] = 10000000;
	ctrl->data[4][5] = 10000000;
	ctrl->data[0][7] = 10000000;
	ctrl->data[1][7] = 10000000;
	ctrl->data[2][7] = 10000000;
	ctrl->data[3][7] = 10000000;
	ctrl->data[4][7] = 10000000;
	for(i=0; i<=4; i++)
				{
					for(j=0; j<=4; j++)
					{
						 ctrl->data[i][j] = 0;
					}
}
}	
				
void transmission_ctrl_update_state(transmission_ctrl_t *ctrl){
	// state machine using case based syntax
	switch(ctrl->gear){
		case FIRST_GEAR:
				*ctrl->rpm = speed_2_rpm(ctrl->GRD[0], *ctrl->speed);
			if(*ctrl->speed < ctrl->data[0][5])
			{
					ctrl->data[0][5] = *ctrl->speed;
			}
			if(*ctrl->speed > ctrl->data[0][6])
			{
					ctrl->data[0][6] = *ctrl->speed;
			}
			if(*ctrl->rpm < ctrl->data[0][7])
			{
					ctrl->data[0][7] = *ctrl->rpm;
			}
			if(*ctrl->rpm > ctrl->data[0][8])
			{
					ctrl->data[0][8] = *ctrl->rpm;
			}
		  if(*ctrl->rpm >= 2500)
			{
			ctrl->data[0][1]++;
			ctrl->gear = SECOND_GEAR;
		 	*ctrl->rpm = 1000;
		  }
			else{
				ctrl->data[0][0]++;
			}
	  break;
		
		case SECOND_GEAR:
			*ctrl->rpm = speed_2_rpm(ctrl->GRD[1], *ctrl->speed);
		  if(*ctrl->speed < ctrl->data[1][5])
			{
					ctrl->data[1][5] = *ctrl->speed;
			}
			if(*ctrl->speed > ctrl->data[1][6])
			{
					ctrl->data[1][6] = *ctrl->speed;
			}
			if(*ctrl->rpm < ctrl->data[1][7])
			{
					ctrl->data[1][7] = *ctrl->rpm;
			}
			if(*ctrl->rpm > ctrl->data[1][8])
			{
					ctrl->data[1][8] = *ctrl->rpm;
			}
			if(*ctrl->rpm >= 2500)
			{
				ctrl->data[1][2]++;
				ctrl->gear = THIRD_GEAR;
	      *ctrl->rpm = 1000;
			} 
			else if( *ctrl->rpm <= 700)
			{
				ctrl->data[1][0]++;
				ctrl->gear = FIRST_GEAR;
			  *ctrl->rpm = 1000;
			}
			else{
				ctrl->data[1][1]++;
			}
	    break;
				
		case THIRD_GEAR :
			*ctrl->rpm = speed_2_rpm(ctrl->GRD[2], *ctrl->speed);
		  if(*ctrl->speed < ctrl->data[2][5])
			{
					ctrl->data[2][5] = *ctrl->speed;
			}
			if(*ctrl->speed > ctrl->data[2][6])
			{
					ctrl->data[2][6] = *ctrl->speed;
			}
			if(*ctrl->rpm < ctrl->data[2][7])
			{
					ctrl->data[2][7] = *ctrl->rpm;
			}
			if(*ctrl->rpm > ctrl->data[2][8])
			{
					ctrl->data[2][8] = *ctrl->rpm;
			}
			if(*ctrl->rpm >= 2500)
			{
				ctrl->data[2][3]++;
				ctrl->gear = FOURTH_GEAR;
	      *ctrl->rpm = 1000;
			} 
			else if( *ctrl->rpm <= 700)
			{
				ctrl->data[2][1]++;
				ctrl->gear = SECOND_GEAR;
			  *ctrl->rpm = 1000;
			}
			else{
				ctrl->data[2][2]++;
			}
	    break;
			
		case FOURTH_GEAR:
			*ctrl->rpm = speed_2_rpm(ctrl->GRD[3], *ctrl->speed);
		  if(*ctrl->speed < ctrl->data[3][5])
			{
					ctrl->data[3][5] = *ctrl->speed;
			}
			if(*ctrl->speed > ctrl->data[3][6])
			{
					ctrl->data[3][6] = *ctrl->speed;
			}
			if(*ctrl->rpm < ctrl->data[3][7])
			{
					ctrl->data[3][7] = *ctrl->rpm;
			}
			if(*ctrl->rpm > ctrl->data[3][8])
			{
					ctrl->data[3][8] = *ctrl->rpm;
			}
		  if(*ctrl->rpm >= 2500)
		  {
					ctrl->data[3][4]++;
					ctrl->gear = FIFTH_GEAR;
			    *ctrl->rpm = 1000;
		  } 
		  else if( *ctrl->rpm <= 700)
			{
			  ctrl->data[3][2]++;
				ctrl->gear = THIRD_GEAR;
			  *ctrl->rpm = 1000;
		  }
			else{
				ctrl->data[3][3]++;
			}
    break;
	
		case FIFTH_GEAR:
      *ctrl->rpm = speed_2_rpm(ctrl->GRD[4], *ctrl->speed);
		  if(*ctrl->speed < ctrl->data[4][5])
			{
					ctrl->data[4][5] = *ctrl->speed;
			}
			if(*ctrl->speed > ctrl->data[4][6])
			{
					ctrl->data[4][6] = *ctrl->speed;
			}
			if(*ctrl->rpm < ctrl->data[4][7])
			{
					ctrl->data[4][7] = *ctrl->rpm;
			}
			if(*ctrl->rpm > ctrl->data[4][8])
			{
					ctrl->data[4][8] = *ctrl->rpm;
			}
			if( *ctrl->rpm <= 2000)
			{
				ctrl->data[4][3]++;
				ctrl->gear = FOURTH_GEAR;
			  *ctrl->rpm = 2500;
			}
			else{
				ctrl->data[4][4]++;
			}
	break;
}
}
	uint32_t speed_2_rpm(float grd, float speed)
	{
  uint32_t _rpm;
  // speed = rpm * CT * 3.6 / 60 / GRT / GRD
  // rpm = speed * GRT * GRD * 60 / CT / 3.6 = speed * GRD * 35.32
  _rpm = (uint32_t) (35.32f * speed * grd);
  return _rpm;
  }
	

