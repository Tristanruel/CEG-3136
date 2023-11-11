#include "stm32f4xx.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "sensor.h"

void sensor_display(sensor_t* sensor){
  printf ("Sensor %d at %s is ", sensor->id, sensor->location);
  switch( sensor->state ) {
    case INACTIVE:    printf ("sensor INACTIVE");    break;
    case IDLE:        printf ("sensor IDLE");        break;
    case TRIGGERED:   printf ("sensor TRIGGERED");   break;
    case MALFUNCTION: printf ("sensor MALFUNCTION"); break;
  }
  printf("\n");
}

void sensor_init (sensor_t* sensor, uint16_t id, char* location){
  memset(sensor, 0, sizeof(sensor_t));
  sensor->id = id;
  memcpy(sensor->location, location, 32);
  sensor->state = IDLE;
  sensor_display(sensor);
}

void sensorm_set (sensor_t* sensor){
  if (sensor->state != TRIGGERED) {
    sensor->state = TRIGGERED;
    sensor_display(sensor);
  }
}
void sensorm_reset (sensor_t* sensor){
  if (sensor->state != IDLE) {
    sensor->state = IDLE;
    sensor_display(sensor);
  }
}
void sensorm_set_malfunction (sensor_t* sensor){
  if (sensor->state != MALFUNCTION) {
    sensor->state = MALFUNCTION;
    sensor_display(sensor);
  }
}
