/*
 * uln2003.c
 *
 * Created: 16/12/2016 13:25:52
 * Author : Simone
 */ 

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define DELAY 3
#define	WAIT_DELAY 250
#define F_CPU 1000000

uint8_t lookup[] = {0b10000, 0b11000, 0b01000, 0b01100, 0b00100, 0b00110, 0b00010, 0b10010}, i;
uint16_t t;
volatile uint16_t cont=0;

ISR(TIMER0_OVF_vect){
	cont++;
}

void spin()
{
	for(t = 0; t < 25000; t++) // 10 min / (delay * 8) = numero cicli full step per arrivare a 10 min
		for(i = 0; i < 8; i++){
			PORTB = lookup[i];
			_delay_ms(DELAY);
		}
	PORTB = 0;
}

int main(void)
{
    /* Replace with your application code */	
	
	ACSR = ADMUX = ADCSRA = 0;
	ACSR = 0b10000000;
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	DDRB = 0b11110;
	
	TIMSK |= 1<<TOIE0;	//overflow interrupt
	sei();
	
    while (1)
    {
		TCCR0B = 0;
		PRR = 0b00001111;
		spin();
		PRR = PRR = 0b00001011;
		TCCR0B |= 1<<CS02 | 1<<CS00; //1024 prescaler
		
		cont = 0;
		
		do{
			sleep_mode();	
		}while(cont < 11444);	//not yet, go back to sleep
		
		/*for(t = 0; t < 12000; t++) // 50 min / (4 * 0.250) = numero di delay da 1/4 di secondo per 50 min
			_delay_ms(WAIT_DELAY);*/
			
    }
	
	return 0;
}

