
/*******************************************************************************
* Title                 :   TODO: OLED   
* Filename              :   TODO: Driver_oled.c
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
#include "Driver_oled.h"
#include "include/caracteres.h"


/*************************************************************************
* Function: Inicialización de la estructura i2c_oled_t
* Preconditions: La estructura i2c_oled_t debe estar definida previamente en el programa. 
*                Además, se debe incluir las bibliotecas necesarias para el manejo de I2C.
* Overview: Este código inicializa una estructura i2c_oled_t con los valores necesarios
*           para configurar un dispositivo OLED a través de la interfaz I2C en un microcontrolador.
* Input: No hay entrada directa a esta inicialización, los valores son asignados explícitamente.
* Output: 
*	  - i2c_oled_t oled: Una instancia inicializada de la estructura i2c_oled_t con los siguientes parámetros:
*     - I2C_NUM_0: Puerto I2C utilizado.
*     - GPIO_NUM_21: Pin GPIO asignado para la línea SDA.
*     - GPIO_NUM_22: Pin GPIO asignado para la línea SCL.
*     - 0x3C: Dirección I2C del dispositivo OLED.
*
*****************************************************************************/

i2c_oled_t oled = {
		I2C_NUM_0,   // Puerto i2c
		GPIO_NUM_21, // Pin SDA
		GPIO_NUM_22, // Pin SCL
		0x3C         // Dirección i2c
};


/**************************************************************************
* Function: i2c_init
* Preconditions: La estructura i2c_oled_t debe estar definida previamente en el programa.
*                
* Overview: Esta función inicializa el puerto I2C para comunicarse con un dispositivo OLED.
*           Configura los pines SDA y SCL, la dirección del dispositivo y la velocidad del reloj I2C.
* Input: 
*   - uint8_t puerto: Número del puerto I2C a utilizar.
*   - uint8_t pinSDA: Número del pin GPIO utilizado para la línea SDA.
*   - uint8_t pinSCL: Número del pin GPIO utilizado para la línea SCL.
*   - uint8_t dir: Dirección I2C del dispositivo OLED.
* Output: 
*   - esp_err_t: Código de error de ESP-IDF que indica el éxito o fallo de la inicialización del puerto I2C.
*
*****************************************************************************/

esp_err_t i2c_init(uint8_t puerto, uint8_t pinSDA, uint8_t pinSCL, uint8_t dir){
	// Ajustes del display
	oled.scl = pinSCL;
	oled.sda = pinSDA;
	oled.address = dir;
	oled.i2c_port = puerto;
	// Estructura para para configurar la conexión i2c
	i2c_config_t conf = {
	    .mode = I2C_MODE_MASTER,
	    .sda_io_num = oled.sda,
	    .sda_pullup_en = GPIO_PULLUP_ENABLE,
	    .scl_io_num = oled.scl,
	    .scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = 1000000,		//400000
	};
    i2c_param_config(oled.i2c_port, &conf); // Aplica la configuración al puerto i2c
    return i2c_driver_install(oled.i2c_port, conf.mode, 0, 0, 0); // Se conecta al puerto i2c
}



/**************************************************************************
* Function: i2c_oled_cmd_1byte
* Preconditions: La estructura i2c_oled_t debe estar definida previamente y la conexión I2C inicializada.
* Overview: Envía un solo byte de comando al dispositivo OLED a través de I2C.
* Input: 
*   - uint8_t dato: El byte de comando a enviar.
* Output: Ninguno.
*****************************************************************************/
void i2c_oled_cmd_1byte(uint8_t dato){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Crea un nuevo objeto cmd para construir secuencias
    i2c_master_start(cmd); // Agrega un comando de inicio a la secuencia

	// Se conecta con el display
    // Escribe la dirección del dispositivo (oled.address) con el bit de escritura (I2C_MASTER_WRITE)
    // El parámetro 'true' indica que se espera una confirmación (ack) después de enviar el byte
    i2c_master_write_byte(cmd, (oled.address << 1) | I2C_MASTER_WRITE, true); 

    i2c_master_write_byte(cmd, 0x00, true); // Envía comando de control
    i2c_master_write_byte(cmd, dato, true); // Envía el byte

    i2c_master_stop(cmd); // Agrega comando de paro a la secuencia
    i2c_master_cmd_begin(oled.i2c_port, cmd, 500/portTICK_PERIOD_MS); // Envía la secuencia de comandos construida
    i2c_cmd_link_delete(cmd); // Elimina el objeto cmd
}



/**************************************************************************
* Function: i2c_oled_cmd_2byte
* Preconditions: La estructura i2c_oled_t debe estar definida previamente y la conexión I2C inicializada.
* Overview: Envía dos bytes de comando al dispositivo OLED a través de I2C.
* Input: 
*   - uint8_t dato[]: Un array de dos bytes de comando a enviar.
* Output: Ninguno.
*****************************************************************************/
void i2c_oled_cmd_2byte(uint8_t dato[]){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Crea un nuevo objeto cmd para construir secuencias
    i2c_master_start(cmd); // Agrega un comando de inicio a la secuencia

	// Se conecta con el display
    // Escribe la dirección del dispositivo (oled.address) con el bit de escritura (I2C_MASTER_WRITE)
    // El parámetro 'true' indica que se espera una confirmación (ack) después de enviar el byte
    i2c_master_write_byte(cmd, (oled.address << 1) | I2C_MASTER_WRITE, true); // 

    i2c_master_write_byte(cmd, 0x00, true); // Envía comando de control
    i2c_master_write_byte(cmd, dato[0], true); // Envía el byte 1
    i2c_master_write_byte(cmd, dato[1], true); // Envía el byte 2

    i2c_master_stop(cmd); // Agrega comando de paro a la secuencia
    i2c_master_cmd_begin(oled.i2c_port, cmd, 500/portTICK_PERIOD_MS); // Envía la secuencia de comandos construida
    i2c_cmd_link_delete(cmd); // Elimina el objeto cmd
}




/**************************************************************************
* Function: i2c_oled_init
* Preconditions: La estructura i2c_oled_t debe estar definida previamente y la conexión I2C inicializada.
* Overview: Inicializa el dispositivo OLED enviando una serie de comandos de configuración.
* Input: Ninguno.
* Output: Ninguno.
*****************************************************************************/
void i2c_oled_init() {
    // Envía los comandos básicos de configuración

    uint8_t cmd[] = { 0xA8, 0x3F };
    i2c_oled_cmd_2byte(cmd);
    cmd[0] = 0xD3; cmd[1] = 0x00;
    i2c_oled_cmd_2byte(cmd);
    i2c_oled_cmd_1byte(0x40);
    i2c_oled_cmd_1byte(0xA1);
    i2c_oled_cmd_1byte(0xC8);
    cmd[0] = 0xDA; cmd[1] = 0x12;
    i2c_oled_cmd_2byte(cmd);
    cmd[0] = 0x81; cmd[1] = 0x7F;
    i2c_oled_cmd_2byte(cmd);
    i2c_oled_cmd_1byte(0xA4);
    i2c_oled_cmd_1byte(0xA6);
    cmd[0] = 0xD5; cmd[1] = 0x80;
    i2c_oled_cmd_2byte(cmd);
    cmd[0] = 0x8D; cmd[1] = 0x14;
    i2c_oled_cmd_2byte(cmd);
    i2c_oled_cmd_1byte(0xAF);
}



/**************************************************************************
* Function: i2c_oled_dato
* Preconditions: La estructura i2c_oled_t debe estar definida previamente y la conexión I2C inicializada.
* Overview: Envía un byte de datos al dispositivo OLED a través de I2C.
* Input: 
*   - uint8_t data: El byte de datos a enviar.
* Output: Ninguno.
*****************************************************************************/
void i2c_oled_dato(uint8_t data){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Crea un nuevo objeto cmd 
    i2c_master_start(cmd); // Agrega un comando de inicio a la secuencia

    i2c_master_write_byte(cmd, (oled.address << 1) | I2C_MASTER_WRITE, true); // Se conecta con el display
    i2c_master_write_byte(cmd, 0x40, true); // Envía comando de datos
    i2c_master_write_byte(cmd, data, true); // Envía el byte

    i2c_master_stop(cmd); // Agrega comando de paro a la secuencia
    i2c_master_cmd_begin(oled.i2c_port, cmd, 500/portTICK_PERIOD_MS); // Envía la secuencia de comandos construida
    i2c_cmd_link_delete(cmd); // Elimina el objeto cmd
}



/**************************************************************************
* Function: i2c_oled_pos
* Preconditions: La estructura i2c_oled_t debe estar definida previamente y la conexión I2C inicializada.
* Overview: Establece la posición del cursor en el dispositivo OLED.
* Input: 
*   - uint8_t y: Coordenada y (página).
*   - uint8_t x: Coordenada x (columna).
* Output: Ninguno.
*****************************************************************************/
void i2c_oled_pos(uint8_t y, uint8_t x){
    // Para aceptar la posición correcta en y
    if (y > 7) {
        y = 7;
    }
    // Para aceptar la posición correcta en x
    if (x > Ancho - 1) {
        x = Ancho - 1;
    }

    // Manda la posición del cursor al display
    i2c_oled_cmd_1byte(0x00 + (0x0F & x)); // 0x00 comando de control. Envía posición en x. (0x0F & x) Es para garantizar que el valor estará entre 0 y 15
    i2c_oled_cmd_1byte(0x10 + (0x0F & (x >> 4))); // 0x10 comando de control. Manda los bits menos significativos
    i2c_oled_cmd_1byte(0xB0 + y); // Coloca el cursor en la página y
}



/**************************************************************************
* Function: i2c_oled_reset
* Preconditions: La estructura i2c_oled_t debe estar definida previamente y la conexión I2C inicializada.
* Overview: Limpia todo el contenido del dispositivo OLED.
* Input: Ninguno.
* Output: Ninguno.
*****************************************************************************/
void i2c_oled_reset(){
    int i, j;
    // Pasa por todo el display borrando la información que tenga
    for (i = 0; i < 8; i++) {
        for (j = 0; j < Ancho; j++) {
            i2c_oled_pos(i, j);
            i2c_oled_dato(0x00); // Borra los datos
        }
    }
    i2c_oled_pos(0, 0); // Posición inicial
}



/**************************************************************************
* Function: i2c_oled_char
* Preconditions: La estructura i2c_oled_t debe estar definida previamente y la conexión I2C inicializada.
* Overview: Muestra un carácter en el dispositivo OLED.
* Input: 
*   - uint8_t caracter: El carácter a mostrar.
* Output: Ninguno.
*****************************************************************************/
void i2c_oled_char(uint8_t caracter){
    uint8_t caracterRotado[8]; // Variable para colocar correctamente el caracter

    /* Rotar caracter 90 grados.*/
    for (int i = 7; i >= 0; i--) {
        caracterRotado[i] = 0;
        for (int j = 0; j < 8; j++) {
            if (caracteres[caracter - 32][j] & (1 << (7 - i))) { /* Toma el caracter de la estructura "caracteres" */
                caracterRotado[i] |= (1 << j);
            }
        }
    }
    /* Invierte filas. */
    for (int i = 0; i < 4; i++) {
        uint8_t temp = caracterRotado[i];
	}
	
	for(int j=0; j<8; j++){
	i2c_oled_dato(caracterRotado[j]);
	}
}



/***************************************************************************
* Function: i2c_oled_string
* Preconditions: i2c_oled_pos, i2c_oled_char
* Overview: Imprime una cadena de caracteres en la pantalla OLED en una posición específica.
* Input: char* string (cadena a imprimir), uint8_t y (posición vertical), uint8_t x (posición horizontal)
* Output: Ninguno
*****************************************************************************/
void i2c_oled_string(char* string, uint8_t y, uint8_t x) {
    i2c_oled_pos(y, x);
    int i = 0;
    uint8_t current_x = x;
    uint8_t current_y = y;

    // Manda los caracteres del string
    while(string[i]) {
        if (current_x > 127) {
            current_x = 0;
            current_y++;
            i2c_oled_pos(current_y, current_x);
        }
        i2c_oled_char(string[i]); // Manda el caracter
        current_x += 8; // Incrementa la variable para el siguiente carácter (asumiendo que cada carácter tiene 8 columnas)
        i++; // Incrementa la variable del índice del string
    }
}



/***************************************************************************
* Function: i2c_oled_string_N
* Preconditions: i2c_oled_pos, i2c_oled_char_n
* Overview: Imprime una cadena de caracteres en la pantalla OLED en una posición específica usando una fuente alternativa.
* Input: char* string (cadena a imprimir), uint8_t y (posición vertical), uint8_t x (posición horizontal)
* Output: Ninguno
*****************************************************************************/
void i2c_oled_string_N(char* string, uint8_t y, uint8_t x) {
    i2c_oled_pos(y, x);
    int i = 0;
    uint8_t current_x = x;
    uint8_t current_y = y;

    // Manda los caracteres del string
    while(string[i]) {
        if (current_x > 127) {
            current_x = 0;
            current_y++;
            i2c_oled_pos(current_y, current_x);
        }
        i2c_oled_char_n(string[i]); // Manda el caracter
        current_x += 8; // Incrementa la variable para el siguiente carácter (asumiendo que cada carácter tiene 8 columnas)
        i++; // Incrementa la variable del índice del string
    }
}



/***************************************************************************
* Function: i2c_oled_banner_N
* Preconditions: i2c_oled_pos, i2c_oled_dato, i2c_oled_char_n
* Overview: Imprime un banner desplazable en la pantalla OLED con una cadena de texto.
* Input: char* string (cadena de texto)
* Output: Ninguno
*****************************************************************************/
void i2c_oled_banner_N(char* string) {
    int i, j, x, text_length, string_width;

    // Calcular el ancho total del texto en píxeles (asumiendo 8 píxeles por carácter)
    text_length = 0;
    while(string[text_length] != '\0') {
        text_length++;
    }
    string_width = text_length * 8;

    // Prende los píxeles de arriba
    for(i = 4; i < 7; i++){
        for(j = 0; j < Ancho; j++){
            i2c_oled_pos(i, j);
            i2c_oled_dato(0xFF); // Enciende los píxeles
        }
    }

    // Desplazamiento del texto de derecha a izquierda
    for(x = Ancho - 1; x > -string_width; x--) {
        // Enciende los píxeles de la línea 5 antes de imprimir los caracteres en la nueva posición
        for(j = 0; j < Ancho-1; j++){
            i2c_oled_pos(5, j);
            i2c_oled_dato(0xFF); // Enciende los píxeles
        }

         //Borra la línea antes de imprimir los caracteres en la nueva posición
        for(j = 0; j < Ancho-1; j++){
            i2c_oled_pos(5, j);
            i2c_oled_dato(0x00); // Borra los datos
        }

        // Imprime los caracteres del string en la nueva posición
        for(i = 0; i < text_length; i++) {
            if((x + i * 8) >= 0 && (x + i * 8) < Ancho-1) {
                i2c_oled_pos(5, x + i * 8);
                i2c_oled_char_n(string[i]); // Manda el carácter
            }
        }

        usleep(10); // Pausa de 100 ms para el efecto de desplazamiento
    }

    // Imprimir el texto en la posición final fija y enciende los píxeles
    for(j = 0; j < Ancho-1; j++){
        i2c_oled_pos(5, j);
        i2c_oled_dato(0xFF); // Enciende los píxeles
    }

    x = 0; // Ajustar según dónde quieras que termine el texto
    for(i = 0; i < text_length; i++) {
        if((x + i * 8) >= 0 && (x + i * 8) < Ancho-1) {
            i2c_oled_pos(5, x + i * 8);
            i2c_oled_char_n(string[i]); // Manda el carácter
        }
    }
}



/***************************************************************************
* Function: i2c_oled_char_n
* Preconditions: i2c_oled_dato
* Overview: Imprime un carácter rotado e invertido en la pantalla OLED.
* Input: uint8_t caracter (carácter a imprimir)
* Output: Ninguno
*****************************************************************************/
void i2c_oled_char_n(uint8_t caracter){
	uint8_t caracterRotado[8]; // Variable para colorar correctamente el caracter

	 /* Rotar caracter 90 grados.*/
	for (int i = 7; i >= 0; i--){
		caracterRotado[i] = 0;
	    for (int j = 0; j < 8; j++){
	      if (caracteres[caracter-32][j] & (1 << (7 - i))){ /* Toma el caracter de la estructura "caracteres" */
	    	  caracterRotado[i] |= (1 << j);
	      }
	    }
	}
	/* Invierte filas. */
	for (int i = 0; i < 4; i++){
	    uint8_t temp = caracterRotado[i];
	    caracterRotado[i] = caracterRotado[7 - i];
	    caracterRotado[7 - i] = temp;
	}

	 /* Imprime caracter. */
	for(int j=0; j<8; j++){
		caracterRotado[j] = ~caracterRotado[j];
		i2c_oled_dato(caracterRotado[j]);
	}
}



/***************************************************************************
* Function: i2c_oled_scroll_string
* Preconditions: i2c_oled_pos, i2c_oled_dato, i2c_oled_char
* Overview: Desplaza una cadena de texto de derecha a izquierda en la pantalla OLED.
* Input: char* string (cadena de texto), uint8_t y (posición vertical)
* Output: Ninguno
*****************************************************************************/
void i2c_oled_scroll_string(char* string, uint8_t y) {
    int i, j, x, text_length, string_width;

    // Calcular el ancho total del texto en píxeles (asumiendo 8 píxeles por carácter)
    text_length = 0;
    while(string[text_length] != '\0') {
        text_length++;
    }
    string_width = text_length * 8;

    // Desplazamiento del texto de derecha a izquierda
    for(x = Ancho - 8; x > -string_width; x--) {
        // Borra la línea antes de imprimir los caracteres en la nueva posición
        for(j = 0; j < Ancho-8; j++){
            i2c_oled_pos(y, j);
            i2c_oled_dato(0x00); // Borra los datos
        }

        // Imprime los caracteres del string en la nueva posición
        for(i = 0; i < text_length; i++) {
            if((x + i * 8) >= 0 && (x + i * 8) < Ancho-8) {
                i2c_oled_pos(y, x + i * 8);
                i2c_oled_char(string[i]); // Manda el carácter
            }
        }

        usleep(10); // Pausa de 100 ms para el efecto de desplazamiento
    }
}



/***************************************************************************
* Function: i2c_oled_pila
* Preconditions: i2c_oled_pos, i2c_oled_dato
* Overview: Imprime un símbolo de pila en la pantalla OLED en una posición específica.
* Input: uint8_t y (posición vertical), uint8_t x (posición horizontal)
* Output: Ninguno
*****************************************************************************/
void i2c_oled_pila(uint8_t y, uint8_t x){
	
	for(int j=0; j<25; j++){				//Ciclo  de 25 columnas en la misma pagina, pa hacer el simbolo de pila 
		i2c_oled_pos(y, x);
		i2c_oled_dato(pila[j]);
		x++;
	}
	
}



/***************************************************************************
* Function: i2c_oled_wifi
* Preconditions: i2c_oled_pos, i2c_oled_dato
* Overview: Imprime un símbolo de wifi en la pantalla OLED en una posición específica.
* Input: uint8_t y (posición vertical), uint8_t x (posición horizontal)
* Output: Ninguno
*****************************************************************************/
void i2c_oled_wifi(uint8_t y, uint8_t x){
	uint8_t t;
	t=x;
	for(int j=0; j<25; j++){                 //Ciclo que encienden lo pixeles, una pagina antes, para el resto de la imagen. 
		i2c_oled_pos(y-1, x);
		i2c_oled_dato(wifi_1[j]);  
		x++;
	}
	for(int j=0; j<25; j++){				//Ciclo que enciden los pixeles para el simbolo de wifi
		i2c_oled_pos(y, t);
		i2c_oled_dato(wifi_2[j]);			
		t++;
	}
	
}