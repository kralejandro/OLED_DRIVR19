
/*******************************************************************************
* Title                 :   TODO: OLED   
* Filename              :   TODO: Driver_oled.h
* Author                :   Kevin Rivera
* Origin Date           :   13/06/2024
* Version               :   17.9.1
* Compiler              :   TODO: VISUAL STUDIO CODE
* Target                :   TODO: I2C, OLED 128x64
* Notes                 :   None
*
*
*******************************************************************************/#include <driver/gpio.h>
#include <driver/i2c.h>
#include "freertos/portmacro.h"
#include "hal/i2c_types.h"
#include <unistd.h>
#include <esp_log.h>
#include <string.h>
#include <math.h>

// Tamaño del display
#define Alto	64
#define Ancho	128
// Estructura para manejar el display con su puerto, pines y direción
typedef struct {
	i2c_port_t i2c_port;
	int sda;
	int scl;
	int address;
} i2c_oled_t;

// Función para conectar el display por medio de i2c
esp_err_t i2c_init(uint8_t puerto, uint8_t pinSDA, uint8_t pinSCL, uint8_t dir);

// Función para mandar un byte al display
void i2c_oled_cmd_1byte(uint8_t data);

// Función para mandar dos bytes al display
void i2c_oled_cmd_2byte(uint8_t data[]);

// Función para inicializar el display mandando los codigos necesarios
void i2c_oled_init();

// Función para mandar un dato en forma de bytes
void i2c_oled_dato(uint8_t data);

// Función para colocar el cursor en la posición (x, y)
void i2c_oled_pos(uint8_t y, uint8_t x);

// Función para borrar la pantalla
void i2c_oled_reset();

// Función para imprimir un caracter
void i2c_oled_char(uint8_t caracter);

// Función para imprimir un caracter negado
void i2c_oled_char_n(uint8_t caracter);

// Funcionpara mandar una cadena de caracteres en la posición (x,y)
void i2c_oled_string(char* string, uint8_t y, uint8_t x);

// Función para Crear un banner
void i2c_oled_banner_N(char* string);

// Función para crear un texto scrolleando
void i2c_oled_scroll_string(char* string, uint8_t y);

// Función para imprimir simbolo de pila
void i2c_oled_pila(uint8_t y, uint8_t x);

void i2c_oled_wifi(uint8_t y, uint8_t x);

// Funcionpara mandar una cadena de caracteres en la posición (x,y), con los pixeles invertidos
void i2c_oled_string_N(char* string, uint8_t y, uint8_t x);
