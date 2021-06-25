/*
 * board_utilities.c
 *
 * Containing definitions for functions declared in
 * board_utilities.h .
 *
 * Author: Adam Csizy
 * Created on: 2019.10.10.
 * Last modified: 2019.11.19.
 */

#include "em_chip.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_core.h"
#include "em_usart.h"
#include "em_gpio.h"
#include "em_emu.h"
#include "em_timer.h"
#include "em_adc.h"
#include "segmentlcd.h"
#include "segmentlcd_individual.h"
#include "board_utilities.h"
#include "game_utilities.h" // Declaration of extern uartInterruptFlag and boolean definition

SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS]; // Segment array declaration.

/*
 * Function 'UART0_RX_IRQHandler': handles UART0_RX interrupt requests short by redefining
 * the '.weak UART0_RX_IRQHandler' default IRQ handler.
 */
void UART0_RX_IRQHandler(void){

	/* Set interrupt flag for further process. */
	uartInterruptFlag = TRUE;

	/* Read UART0_Rx data. */
	uartInputValue = USART_RxDataGet(UART0);
	USART_Tx(UART0, uartInputValue); // For debugging

	/* Clear peripheral interrupt flag. */
	USART_IntClear(UART0, _USART_IFC_MASK);
}

/*
 * Function 'TIMER0_IRQHandler': handles TIMER0 interrupt requests short by redefining
 * the '.weak TIMER0_IRQHandler' default IRQ handler.
 */
void TIMER0_IRQHandler(void){

	/* Clear program halt flag. */
	haltFlag = false;
	
	/* Disable TIMER0 interrupt */
	TIMER_IntDisable(TIMER0, TIMER_IF_OF);

	/* Clear peripheral interrupt flag. */
	TIMER_IntClear(TIMER0, TIMER_IF_OF);
}

/*
 * Function 'initGeckoBoard': initializes hardware and software components essential
 * for the application.
 */
void initGeckoBoard(){

/////////////////////////////////////////////////////INIT CHIP/////////////////////////////////////////////////////////

	/* Chip errata. */
	CHIP_Init();

//////////////////////////////////////////////////////INIT LCD/////////////////////////////////////////////////////////

	/* Initialize the LCD display. */
	SegmentLCD_Init(false);

/////////////////////////////////////////////////////INIT UART0////////////////////////////////////////////////////////

	/* Enable clock for GPIO. */
	CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

	/* Set pin 7 in port F to high for UART communication. */
	GPIO_PinModeSet(gpioPortF, 7, gpioModePushPull, 1);

	/* Enable clock for UART0. */
	CMU_ClockEnable(cmuClock_UART0, true);

	/* Initialize UART0 with default values (115200 Baud, 8N1 frame format). */
	USART_InitAsync_TypeDef UART0_init = USART_INITASYNC_DEFAULT;
	USART_InitAsync(UART0, &UART0_init);

	/* Set TX (PE0) and RX (PE1) pins as push-pull output and input resp. */
	/* DOUT for TX is 1, as it is the idle state for UART communication. */
	GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull, 1);

	/* DOUT for RX is 0, as DOUT can enable a glitch filter for inputs,
	* and we are fine without such a filter. */
	GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 0);

	/* Use PE0 as TX and PE1 as RX (Location 1, see data sheet). */
	/* Enable both RX and TX for routing. */
	UART0->ROUTE |= UART_ROUTE_LOCATION_LOC1;

	/* Select "Location 1" as the routing configuration. */
	UART0->ROUTE |= UART_ROUTE_TXPEN | UART_ROUTE_RXPEN;

	/* Clear UART0 peripheral interrupt flag before enable UART0 interrupt. */
	USART_IntClear(UART0,_USART_IFC_MASK);
	USART_IntEnable(UART0, USART_IEN_RXDATAV); // USART_IEN_RXDATAV (data valid interrupt enable)

	/* Clear UART0_Rx PEND bit in Nested Vector Interrupt Controller (NVIC)
	 * before enable NVIC interrupt request for UART0_Rx. NVIC PEND bit
	 * is automatically cleared on calling the appropriate ISR for UART0_Rx IRQ.
	 * Note that peripheral interrupt flag must be cleared manually. */
	NVIC_ClearPendingIRQ(UART0_RX_IRQn);
	NVIC_EnableIRQ(UART0_RX_IRQn);

////////////////////////////////////////////////////////INIT TIMER0/////////////////////////////////////////////////////

	/* Set peripheral clock divisor. */
	CMU_ClockDivSet(cmuClock_HFPER, cmuClkDiv_1); // divisor: 1

	/* Enable clock for TIMER0. */
	CMU_ClockEnable(cmuClock_TIMER0, true);

	/* Initialize TIMER0 with default values. */
	TIMER_Init_TypeDef TIMER0_init = TIMER_INIT_DEFAULT;
	TIMER0_init.prescale = timerPrescale1024; // Timer prescaler: 1024
	TIMER_Init(TIMER0, &TIMER0_init);

	/* Reset counter. */
	TIMER_CounterSet(TIMER0, 0);

	/* Set TOP to default value. */
	TIMER_TopSet(TIMER0, 13672*(REACTION_TIME/1000)); // clk(14MHz)/prescaler(1024)*wait_sec(2 sec) = TOP

	/* Clear TIMER0 peripheral interrupt flag before enable TIMER0 interrupt. */
	TIMER_IntClear(TIMER0, TIMER_IF_OF);
	// TIMER_IntEnable(TIMER0, TIMER_IF_OF);

	/* Clear TIMER0 PEND bit in Nested Vector Interrupt Controller (NVIC)
	* before enable NVIC interrupt request for TIMER0. NVIC PEND bit
	* is automatically cleared on calling the appropriate ISR for TIMER0 IRQ.
	* Note that peripheral interrupt flag must be cleared manually. */
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);

	//////////////////////////////////////////////////INIT ADC////////////////////////////////////////////////////////

	/* Enable clock for ADC 0. */
	 CMU_ClockEnable(cmuClock_ADC0 ,true);
	 CMU_ClockEnable(cmuClock_HFPER, true);

	/* Initialize pin 5 in port D (noise source). */
	 GPIO_PinModeSet(gpioPortD, 5, gpioModeInput, 0);

	/* Base the ADC configuration on the default setup. */
	  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	  ADC_InitSingle_TypeDef sInit = ADC_INITSINGLE_DEFAULT;

	/* Initialize time bases. */
	  init.timebase = ADC_TimebaseCalc(0);
	  init.prescale = ADC_PrescaleCalc(400000, 0);
	  ADC_Init(ADC0, &init);

	/* Set input to temperature sensor. Reference must be 1.25V */
	  sInit.reference = adcRef1V25;
	  sInit.input = adcSingleInputCh5;
	  ADC_InitSingle(ADC0, &sInit);


	/* Start one ADC sample. */
	  ADC_Start(ADC0, adcStartSingle);

	/* Wait for valid single data (successive approximation requires some time). */
	  while(!(ADC0->STATUS & ADC_STATUS_SINGLEDV));

	/* Initialize global seed value by ADC. */
	  randGenSeed = ADC_DataSingleGet(ADC0);
}

/*
 * Function 'displayLevel': displays the given level on the upper LCD segments.
 * @param1: map level to be displayed.
 */
void displayLevel(uint16_t level){

	int8_t iter, digitArray[SEGMENT_LCD_NUM_OF_UPPER_CHARS]; // Array declaration for digit values.
	uint16_t divisor = 1000; // Initial divisor declaration.
	SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS]; // Segment array declaration.

	for(iter = (SEGMENT_LCD_NUM_OF_UPPER_CHARS - 1);iter > -1;--iter){ // Replicate level digits into an array.

			digitArray[iter] = level/divisor;
			level = level-divisor*digitArray[iter];
			divisor = divisor/10;
	}

	for(iter = 0;iter < SEGMENT_LCD_NUM_OF_UPPER_CHARS;++iter){ // Set digit segments one by one according to array values.

		switch(digitArray[iter]){

		case 0:

			upperCharSegments[iter].a = 1;
			upperCharSegments[iter].b = 1;
			upperCharSegments[iter].c = 1;
			upperCharSegments[iter].d = 1;
			upperCharSegments[iter].e = 1;
			upperCharSegments[iter].f = 1;
			upperCharSegments[iter].g = 0;
			break;

		case 1:

			upperCharSegments[iter].a = 0;
			upperCharSegments[iter].b = 1;
			upperCharSegments[iter].c = 1;
			upperCharSegments[iter].d = 0;
			upperCharSegments[iter].e = 0;
			upperCharSegments[iter].f = 0;
			upperCharSegments[iter].g = 0;
			break;

		case 2:

			upperCharSegments[iter].a = 1;
			upperCharSegments[iter].b = 1;
			upperCharSegments[iter].c = 0;
			upperCharSegments[iter].d = 1;
			upperCharSegments[iter].e = 1;
			upperCharSegments[iter].f = 0;
			upperCharSegments[iter].g = 1;
			break;

		case 3:

			upperCharSegments[iter].a = 1;
			upperCharSegments[iter].b = 1;
			upperCharSegments[iter].c = 1;
			upperCharSegments[iter].d = 1;
			upperCharSegments[iter].e = 0;
			upperCharSegments[iter].f = 0;
			upperCharSegments[iter].g = 1;
			break;

		case 4:

			upperCharSegments[iter].a = 0;
			upperCharSegments[iter].b = 1;
			upperCharSegments[iter].c = 1;
			upperCharSegments[iter].d = 0;
			upperCharSegments[iter].e = 0;
			upperCharSegments[iter].f = 1;
			upperCharSegments[iter].g = 1;
			break;

		case 5:

			upperCharSegments[iter].a = 1;
			upperCharSegments[iter].b = 0;
			upperCharSegments[iter].c = 1;
			upperCharSegments[iter].d = 1;
			upperCharSegments[iter].e = 0;
			upperCharSegments[iter].f = 1;
			upperCharSegments[iter].g = 1;
			break;

		case 6:

			upperCharSegments[iter].a = 1;
			upperCharSegments[iter].b = 0;
			upperCharSegments[iter].c = 1;
			upperCharSegments[iter].d = 1;
			upperCharSegments[iter].e = 1;
			upperCharSegments[iter].f = 1;
			upperCharSegments[iter].g = 1;
			break;

		case 7:

			upperCharSegments[iter].a = 1;
			upperCharSegments[iter].b = 1;
			upperCharSegments[iter].c = 1;
			upperCharSegments[iter].d = 0;
			upperCharSegments[iter].e = 0;
			upperCharSegments[iter].f = 0;
			upperCharSegments[iter].g = 0;
			break;

		case 8:

			upperCharSegments[iter].a = 1;
			upperCharSegments[iter].b = 1;
			upperCharSegments[iter].c = 1;
			upperCharSegments[iter].d = 1;
			upperCharSegments[iter].e = 1;
			upperCharSegments[iter].f = 1;
			upperCharSegments[iter].g = 1;
			break;

		case 9:

			upperCharSegments[iter].a = 1;
			upperCharSegments[iter].b = 1;
			upperCharSegments[iter].c = 1;
			upperCharSegments[iter].d = 1;
			upperCharSegments[iter].e = 0;
			upperCharSegments[iter].f = 1;
			upperCharSegments[iter].g = 1;
			break;
		}
	}

	SegmentLCD_UpperSegments(upperCharSegments); // Update segment status and LCD display.
}

/*
 * Function 'wait': halts the program for the time determined by
 * the timer's TOP value.
 */
void wait(void){

	TIMER_CounterSet(TIMER0, 0); // Set counter to zero.

	TIMER_IntClear(TIMER0, TIMER_IF_OF); // Clear peripheral interrupt flag.
	TIMER_IntEnable(TIMER0, TIMER_IF_OF); // Enable peripheral interrupt.

	// haltFlag = true; // Uncomment this line to turn on fixed spaceship speed.

	/* Option No. 1 */
	EMU_EnterEM1(); // EMU_EnterEM1();
	
	/* Wait until program halt flag is true. */
	// while(haltFlag){}; // Uncomment this line to turn on fixed spaceship speed.
}

/*
 * Function 'blinkingDotsOn': starts blinking the decimal point segments on
 * the lower LCD display by turning them on and off.
 * @param1: decimal point display status.
 */
void blinkingDotsOn(uint8_t *displayStatus){

	if(*displayStatus){ // If display status is true, turn off segmentrs and overwrite status.

		SegmentLCD_Symbol(LCD_SYMBOL_DP2, 0);
		SegmentLCD_Symbol(LCD_SYMBOL_DP3, 0);
		SegmentLCD_Symbol(LCD_SYMBOL_DP4, 0);
		SegmentLCD_Symbol(LCD_SYMBOL_DP5, 0);
		SegmentLCD_Symbol(LCD_SYMBOL_DP6, 0);
		*displayStatus = 0;
	}
	else{ // If display status is false, turn on segments and overwrite status.

		SegmentLCD_Symbol(LCD_SYMBOL_DP2, 1);
		SegmentLCD_Symbol(LCD_SYMBOL_DP3, 1);
		SegmentLCD_Symbol(LCD_SYMBOL_DP4, 1);
		SegmentLCD_Symbol(LCD_SYMBOL_DP5, 1);
		SegmentLCD_Symbol(LCD_SYMBOL_DP6, 1);
		*displayStatus = 1;
	}
}

/*
 * Function 'blinkingDotsOff': stops blinking the decimal point segments on
 * the lower LCD display by turning them off.
 * @param1: decimal point display status.
 */
void blinkingDotsOff(uint8_t *displayStatus){

	SegmentLCD_Symbol(LCD_SYMBOL_DP2, 0);
	SegmentLCD_Symbol(LCD_SYMBOL_DP3, 0);
	SegmentLCD_Symbol(LCD_SYMBOL_DP4, 0);
	SegmentLCD_Symbol(LCD_SYMBOL_DP5, 0);
	SegmentLCD_Symbol(LCD_SYMBOL_DP6, 0);
	*displayStatus = 0;
}

/*
 * Function 'clearMap': clears the map by clearing all LCD segment.
 */
void clearMap(void){

	uint8_t digit;

	for(digit = 0;digit < SEGMENT_LCD_NUM_OF_LOWER_CHARS;++digit) // Clear each segment per digit using the 'raw' field.
		lowerCharSegments[digit].raw = 0;

	SegmentLCD_LowerSegments(lowerCharSegments); // Update segment status and LCD display.
}

/*
 * Function 'displayObject': displays the given map object by setting
 * the appropriate LCD segment. If segment 'G' is given, segment 'M'
 * is displayed as well.
 * @param1: digit containing the segment to be set.
 * @param2: segment to be set.
 */
void displayObject(uint8_t digit, uint8_t segment){

	switch(segment){ // Set segment by input parameter.

	case 'A':

		lowerCharSegments[digit].a = 1; // Set segment A.
		break;

	case 'F':

		lowerCharSegments[digit].f = 1; // Set segment F.
		break;

	case 'G':

		lowerCharSegments[digit].g = 1; // Set segment G.
		lowerCharSegments[digit].m = 1; // Set segment M.
		break;

	case 'E':

		lowerCharSegments[digit].e = 1; // Set segment E.
		break;

	case 'D':

		lowerCharSegments[digit].d = 1; // Set segment D.
		break;
	}

	SegmentLCD_LowerSegments(lowerCharSegments); // Update segment status and LCD display.
}

/*
 * Function 'clearObject': clears the given map object by clearing
 * the appropriate LCD segment. If segment 'G' is given, segment 'M'
 * is cleared as well.
 * @param1: digit containing the segment to be cleared.
 * @param2: segment to be cleared.
 */
void clearObject(uint8_t digit, uint8_t segment){

		switch(segment){ //  Clear segment by input parameter.

		case 'A':

			lowerCharSegments[digit].a = 0; // Clear segment A.
			break;

		case 'F':

			lowerCharSegments[digit].f = 0; // Clear segment F.
			break;

		case 'G':

			lowerCharSegments[digit].g = 0; // Clear segment G.
			lowerCharSegments[digit].m = 0; // Clear segment M.
			break;

		case 'E':

			lowerCharSegments[digit].e = 0; // Clear segment E.
			break;

		case 'D':

			lowerCharSegments[digit].d = 0; // Clear segment D.
			break;
		}

		SegmentLCD_LowerSegments(lowerCharSegments); // Update segment status and LCD display.
}
