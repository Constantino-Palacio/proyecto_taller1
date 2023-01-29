/*
   CONTROL DE LUCES
   
   CONSTANTINO PALACIO, OCT-22.
*/

#include "luces.h"
#include "puertos.h"
#include "sapi.h"

int   relay_status=ON,        // estado del relay
      relay_previous=ON,      // estado previo del relay
      
      toggle_status=OFF,
      led_status=OFF;
      
uint8_t  valor_pwm=0;

int relay_change() {
   return (relay_previous==relay_status);
}

void relay_off() {
   relay_status=ON;        // apagar relay
   //gpioWrite(RELAY_ST_OUT,ON);
}

void relay_on() {
   relay_status=OFF;       // encender relay
   //gpioWrite(RELAY_ST_OUT,OFF);
}

void relay_toggle() {
   relay_previous=relay_status;           // actualizar estado
   gpioWrite(RELAY_OUT,!relay_status);    // conmutar relay
   //printf("*********************************************************** RELAY : %d *\n",relay_status);    // debug
}

int toggle_read() {
   if (!gpioRead(TOGGLE_IN)) toggle_status=!toggle_status;  // leer la entrada de control
      
   //DEBUG
   /*if(toggle_status)
      gpioWrite(RELAY_ST_OUT,ON);
   else
      gpioWrite(RELAY_ST_OUT, OFF);*/
   
   //printf("* TOGGLE %d *\n",toggle_status);  // debug
   return toggle_status;                                    // y devolver el resultado
}

void led_bright(int b) {                     // dar brillo al LED   
   gpioWrite(LED_ST_OUT, led_status);     // debug
   if (led_status) {
      /*
         Tomar el valor recibido y convertirlo a un valor en rango 0..255 para
         generar el duty cycle del PWM.
      
         El ADC retorna un valor en rango 0..1023: 0..255 entra cuatro veces en
         este rango, entonces:
              0.. 255 =  0% -  25% =   0.. 64
            256.. 511 = 26% -  50% =  65..127
            512.. 767 = 51% -  75% = 128..191
            768..1023 = 76% - 100% = 192..255
      */
      if(!led_status)
         pwmWrite(LED_OUT,0);
      if ((b >= 0) && (b <= 255))           // 0..25 %
         valor_pwm = b/4;
      else if ((b >= 256) && (b <= 511))    // 26..50 %
         valor_pwm = 65 + b/8;
      else if ((b >= 512) && (b <= 767))    // 51..75 %
         valor_pwm = 128 + b/12;
      else                                   // 76..100 %
         valor_pwm = 192 + b/16;
      
      pwmWrite(LED_OUT,valor_pwm);
      
      //dacWrite(LED_OUT, b);
      //printf("* POT    %d *\n",b);     // debug
   } //else printf("* POT    OFF *\n");  // debug
}
void led_on()
{
   led_status=ON;
}
void led_off()
{
   led_status=OFF;
   pwmWrite(LED_OUT,0);
}
void led_toggle() {
   if (!gpioRead(LED_TOGGLE)) led_status=!led_status;       // leer entrada de control y actualizar el estado del led
      
   //DEBUG
   if(led_status)
      gpioWrite(LED_ST_OUT,ON);
   else
      gpioWrite(LED_ST_OUT, OFF);
}

int pot_read() {                             // leer potenciometro y devolver el valor
   return adcRead(POT_IN);
}