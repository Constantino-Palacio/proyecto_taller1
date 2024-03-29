
#include "sapi.h"
#include "mode.h"
#include "puertos.h"
int   mode_status = OFF;

void mode_off() {
   mode_status=OFF;                     // MODO WIFI OFF
   gpioWrite(MODE_ST_OUT, OFF);         // y actualizar el LED de estado
}

void mode_on() {
   mode_status=ON;                      // MODO WIFI ON
   gpioWrite(MODE_ST_OUT, ON);          // y actualizar el LED de estado
}

int mode_toggle() {
   if (!gpioRead(MODE_TOGGLE)){
      delay(10);
      if (!gpioRead(MODE_TOGGLE)){
      mode_status=!mode_status;}
      }
   if(mode_status) mode_on();
   else mode_off();
   //printf("* Mode    %d *\n",mode_status);
   return mode_status;
}