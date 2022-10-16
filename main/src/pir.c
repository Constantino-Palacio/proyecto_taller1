/* * * *   CODIGO DEL SENSOR PIR   * * * */

#include "sapi.h"
#include "pir.h"
#include "puertos.h"

int   pir_status = OFF,       // inicialmente, el PIR est� desactivado
      pir_value;              // valor le�do desde el PIR

void pir_off() {
   pir_status=OFF;                     // apagar PIR
   gpioWrite(PIR_ST_OUT, OFF);         // y actualizar el LED de estado
}

void pir_on() {
   pir_status=ON;                      // encender PIR
   gpioWrite(PIR_ST_OUT, ON);          // y actualizar el LED de estado
}

void pir_toggle() {
   if (!gpioRead(PIR_ENABLE)) pir_status=!pir_status;
   printf("* PIR    %d *\n",pir_status);
}

int pir_enabled() {
   return pir_status;
}

/*
   Funcion PIR_READ
      RECIBE   NADA
      RETORNA  0: NO HAY MOVIMIENTO (O DESHABILITADO), 1: HAY MOVIMIENTO

   Lee el puerto del sensor PIR y devuelve si hay o no movimiento siempre y cuando est� habilitado el sensor.
*/

int pir_read() {
   int estado=!gpioRead(PIR_IN);
   delay(50);
   estado=gpioRead(PIR_IN);
   if (pir_status) return estado; else return 0;
}