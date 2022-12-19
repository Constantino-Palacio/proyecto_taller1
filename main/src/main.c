/*
   Tareas por hacer:
   
   *1. corregir el bounce en las teclas
   *2. implementar sensor de luminosidad con adc (ver como conectar el ldr al adc -> esquematico)
   *3. implementar dimmer con pwm
   *4. leds de estado
   5. ver que si el sensor capta algo y apago la deteccion, la luz queda prendida (???)
   *6. retardos no bloqueantes
*/

/* * * *   LIBRERIAS   * * * */

#include "luces.h"         // control de las luces, relay y LED
#include "lux.h"           // control de sensor de luminosidad LDR
#include "pir.h"           // control de sensor de movimiento PIR
#include "puertos.h"       // definiciones e inicialización de puertos
#include "sapi.h"          // librería sAPI
#include "mode.h"
#include "cJSON.h"
#include "wifi.h"
/* * * *   DEFINICIONES DE PUERTOS DE ADC/DAC   * * * */

#define  MAX_LUX  200      // maximo del sensor de luminosidad

int main(void){
   
   int mode_status;

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   puertos_init();         // inicializa puertos
   
   relay_off();            // apaga la luz
   led_bright(0);          // apaga la luz LED (intensidad en 0%)
   
   pir_off();              // deshabilita PIR
   lux_off();              // deshabilita LUX
   
   delay_t delay1;         // retardos no bloqueantes
   delay_t delay2;
   
   uint8_t uart_chr;       // dato leido de la UART

   /* Inicializar Retardo no bloqueante con tiempo en ms */
   delayConfig(&delay1,100);  // para chequeo de entradas de control
   delayConfig(&delay2,500);  // para chequeo y actualización del estado del relay
   
   /*wifi setup*/
   wifiSetup();
   cJSON *json = NULL;
   /* bucle principal */

   
   while(1) {      
      /*
         Encender la luz si se da alguna o varias de las siguientes condiciones:
            A. Se prende manualmente a través de la entrada TOGGLE_IN
            B. Se recibe un '1' del sensor PIR
            C. Se recibe un valor < MAX_LUX del sensor LDR
      */

      // leer entradas de control cada 50ms
      if (delayRead(&delay1)) {
        
         /*
            Leer selector de modo y decidir:
               Si es '1' u 'ON', ignorar las entradas de configuración (botonera) y hacer caso a la interfaz web
               Si es '0' u 'OFF', ignorar la interfaz web y hacer caso a la botonera.
         */
         mode_status = mode_toggle();
         
         if (mode_status) {
            if(leerJson()){
               imprimirJson();
               ledSetStatus();
               luxSetStatus();
               PIRSetStatus();
            }
            if ((luz_1()) || (lux_read()<MAX_LUX) || (pir_read())) relay_on(); else relay_off();
         } else {
            // leer entradas de control de los sensores
            pir_toggle();
            lux_toggle();
            
            // leer entrada de control de luz LED
            led_toggle();
            
            // leer potenciómetro y ajustar el brillo del LED
            led_bright(pot_read());
            
            // si los sensores están activados, cualquiera enciende la luz
            if ((toggle_read()) || (lux_read()<MAX_LUX) || (pir_read())) relay_on(); else relay_off();
         }
      }
      
      // actualizar el estado del relay cada 500ms
      if (delayRead(&delay2)) relay_toggle();
   }

   return 0 ;
}
void luxSetStatus(){
   if(sensor_luminosidad()){
      gpioWrite(LUX_ST_OUT,ON);
   }else{
      gpioWrite(LUX_ST_OUT, OFF);
   }
}
void PIRSetStatus(){
   if(sensor_movimiento()){
       pir_on();
   }else{
       pir_off();
   }
}
void ledSetStatus(){
   if(led()){
      led_on();
      int a = intensidad();
      //printf("intensidad %d",a);
      led_bright(a);
      gpioWrite(LED_ST_OUT,ON);
   }else{
      led_off();
      gpioWrite(LED_ST_OUT, OFF);
   }
}