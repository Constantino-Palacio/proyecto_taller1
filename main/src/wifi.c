#include "wifi.h"
#include "sapi.h"     // <= sAPI header
#include "cJSON.h"
#include <stdlib.h>
#include <stdio.h>
#include "puertos.h"
char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}
static cJSON *json;
static char uartBuff[10];
uint16_t i;

int luz_1(){
   cJSON *luz_1 = NULL;
   luz_1 = cJSON_GetObjectItemCaseSensitive(json, "luz_1");
   if(cJSON_IsTrue(luz_1)) return 1; else return 0;

}
int led(){
   cJSON *led = NULL;
   led = cJSON_GetObjectItemCaseSensitive(json, "led");
   if(cJSON_IsTrue(led)) return 1; else return 0;

}
int sensor_luminosidad(){
   cJSON *sensor_luminosidad = NULL;
   sensor_luminosidad = cJSON_GetObjectItemCaseSensitive(json, "sensor_luminosidad");
   if(cJSON_IsTrue(sensor_luminosidad)) return 1; else return 0;

}
int sensor_movimiento(){
   cJSON *sensor_movimiento = NULL;
   sensor_movimiento = cJSON_GetObjectItemCaseSensitive(json, "sensor_movimiento");
   if(cJSON_IsTrue(sensor_movimiento)) return 1; else return 0;

}
int intensidad(){
   cJSON *intensidad = NULL;
   intensidad = cJSON_GetObjectItemCaseSensitive(json, "sensor_intensidad");
   int a = cJSON_GetNumberValue(intensidad);
   return a;

}
#define JSON_MAX_SIZE 150
uint8_t dato;
int leerJson(){
   
   char* palabra =  (char *) malloc(150 * sizeof(char));  

   i = 0;
   //Leo json de la uart
   while( (i < JSON_MAX_SIZE) && (dato != '\n')){
      if(uartReadByte( UART_232, &dato)){
         palabra[i]=dato;
         //uartWriteByte( UART_USB, dato );
         i++;
      }
   }
   
   //Verificación de error
   if((i < JSON_MAX_SIZE)){
      palabra[i-1]='\0';
      cJSON_Delete(json);
      json = cJSON_Parse(palabra);
      
      const char *error_ptr = cJSON_GetErrorPtr();
      if ((error_ptr != NULL || json != NULL)){
         free(palabra);  
         return 0;
      }
   }
   free(palabra);  
   //En caso de error retorno false
   return 0;
}

int imprimirJson(){
   char *string = cJSON_Print(json);
   uartWriteString( UART_USB,  string);
   free(string);
   return 1;
}

int wifiSetup(){
   // ------------- INICIALIZACIONES -------------

   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_USB, 9600 );
   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_232, 9600 );
   
   char miTexto[] = "Setup wifi config...\r\n";
   uartWriteString( UART_USB, miTexto ); 
   uartWriteString( UART_USB, "\r\n" );
}



