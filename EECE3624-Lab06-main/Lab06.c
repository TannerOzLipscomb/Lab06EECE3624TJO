/*
 * Lab06.c
 *
 * Created: 10/4/2022 9:16:57 AM
 * Initial Coder: jfhutton
 * Current Coder: <Tanner Ozanich>
 * Modified:      <10/06/26>
 *
 * This lab uses hardware LEDs wired to PORTA, an interrupt from the joystick center
 * button, and a timer interrupt to build a game loop program that can display 
 * different patterns on the LEDs.
 *
 * While there are many "down and dirty" ways to get this coding done, try to 
 * remember your coding and data structures classes.  Things like ENUM, Arrays, Functions
 * could help make for more elegant coding.
 *
 */ 

#include <avr/io.h>              // Needed for AVR IO defines
#include <avr/interrupt.h>       // Needed for AVR interupt devines

#define LEDS            PORTA    // alias PORTA

	
// global variables for communication between ISRs and main
const unsigned char TCNT0_COUNT_SET = 0x8E;// Count for 1ms loop (Provided by Prof Hutton)

volatile unsigned char mode = 0; // mode tracking variable
volatile unsigned char tick = 0; // timer variable. each tick = 1ms
unsigned char LED = 0;           // variable used in pattern generation
unsigned char mode2 = 0;		 // helper variable used for mode 2

int main(void){
	// Initialization for LEDs
	// Set direction for A ports.
	DDRA = 0xFF;  // Set the Direction for all PORTA pints to be outputs
	LEDS = 0xFF;  // Set the PORTA for all pins to be high (i.e. OFF)
	// Initialization for Timer Interrupt
	TCCR0 = (1<<CS02);
	TCNT0 = TCNT0_COUNT_SET;
	TIMSK = (1<<TOIE0);
	// Port Initialization
	DDRD &= 0xFE;
	PORTD |= 0x01;
	// Interrupt Enable Block
	EICRA = 0x03;  
	EIMSK = 0x01;  
	// Enable Global Interrupts
	sei();
	// Main Loop
	while (1) {
		if (tick == 100){ // triggers LED change at 100ms 
			tick = 0;
			switch(mode){
				case(0): // first case right to left
				if (LED == 7){
					LED = 0;
				}else{
					LED++;
				}
				LEDS = ~(1 << LED);
				break;
				
				case(1): // second case left to right
				if (LED == 0){
					LED = 7;
				}else{
					LED--;
				}
				LEDS = ~(1 << LED);
				break;
				
				case(2): // third case back and forth
				if (LED == 7){
					mode2 = 0;
				}
				if (LED == 0){
					mode2 = 1;
				}
				if (mode2 == 1){
					LED++;
				}else{
					LED--;
				}
				LEDS = ~(1 << LED);
				break;
				
				case(3): // fourth case binary counting
				if (LEDS == 0x00){
					LEDS == 0xFF;
				}
				LEDS--;
				break;
				
		}
		}
		}
}
// Timer. 
ISR(TIMER0_OVF_vect){
	tick++;
	TCNT0 = TCNT0_COUNT_SET;	
}
// ISR INT0
ISR(INT0_vect){
	mode++;
	if (mode == 4){
		mode = 0;
	} else if (mode == 3){
		LEDS = 0xFF;
	}
}
