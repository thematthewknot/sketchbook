// Code writen by Matthew Varian www.3too1.com
// Free for distrubution and use 


byte RED = 4;
byte BLUE = 10;
byte GREEN = 9;
byte COLORS[] = {RED, BLUE, GREEN};
boolean on = true;
int i = 0;
int fade = 0;



int pin = 5;
volatile int state = HIGH;



#define BOUNCE_DURATION 50   // define an appropriate bounce time in ms for your switches
volatile unsigned long bounceTime=0; // variable to hold ms count to debounce a pressed switch



int a=0;
int b=900; //defines photocell trigger value



void setup() {
  pinMode(RED, OUTPUT); 
  pinMode(GREEN, OUTPUT); 
  pinMode(BLUE, OUTPUT); 
   pinMode(a, INPUT);  
   
  pinMode(pin, INTPUT);
  attachInterrupt(0, intHandler1, FALLING);
 
  } 

void loop() {
 
  digitalWrite(COLORS[i%3], on);
  i++;
  on = !on;
  delay(500);
 
 
 
 /*
  if(analogRead(a)>b)
  { 
    //send alt-f4 command
    Keyboard.set_modifier(MODIFIERKEY_ALT);
    Keyboard.send_now();
    Keyboard.set_key1(KEY_F4);
    Keyboard.send_now();
    Keyboard.set_modifier(0);
    Keyboard.set_key1(0);
    Keyboard.send_now();
    //wait 1sec
    delay(1000);
  }
  */
}


void blink()
{
  state = !state;
  
  int i;
  for (i=0; i<40; i++) {
    Mouse.move(2, -1);
    delay(25);
  }
  for (i=0; i<40; i++) {
    Mouse.move(2, 2);
    delay(25);
  }
  for (i=0; i<40; i++) {
    Mouse.move(-4, -1);
    delay(25);
  }
  
}

void intHandler1(){  
// this is the interrupt handler for button presses
// it ignores presses that occur in intervals less then the bounce time
 if(millis() > bounceTime)  
 {
   
   
   move1();
     // Your code here to handle new button press …
     bounceTime = millis() + BOUNCE_DURATION;  // set whatever bounce time in ms is appropriate
}
}


void move1()
{int i;
  for (i=0; i<40; i++) {
    Mouse.move(0, -2);
    delay(25);
  }
  for (i=0; i<40; i++) {
    Mouse.move(0, 2);
    delay(25);
  }
}
