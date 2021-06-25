/*
 * board_utilities.h
 *
 * Header containing function declarations
 * used in main.c and game_utilities.c .
 *
 * Author: Adam Csizy
 * Created on: 2019.10.10.
 * Last modified: 2019.11.15.
 */

#include "em_device.h"

#ifndef SRC_BOARD_UTILITIES_H_
#define SRC_BOARD_UTILITIES_H_


// Function declarations used in main.c and game_utilities.c

void UART0_RX_IRQHandler(void)__attribute__((interrupt("IRQ")));
void TIMER0_IRQHandler(void)__attribute__((interrupt("IRQ")));
void initGeckoBoard();
void displayLevel(uint16_t);
void wait(void);
void blinkingDotsOn(uint8_t *displayStatus);
void blinkingDotsOff(uint8_t *displayStatus);
void clearMap(void);
void displayObject(uint8_t, uint8_t);
void clearObject(uint8_t, uint8_t);


#endif /* SRC_BOARD_UTILITIES_H_ */
