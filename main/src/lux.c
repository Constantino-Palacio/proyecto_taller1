#include "lux.h"
#include "puertos.h"
#include "sapi.h"

int lux_status=OFF;

void lux_on(){
   lux_status=ON;    // habilitar LDR
}

void lux_off() {
   lux_status=OFF;   // deshabilitar LDR
}

int lux_toggle() {
   if (!gpioRead(LUX_ENABLE)) {        // ver si se presion� la entrada de control
         delay(50);
         return gpioRead(LUX_ENABLE);
   }
}

int lux_enabled() {
   return lux_status;
}

int lux_read() {
   if (lux_status) {
      return adcRead(LUX_IN);
   } else return 5000;           // si el sensor est� desactivado, devolver "5000" (dato inv�lido)
}