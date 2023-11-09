/*
 * keyboard.h
 *
 *  Created on: Nov 6, 2023
 *      Author: ANDRES CUELLAR
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_
#include <stdio.h>
#include "stm32l4xx_hal.h"


//Declaration of the ports for the rows and columns defined in the .IOC
#define COLUMN_1_GPIO_Port GPIOB
#define COLUMN_2_GPIO_Port GPIOA
#define COLUMN_3_GPIO_Port GPIOA
#define COLUMN_4_GPIO_Port GPIOC

#define ROW_1_GPIO_Port GPIOA
#define ROW_2_GPIO_Port GPIOB
#define ROW_3_GPIO_Port GPIOB
#define ROW_4_GPIO_Port GPIOB


//Assigning the pins defined in the IOC file for the rows and columns.
#define COLUMN_1_Pin GPIO_PIN_10
#define COLUMN_2_Pin GPIO_PIN_8
#define COLUMN_3_Pin GPIO_PIN_9
#define COLUMN_4_Pin GPIO_PIN_7


#define ROW_1_Pin GPIO_PIN_10
#define ROW_2_Pin GPIO_PIN_3
#define ROW_4_Pin GPIO_PIN_4
#define ROW_3_Pin GPIO_PIN_5

#define KEY_DEBOUNCE_MS 120 /*!> Minimum time required for since last press */


void keyboard_init(void); //function that gives the initial values for the rows.

uint8_t keypad_handler(uint16_t column_to_evaluate); //function that returns the value of the key being pressed.

#endif /* INC_KEYBOARD_H_ */
