/*
 * uln2003.c
 *
 * Created: 16/12/2016 13:25:52
 * Author : Simone Dassi
 */ 

#define F_CPU 1000000

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define DELAY 3		//lower value = higher motor speed

uint8_t lookup[] = {0b10000, 0b11000, 0b01000, 0b01100, 0b00100, 0b00110, 0b00010, 0b10010}, i; //pin states packed in array because making 8 nested if-else is just awful.
uint16_t t;
volatile uint16_t cont=0; //volatile: just making sure the compiler doesn't optimize this away

ISR(TIMER0_OVF_vect){
	cont++;
}

void spin()
{
	for(t = 0; t < 25000; t++) //25000 = 10 min / (DELAY * 8) = number of cycles
		for(i = 0; i < 8; i++){		//single full-step cycle
			PORTB = lookup[i];	//here's where the magic happens :)
			_delay_ms(DELAY);
		}
	PORTB = 0;
}

int main(void)
{

	ACSR = ADMUX = ADCSRA = 0;		// turn off things we don't need
	ACSR = 0b10000000;
	set_sleep_mode(SLEEP_MODE_IDLE);

	DDRB = 0b11110;

	TIMSK |= 1<<TOIE0;	//overflow interrupt
	sei();
	
    while (1)
    {
		TCCR0B = 0;		//timer0 stopped
		PRR = 0b00001111;	//disable timer0
		spin();			//you spin me right round baby right round
		PRR = 0b00001011;	//enable timer0
		TCCR0B |= 1<<CS02 | 1<<CS00; //1024 prescaler
		
		cont = 0;
		
		do{
			sleep_mode();	//thy time has not come yet, go back to sleep
		}while(cont < 11444);	//11444: number of timer0 overflows @1024 prescaling to wait 50 minutes
			
    }
	
	return 0;	//never actually hit
}

