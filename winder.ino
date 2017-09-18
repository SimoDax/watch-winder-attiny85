#define DELAY 3    //lower value = higher motor speed

uint8_t lookup[] = {0b10000, 0b11000, 0b01000, 0b01100, 0b00100, 0b00110, 0b00010, 0b10010}, i; //pin states packed in array because making 8 nested if-else is just awful.
uint16_t t;
volatile uint16_t cont=0; //volatile: just making sure the compiler doesn't optimize this away

void spin()
{
  for(t = 0; t < 600000 / (DELAY * 8); t++)   //25000 = 10 min / (DELAY * 8) = number of cycles
    for(i = 0; i < 8; i++){                   //single full-step cycle
      PORTB = lookup[i];                      //here's where the magic happens :)
      delay(DELAY);
    }
  PORTB = 0;
}

void setup() {
  // put your setup code here, to run once:
  
  ACSR = ADMUX = ADCSRA = 0;    // turn off things we don't need
  ACSR = 0b10000000;

  DDRB = 0b11110;
}

void loop() {
  // put your main code here, to run repeatedly:
  
    spin();     //you spin me right round baby right round

    cont = 0;
    
    do{
      delay(1000);
      cont++;
    }while(cont < 3000); //3000: seconds in 50 minutes
    
}
