// Code writen by Matthew Varian www.3too1.com
// Free for distrubution and use 


byte RED = 15;
byte BLUE = 12;
byte GREEN = 14;
byte COLORS[] = {RED, BLUE, GREEN};
boolean on = true;
int i = 0;
int fade = 0;
int max  = 2;
int min = 0;
int up =0;
int down=0;

#define encoder0PinA  6
#define encoder0PinB  7




#define BOUNCE_DURATION 100   // define an appropriate bounce time in ms for your switches
volatile unsigned long bounceTime=0; // variable to hold ms count to debounce a pressed switch


volatile unsigned int encoder0Pos = 0;

int menue=min+1;
int EasyButton0 = 20;
int EasyButton1 = 21;

int buttonState = 0; 

void setup() {
  pinMode(RED, OUTPUT); 
  pinMode(GREEN, OUTPUT); 
  pinMode(BLUE, OUTPUT); 


  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor

  attachInterrupt(1, intHandler1, CHANGE);
   
   pinMode(EasyButton0,INPUT_PULLUP);
   pinMode(EasyButton1,OUTPUT);
  digitalWrite(EasyButton1, LOW);

 
  } 

void loop() {
 
	 
	if(up==1)
	{
		 
		if (menue ==max)
		menue=min;
		else
		menue++;
			
		up=0;
		//Serial.println (menue, DEC);
	}
	if(down==1)
	{
			 
		if (menue ==min)
		menue=max;
		else
		menue--;
			
		down=0;
		//Serial.println (menue, DEC);
	}
		 
	 	switch(menue)
	{
		case 0:
			function0();
			break;
		case 1:
			function1();
			break;		
		case 2:
			function2();
			break;
		case 3:
			function3();
			break;	
		case 4:
			function4();
			break;
		case 5:
			function5();
			break;
		case 6:
			function6();
			break;		
		case 7:
			function7();
			break;
		case 8:
			function8();
			break;	
		case 9:
			function9();
			break;	
		case 10:
			function10();
			break;

	}

 
 
 
 
 
   buttonState = !digitalRead(EasyButton0);
/*
	if (buttonState == HIGH) 
	{       
		digitalWrite(RED, HIGH);
	}
	else
	{
		digitalWrite(RED, LOW);
	}
	
	if(encoder0Pos>2)
	{
			digitalWrite(BLUE, HIGH);
		digitalWrite(GREEN, LOW);
	}
	if(encoder0Pos<-2)
	{	digitalWrite(GREEN, HIGH);
		digitalWrite(BLUE, LOW);
	}
	*/
	//delay(1000);
}

void doEncoder() {

  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
  //  encoder0Pos++;
   up=1;
   
    
  } else {
	//  encoder0Pos--;
   down=1;
    
    
  }
  
  //Serial.println (encoder0Pos, DEC);
}




void intHandler1(){  
	if(millis() > bounceTime)  
 {
   
   doEncoder();
     // Your code here to handle new button press …
     bounceTime = millis() + BOUNCE_DURATION;  // set whatever bounce time in ms is appropriate
}
}


void function0()
{
		digitalWrite(BLUE, HIGH);
	if(!digitalRead(EasyButton0))
	{
		int l;
		for (l=0; l<40; l++)
		{
		Mouse.move(0, -2);
		delay(25);
		}
		for (l=0; l<40; l++) 
		{
		Mouse.move(0, 2);
		delay(25);
		}
	}
digitalWrite(BLUE, LOW);
}

void function1()
	{
	digitalWrite(RED, HIGH);
	if(!digitalRead(EasyButton0))
	{
		int l;
		for (l=0; l<40; l++)
		{
		Mouse.move(-10, 0);
		delay(25);
		}
		for (l=0; l<40; l++) 
		{
		Mouse.move(10, 0);
		delay(25);
		}
	}
digitalWrite(RED, LOW);
}

void function2()
{	digitalWrite(GREEN, HIGH);
	if(!digitalRead(EasyButton0))
	{
		int l;
		for (l=0; l<40; l++)
		{
		Mouse.move(20, -20);
		delay(25);
		}
		for (l=0; l<40; l++) 
		{
		Mouse.move(-20, 20);
		delay(25);
		}
	}
digitalWrite(GREEN, LOW);
}

void function3()
{
}

void function4()
{
}

void function5()
{
}

void function6()
{
}

void function7()
{
}

void function8()
{
}

void function9()
{
}

void function10()
{
}

