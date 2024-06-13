/*******************************************************************************
* Title                 :   TODO: OLED   
* Filename              :   TODO: main.c
* Author                :   Kevin Rivera
* Origin Date           :   13/06/2024
* Version               :   17.9.1
* Compiler              :   TODO: VISUAL STUDIO CODE
* Target                :   TODO: I2C, OLED 128x64
* Notes                 :   None
*
*
*******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "Driver_oled.h"


void app_main(void)
{
    i2c_init(I2C_NUM_0, GPIO_NUM_21, GPIO_NUM_22, 0x3C); // Se conecta el display con i2c
    i2c_oled_init();  // Se configura el display con comandos
    i2c_oled_reset(); // Borra la información del display
    
    
    i2c_oled_string_N("Kevin Rivera", 0, 20); 
    i2c_oled_pila(7,100);
    i2c_oled_wifi(7,0);
    while(1){
    //i2c_oled_scroll_string("DRIVER OLED", 5);
    i2c_oled_banner_N("DRIVER OLED");
    }

}

