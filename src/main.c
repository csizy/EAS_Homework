/*
 * main.c
 *
 * Main program component of the application developed
 * for Silabs EFM32 Giant Gecko Starter Kit (STK3700).
 *
 * Author: Adam Csizy
 * Created on: 2019.10.03.
 * Last modified: 2019.11.19.
 */

#include <stdlib.h>
#include "em_device.h"
#include "game_utilities.h"
#include "board_utilities.h"


// Global variable definitions

volatile uint8_t uartInterruptFlag = FALSE; // UART0_Rx interrupt flag, default value FALSE.
volatile uint8_t uartInputValue; // Global variable for actual UART0_Rx data storage.
volatile uint32_t randGenSeed; // Seed value by ADC noise for random number generation.
volatile uint8_t haltFlag; // Program halt flag.

// Main function of the program

int main(void)
{
	/* Main function variables */

	Direction direction; // Spaceship direction modified by interrupt, default value: FORWARD.
	uint8_t resetStatus = FALSE; // Game reset status, default value FALSE.
	uint16_t levelCounter; // Number of completed levels stored in a 16 bit unsigned integer. Maximal value: 9999 (to prevent 4 digit LCD overflow).
	uint8_t levelUp = FALSE; // Level up flag set on by a passed level, cleared by a failed level, default value: FALSE.
	uint8_t decimalDotDisplayStatus = 0; // Decimal point display status, default value: FALSE.
	GameState state = NEW_MAP; // Game state, default value: NEW_MAP.

	Orientation orientation; // Space ship orientation.
	uint8_t objectPositions[OBJECT_POSITIONS_ARRAY_SIZE]; // Array containing the map object positions including the space ship and 3 asteroids.
	MapElement map[MAP_SIZE]; // Array containing the map structure.

	/* Initializations */

	initGeckoBoard(); // Initialization of Silabs EFM32 Giant Gecko Starter Kit (STK3700) board.
	initMap(map); // Map structure initialization.
	srand(randGenSeed); // Seeds the pseudo-random number generator.

	// USART_Tx(UART0, 'A'); // For debugging.

	/* Infinite loop and state machine */

	while(1){

		switch(state){

		case NEW_MAP: // New map state if level up, reset(game over) or power on.

			direction = FORWARD; // Set default direction.
			initObjectPositions(map, objectPositions, &orientation, levelUp); // Place spaceship and 3 asteroids on the map.

			if(levelUp){ // If new level, then increase and display the level counter.

				if(levelCounter < 9999) // Prevent 4 digit LCD display from overflow.
					displayLevel(++levelCounter);
				else
					displayLevel((levelCounter-=9999)); // Reset level counter if overflow.
			}
			else{

				levelCounter=0; // Reset level counter.
				displayLevel(levelCounter); // Display level.
			}

			wait(); // Wait REACTION_TIME amount of time for user interaction.

			if(uartInterruptFlag){ // Check if direction is modified before moving spaceship.

				uartInterruptFlag = FALSE; // Clear interrupt flag.
				uartInterruptHandler(&resetStatus, &direction, &state); // Call ISR.
			}

			move(map, objectPositions, &orientation, &direction); // Move ship based on direction.
			direction = FORWARD; // Set default direction.

			if(isAsteroid(objectPositions)){ // Check if asteroid hit the spaceship.
				levelUp = FALSE;
				state = END_GAME;
				break;
			}

			state = IN_GAME;
			break;

		case IN_GAME: // In game state, spaceship is alive.

			wait(); // Wait REACTION_TIME amount of time for user interaction.

			if(uartInterruptFlag){ // Check if direction is modified before moving spaceship.

				uartInterruptFlag = FALSE; // Clear interrupt flag.
				uartInterruptHandler(&resetStatus, &direction, &state); // Call ISR.
			}

			move(map, objectPositions, &orientation, &direction); // Move ship based on direction.
			direction = FORWARD; // Set default direction.

			if(isAsteroid(objectPositions)){ // Check if asteroid hit the spaceship.
				levelUp = FALSE;
				state = END_GAME;
				break;
			}

			if(isEndOfMap(objectPositions)){ // Check if spaceship reached end of the map.
				levelUp = TRUE;
				state = NEW_MAP;
				wait();
				break;
			}

			state = IN_GAME;
			break;

		case END_GAME: // End of the game state, caused by asteroid hit (game over).

			blinkingDotsOn(&decimalDotDisplayStatus); // Blink dots on display.
			wait(); // Wait REACTION_TIME amount of time for user interaction.

			if(uartInterruptFlag){ // Check if reset status is modified.

				uartInterruptFlag = FALSE; // Clear interrupt flag.
				uartInterruptHandler(&resetStatus, &direction, &state); // Call ISR.
			}

			if(resetStatus){
				blinkingDotsOff(&decimalDotDisplayStatus);
				state = NEW_MAP;
				resetStatus = FALSE;
				break;
			}

			state = END_GAME;
			break;
		}
	}
}

/*
 * 	Development 'TODO' list:
 *	Reduce redundancy and code size by replacing map-segment array with a complex control
 *	logic in function 'move()' using only segment and digit IDs as reference.
 *
 *  	Common file paths for factory provided drivers and function libraries:
 *  	Peripherals: C:\SiliconLabs\SimplicityStudio\v4\developer\sdks\gecko_sdk_suite\v2.6\hardware\kit\common\drivers
 *  	Emlibs: C:\SiliconLabs\SimplicityStudio\v4\developer\sdks\gecko_sdk_suite\v2.6\platform\emlib\src
 *
 *	Silicon Labs EFM32 Giant Gecko Software Documentation:
 *	https://siliconlabs.github.io/Gecko_SDK_Doc/efm32gg/html/modules.html
 *
 *
 *  	How not to mess up UART communication on debug:
 *
 *  	1. Disconnect from development kit in the debugger if kit is already connected to PC.
 *  	2. Press reset button on the development board and wait at least 5 seconds.
 *  	3. Reconnect development kit to PC.
 *  	4. Wait until the operating system identifies the connected development kit.
 *  	5. Download program to development kit from debugger.
 *  	6. Open serial communication port in Putty before running the program in the debugger. Do not send data via serial communication port.
 *  	7. Run program in debugger.
 *  	8. Now we are allowed to send data via serial communication port in Putty.
 */

