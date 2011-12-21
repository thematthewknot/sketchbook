
int dip1=0;
int dip2=1;
int dip3=2;
int dip4=3;


#define BOUNCE_DURATION 50   // define an appropriate bounce time in ms for your switches

volatile unsigned long bounceTime=0; // variable to hold ms count to debounce a pressed switch

int In1=5;
int In2=7;



void setup() 
{ 
  pinMode(dip1,INPUT_PULLUP);
 pinMode(dip2,INPUT_PULLUP);
  pinMode(dip3,INPUT_PULLUP);
 pinMode(dip4,INPUT_PULLUP);
 pinMode(In1,INPUT_PULLUP);
 pinMode(In2,INPUT_PULLUP);
 
 // attachInterrupt(In1, intHandler1, RISING);
  
 // attachInterrupt(In2, intHandler2, RISING);


} 

void loop() {
  if(!digitalRead(In1))
  { 
    intHandler1();
  }
   if(!digitalRead(In2))
  { 
    intHandler2();
  }
 if(!digitalRead(dip1))
  {
  int i;
  for (i=0; i<40; i++) {
    Mouse.move(2, 0);
    delay(25);
  }
  for (i=0; i<40; i++) {
    Mouse.move(-2, 0);
    delay(25);
  }

  }
    
 if(!digitalRead(dip2))
  {
  int i;
  for (i=0; i<40; i++) {
    Mouse.move(0, -2);
    delay(25);
  }
  for (i=0; i<40; i++) {
    Mouse.move(0, 2);
    delay(25);
  }
 
  }
  
    
 if(!digitalRead(dip3))
  {
  int i;
  for (i=0; i<40; i++) {
    Mouse.move(4, 0);
    delay(25);
  }
  for (i=0; i<40; i++) {
    Mouse.move(-4, 0);
    delay(25);
  }

  }
  
    
 if(!digitalRead(dip4))
  {
  int i;
  for (i=0; i<40; i++) {
    Mouse.move(0, -4);
    delay(25);
  }
  for (i=0; i<40; i++) {
    Mouse.move(0, 4);
    delay(25);
  }

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





void intHandler2(){  
// this is the interrupt handler for button presses
// it ignores presses that occur in intervals less then the bounce time
 if(millis() > bounceTime)  
 {
    
   move2();
   
   
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

void move2()
{int i;
  for (i=0; i<40; i++) {
    Mouse.move(2, 0);
    delay(25);
  }
  for (i=0; i<40; i++) {
    Mouse.move(-2, 0);
    delay(25);
  }
}

