/*
 * game_utilities.c
 *
 * Containing definitions for functions declared in
 * game_utilities.h .
 *
 * Author: Adam Csizy
 * Created on: 2019.10.04.
 * Last modified: 2019.11.15.
 */

#include "em_usart.h"
#include <stdlib.h>
#include "game_utilities.h"
#include "board_utilities.h"

/*
 * Function 'initMap': initializes the structure of the given map.
 * @param1: MapElement (segment) array to be initialized.
 */
void initMap(MapElement map[]){

	/* Digit field parameter reduced by 1 due to implementation concerns. */

	/* Digit 1 segment A */
	map[0].digit = 0;
	map[0].segment = 'A';
	map[0].nextForward = 3; // Digit 2 segment A
	map[0].nextLeft = 6; // Digit 2 segment E
	map[0].nextRight = 4; // Digit 2 segment F

	/* Digit 1 segment G */
	map[1].digit = 0;
	map[1].segment = 'G';
	map[1].nextForward = 5; // Digit 2 segment G
	map[1].nextLeft = 4; // Digit 2 segment F
	map[1].nextRight = 6; // Digit 2 segment E

	/* Digit 1 segment D */
	map[2].digit = 0;
	map[2].segment = 'D';
	map[2].nextForward = 7; // Digit 2 segment D
	map[2].nextLeft = 6; // Digit 2 segment E
	map[2].nextRight = 4; // Digit 2 segment F

	/* Digit 2 segment A */
	map[3].digit = 1;
	map[3].segment = 'A';
	map[3].nextForward = 8; // Digit 3 segment A
	map[3].nextLeft = 11; // Digit 3 segment E
	map[3].nextRight = 9; // Digit 3 segment F

	/* Digit 2 segment F */
	map[4].digit = 1;
	map[4].segment = 'F';
	map[4].nextForward = 6; // Digit 2 segment E
	map[4].nextLeft = 5; // Digit 2 segment G
	map[4].nextRight = 3; // Digit 2 segment A

	/* Digit 2 segment G */
	map[5].digit = 1;
	map[5].segment = 'G';
	map[5].nextForward = 10; // Digit 3 segment G
	map[5].nextLeft = 9; // Digit 3 segment F
	map[5].nextRight = 11; // Digit 3 segment E

	/* Digit 2 segment E */
	map[6].digit = 1;
	map[6].segment = 'E';
	map[6].nextForward = 4; // Digit 2 segment F
	map[6].nextLeft = 7; // Digit 2 segment D
	map[6].nextRight = 5; // Digit 2 segment G

	/* Digit 2 segment D */
	map[7].digit = 1;
	map[7].segment = 'D';
	map[7].nextForward = 12; // Digit 3 segment D
	map[7].nextLeft = 11; // Digit 3 segment E
	map[7].nextRight = 9; // Digit 3 segment F

	/* Digit 3 segment A */
	map[8].digit = 2;
	map[8].segment = 'A';
	map[8].nextForward = 13; // Digit 4 segment A
	map[8].nextLeft = 16; // Digit 4 segment E
	map[8].nextRight = 14; // Digit 4 segment F

	/* Digit 3 segment F */
	map[9].digit = 2;
	map[9].segment = 'F';
	map[9].nextForward = 11; // Digit 3 segment E
	map[9].nextLeft = 10; // Digit 3 segment G
	map[9].nextRight = 8; // Digit 3 segment A

	/* Digit 3 segment G */
	map[10].digit = 2;
	map[10].segment = 'G';
	map[10].nextForward = 15; // Digit 4 segment G
	map[10].nextLeft = 14; // Digit 4 segment F
	map[10].nextRight = 16; // Digit 4 segment E

	/* Digit 3 segment E */
	map[11].digit = 2;
	map[11].segment = 'E';
	map[11].nextForward = 9; // Digit 3 segment F
	map[11].nextLeft = 12; // Digit 3 segment D
	map[11].nextRight = 10; // Digit 3 segment G

	/* Digit 3 segment D */
	map[12].digit = 2;
	map[12].segment = 'D';
	map[12].nextForward = 17; // Digit 4 segment D
	map[12].nextLeft = 16; // Digit 4 segment E
	map[12].nextRight = 14; // Digit 4 segment F

	/* Digit 4 segment A */
	map[13].digit = 3;
	map[13].segment = 'A';
	map[13].nextForward = 18; // Digit 5 segment A
	map[13].nextLeft = 21; // Digit 5 segment E
	map[13].nextRight = 19; // Digit 5 segment F

	/* Digit 4 segment F */
	map[14].digit = 3;
	map[14].segment = 'F';
	map[14].nextForward = 16; // Digit 4 segment E
	map[14].nextLeft = 15; // Digit 4 segment G
	map[14].nextRight = 13; // Digit 4 segment A

	/* Digit 4 segment G */
	map[15].digit = 3;
	map[15].segment = 'G';
	map[15].nextForward = 20; // Digit 5 segment G
	map[15].nextLeft = 19; // Digit 5 segment F
	map[15].nextRight = 21; // Digit 5 segment E

	/* Digit 4 segment E */
	map[16].digit = 3;
	map[16].segment = 'E';
	map[16].nextForward = 14; // Digit 4 segment F
	map[16].nextLeft = 17; // Digit 4 segment D
	map[16].nextRight = 15; // Digit 4 segment G

	/* Digit 4 segment D */
	map[17].digit = 3;
	map[17].segment = 'D';
	map[17].nextForward = 22; // Digit 5 segment D
	map[17].nextLeft = 21; // Digit 5 segment E
	map[17].nextRight = 19; // Digit 5 segment F

	/* Digit 5 segment A */
	map[18].digit = 4;
	map[18].segment = 'A';
	map[18].nextForward = 23; // Digit 6 segment A
	map[18].nextLeft = 26; // Digit 6 segment E
	map[18].nextRight = 24; // Digit 6 F

	/* Digit 5 segment F */
	map[19].digit = 4;
	map[19].segment = 'F';
	map[19].nextForward = 21; // Digit 5 segment E
	map[19].nextLeft = 20; // Digit 5 segment G
	map[19].nextRight = 18; // Digit 5 segment A

	/* Digit 5 segment G */
	map[20].digit = 4;
	map[20].segment = 'G';
	map[20].nextForward = 25; // Digit 6 segment G
	map[20].nextLeft = 24; // Digit 6 segment F
	map[20].nextRight = 26; // Digit 6 segment E

	/* Digit 5 segment E */
	map[21].digit = 4;
	map[21].segment = 'E';
	map[21].nextForward = 19; // Digit 5 segment F
	map[21].nextLeft = 22; // Digit 5 segment D
	map[21].nextRight = 20; // Digit 5 segment G

	/* Digit 5 segment D */
	map[22].digit = 4;
	map[22].segment = 'D';
	map[22].nextForward = 27; // Digit 6 segment D
	map[22].nextLeft = 26; // Digit 6 segment E
	map[22].nextRight = 24; // Digit 6 segment F

	/* Digit 6 segment A */
	map[23].digit = 5;
	map[23].segment = 'A';
	map[23].nextForward = 28; // Digit 7 segment A
	map[23].nextLeft = 31; // Digit 7 segment E
	map[23].nextRight = 29; // Digit 7 segment F

	/* Digit 6 segment F */
	map[24].digit = 5;
	map[24].segment = 'F';
	map[24].nextForward = 26; // Digit 6 segment E
	map[24].nextLeft = 25; // Digit 6 segment G
	map[24].nextRight = 23; // Digit 6 segment A

	/* Digit 6 segment G */
	map[25].digit = 5;
	map[25].segment = 'G';
	map[25].nextForward = 30; // Digit 7 segment G
	map[25].nextLeft = 29; // Digit 7 segment F
	map[25].nextRight = 31; // Digit 7 segment E

	/* Digit 6 segment E */
	map[26].digit = 5;
	map[26].segment = 'E';
	map[26].nextForward = 24; // Digit 6 segment F
	map[26].nextLeft = 27; // Digit 6 segment D
	map[26].nextRight = 25; // Digit 6 segment G

	/* Digit 6 segment D */
	map[27].digit = 5;
	map[27].segment = 'D';
	map[27].nextForward = 32; // Digit 7 segment D
	map[27].nextLeft = 31; // Digit 7 segment E
	map[27].nextRight = 29; // Digit 7 segment F

	/* Digit 7 segment A */
	map[28].digit = 6;
	map[28].segment = 'A';
	map[28].nextForward = INVALID_MAP_INDEX;
	map[28].nextLeft = INVALID_MAP_INDEX;
	map[28].nextRight = INVALID_MAP_INDEX;

	/* Digit 7 segment F */
	map[29].digit = 6;
	map[29].segment = 'F';
	map[29].nextForward = 31; // Digit 7 segment E
	map[29].nextLeft = 30; // Digit 7 segment G
	map[29].nextRight = 28; // Digit 7 segment A

	/* Digit 7 segment G */
	map[30].digit = 6;
	map[30].segment = 'G';
	map[30].nextForward = INVALID_MAP_INDEX;
	map[30].nextLeft = INVALID_MAP_INDEX;
	map[30].nextRight = INVALID_MAP_INDEX;

	/* Digit 7 segment E */
	map[31].digit = 6;
	map[31].segment = 'E';
	map[31].nextForward = 29; // Digit 7 segment F
	map[31].nextLeft = 32; // Digit 7 segment D
	map[31].nextRight = 30; // Digit 7 segment G

	/* Digit 7 segment D */
	map[32].digit = 6;
	map[32].segment = 'D';
	map[32].nextForward = INVALID_MAP_INDEX;
	map[32].nextLeft = INVALID_MAP_INDEX;
	map[32].nextRight = INVALID_MAP_INDEX;

}

/*
 * Function 'initObjectPositions': initializes the map object positions with reference
 * for the appropriate MapElement (segment) array index. Array indexes are generated
 * randomly. Source: https://www.geeksforgeeks.org/generating-random-number-range-c/
 * @param1: map for position references.
 * @param2: array containing the object positions to be initialized.
 * @param3: space ship orientation.
 * @param4: level up flag.
 */
void initObjectPositions(const MapElement map[], uint8_t objectPositions[], Orientation *orientation, uint8_t levelUp){

	uint8_t i, k, randPos, posTaken; // Iterators, auxiliary iterators, flags.
	uint8_t upper = (MAP_SIZE-1); // Upper limit of random number generation domain.
	uint8_t lower = 0; // Lower limit of random number generation domain.

	if(levelUp){

		/* Place space ship on the first digit segment in the equivalent row with the actual position on level up. */
		switch(objectPositions[SPACE_SHIP_ACT]){

		case 28: // Digit 7 segment A
			objectPositions[SPACE_SHIP_ACT] = 0; // Digit 1 segment A
			break;

		case 30: // Digit 7 segment G
			objectPositions[SPACE_SHIP_ACT] = 1; // Digit 1 segment G
			break;

		case 32: // Digit 7 segment D
			objectPositions[SPACE_SHIP_ACT] = 2; // Digit 1 segment D
			break;
		}
	}
	else{

		/* Default start position and orientation of the space ship. */
		objectPositions[SPACE_SHIP_ACT] = 1; // Digit 1 segment G
		*orientation = HORIZONTAL;
	}

	for(i = ASTEROID_1;i < OBJECT_POSITIONS_ARRAY_SIZE;++i){

		do{

			posTaken = FALSE; // Clear 'position taken' flag.
			randPos = (rand() % (upper - lower + 1)) + lower; // Generate random map array index for random object position.

			for(k = SPACE_SHIP_ACT;k < i;++k){ // Check if position is already taken by previous objects.

			if(objectPositions[k] == randPos)
				posTaken = TRUE; // Set 'position taken' flag.
			}
		}
		while(posTaken == TRUE); // Generate new random map array index if the current index is already taken.

		objectPositions[i] = randPos; // Place object to position.
	}

	clearMap(); // Clear all lower LCD segments.

	for(i = SPACE_SHIP_ACT;i < OBJECT_POSITIONS_ARRAY_SIZE;++i){ // Display map objects as LCD segments.

		displayObject(map[objectPositions[i]].digit, map[objectPositions[i]].segment);
	}
}

/*
 * Function 'uartInterruptHandler': handles UART0_RX interrupt requests long by updating
 * spaceship direction or reset status based on the serial input data.
 * @param1: reset status to be updated.
 * @param2: spaceship direction to be updated.
 * @param3: game state.
 */
void uartInterruptHandler(uint8_t *resetStatus, Direction *direction, const GameState *gameState){

	/* Update direction or reset status. */
	switch(uartInputValue){

	case 'b':

		*direction = LEFT;
		break;

	case 'j':

		*direction = RIGHT;
		break;

	case 'r':

		if(END_GAME == *gameState)
			*resetStatus = TRUE;
		else
			*direction = FORWARD;
		break;

	default:

		*direction = FORWARD;
		break;
	}
}

/*
 * Function 'move': moves the space ship to the next position defined by
 * its orientation, direction and the map structure.
 * @param1: map for position references.
 * @param2: array containing the map object positions.
 * @param3: space ship orientation.
 * @param4: space ship direction.
 */
void move(const MapElement map[], uint8_t objectPositions[], Orientation *orientation, Direction *direction){

	switch(*direction){

	case FORWARD: // Move space ship forward request.

		clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
		objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextForward; // Overwrite actual position with the next position (MapElement array index reference).
		displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
		// Orientation does not change.
		break;

	case LEFT: // Move space ship left request.

		switch(map[objectPositions[SPACE_SHIP_ACT]].segment){ // Filter special segment position cases.

		case 'F': // Any digit segment F

			if(*orientation == DOWN){ // Actual orientation DOWN

				clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
				objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextLeft; // Overwrite actual position with the next position (MapElement array index reference).
				displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
				*orientation = HORIZONTAL; // Set new orientation. DOWN + LEFT => HORIZONTAL
			}
			else{ // Actual orientation UP

				clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
				objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextForward; // Overwrite actual position with the next position (MapElement array index reference).
				displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
				// Orientation does not change. UP + LEFT => FORBIDDEN
			}
			break;

		case 'E': // Any digit segment E

			if(*orientation == DOWN){ // Actual orientation DOWN

				clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
				objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextLeft; // Overwrite actual position with the next position (MapElement array index reference).
				displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
				*orientation = HORIZONTAL; // Set new orientation. DOWN + LEFT => HORIZONTAL
			}
			else{ // Actual orientation UP

				clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
				objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextForward; // Overwrite actual position with the next position (MapElement array index reference).
				displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
				// Orientation does not change. UP + LEFT => FORBIDDEN
			}
			break;

		default:

			clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
			objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextLeft; // Overwrite actual position with the next position (MapElement array index reference).
			displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
			*orientation = UP; // Set new orientation. HORIZONTAL + LEFT => UP
			break;
		}

		break;

	case RIGHT: // Move space ship right request.

		switch(map[objectPositions[SPACE_SHIP_ACT]].segment){ // Filter special segment position cases.

		case 'F': // Any digit segment F

			if(*orientation == UP){ // Actual orientation UP

				clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
				objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextRight; // Overwrite actual position with the next position (MapElement array index reference).
				displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
				*orientation = HORIZONTAL; // Set new orientation. UP + RIGHT => HORIZONTAL
			}
			else{ // Actual orientation DOWN

				clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
				objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextForward; // Overwrite actual position with the next position (MapElement array index reference).
				displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
				// Orientation does not change. DOWN + RIGHT => FORBIDDEN
			}
			break;

		case 'E': // Any digit segment E

			if(*orientation == UP){ // Actual orientation UP

				clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
				objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextRight; // Overwrite actual position with the next position (MapElement array index reference).
				displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
				*orientation = HORIZONTAL; // Set new orientation. UP + RIGHT => HORIZONTAL
			}
			else{ // Actual orientation DOWN

				clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
				objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextForward; // Overwrite actual position with the next position (MapElement array index reference).
				displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
				// Orientation does not change. DOWN + RIGHT => FORBIDDEN
			}
			break;

		default:

			clearObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Clear space ship position.
			objectPositions[SPACE_SHIP_ACT] = map[objectPositions[SPACE_SHIP_ACT]].nextRight; // Overwrite actual position with the next position (MapElement array index reference).
			displayObject(map[objectPositions[SPACE_SHIP_ACT]].digit, map[objectPositions[SPACE_SHIP_ACT]].segment); // Display new space ship position.
			*orientation = DOWN; // Set new orientation. HORIZONTAL + RIGHT => DOWN
			break;
		}
		break;
	}

}

/*
 * Function 'isAsteroid': compares the actual position of the space ship
 * with the position of the asteroid objects.
 * @param1: array containing the map object positions.
 * @return: TRUE (match) or FALSE (no match).
 */
uint8_t isAsteroid(const uint8_t objectPositions[]){

	int i;

	for(i = ASTEROID_1;i < OBJECT_POSITIONS_ARRAY_SIZE;++i){

		if(objectPositions[i] == objectPositions[SPACE_SHIP_ACT])
			return TRUE;
	}

	return FALSE;
}

/*
 * Function 'isEndOfMap': compares the actual position of the space ship
 * with map elements (segments) representing the end of the map.
 * @param1: array containing the map object positions.
 * @return: TRUE (match) or FALSE (no match).
 */
uint8_t isEndOfMap(const uint8_t objectPositions[]){

	if(objectPositions[SPACE_SHIP_ACT] == 32) // Digit 7 segment D
		return TRUE;

	if(objectPositions[SPACE_SHIP_ACT] == 30) // Digit 7 segment G
		return TRUE;

	if(objectPositions[SPACE_SHIP_ACT] == 28) // Digit 7 segment A
		return TRUE;

	return FALSE;
}
