/*
 * game_utilities.h
 *
 * Header containing type definitions, macros and function
 * declarations used in main.c .
 *
 * Author: Adam Csizy
 * Created on: 2019.10.04.
 * Last modified: 2019.11.19.
 */

#include "em_device.h"

#ifndef SRC_ADDONS_H_
#define SRC_ADDONS_H_


// Macros used in main.c and game_utilities.c

#define TRUE 1
#define FALSE 0

#define REACTION_TIME 2000 // Offered reaction time for the user in milliseconds

#define MAP_SIZE 33 // Number of map LCD segments
#define OBJECT_POSITIONS_ARRAY_SIZE 4 // Number of LCD segments displayed simultaneously + 1

#define INVALID_MAP_INDEX 99 // Invalid map array index (uint8_t)
#define SPACE_SHIP_ACT 0 // Space ship object actual position array index
#define ASTEROID_1 1 // Asteroid #1 object position array index
#define ASTEROID_2 2 // Asteroid #2 object position array index
#define ASTEROID_3 3 // Asteroid #3 object position array index

// Enumeration and structure type definitions used in main.c

typedef enum{FORWARD, LEFT, RIGHT} Direction; // Enumeration type for space ship direction.
typedef enum{NEW_MAP, IN_GAME, END_GAME} GameState; // Enumeration type for game state.
typedef enum{HORIZONTAL, UP, DOWN} Orientation; // Enumeration type for space ship orientation.

typedef struct{

	uint8_t digit; // 0 - 6
	uint8_t segment; // A,B,C,D,E,F,G,H,J,K,M,N,P,Q

	uint8_t nextForward; // Next forward map element (segment) referenced by its map array index.
	uint8_t nextLeft; // Next left map element (segment) referenced by its map array index.
	uint8_t nextRight; // Next right map element (segment) referenced by its map array index.

}MapElement; // Structure type for map element (segment).

// Global variable declarations

extern volatile uint8_t uartInterruptFlag; // Global UART0_Rx interrupt flag
extern volatile uint32_t randGenSeed; // Global seed value
extern volatile int uartInputValue; // Global actual UART0_Rx data
extern volatile uint8_t haltFlag; // Global halt flag for program halt status.

// Function declarations used in main.c

void initMap(MapElement map[]);
void initObjectPositions(const MapElement map[], uint8_t objectPositions[], Orientation *orientation, uint8_t levelUp);
void uartInterruptHandler(uint8_t *resetStatus, Direction *direction, const GameState *gameState);
void move(const MapElement map[], uint8_t objectPositions[], Orientation *orientation, Direction *direction);
uint8_t isAsteroid(const uint8_t objectPositions[]);
uint8_t isEndOfMap(const uint8_t objectPositions[]);


#endif /* SRC_ADDONS_H_ */
