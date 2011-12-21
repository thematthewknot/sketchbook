// Code writen by Matthew Varian www.3too1.com
// Free for distrubution and use 


byte RED = 15;
byte BLUE = 12;
byte GREEN = 14;
byte COLORS[] = {RED, BLUE, GREEN};
boolean on = true;
int i = 0;
int fade = 0;
int max  = 5;
int min = 0;
int up =0;
int down=0;
int encodbutton=5;
#define encoder0PinA  6
#define encoder0PinB  7




#define BOUNCE_DURATION 100   // define an appropriate bounce time in ms for your switches
volatile unsigned long bounceTime=0; // variable to hold ms count to debounce a pressed switch

#define BOUNCE_DURATION2 50   // define an appropriate bounce time in ms for your switches
volatile unsigned long bounceTime2=0; // variable to hold ms count to debounce a pressed switch

volatile unsigned int encoder0Pos = 0;

int menue=min+1;
int EasyButton0 = 20;
int EasyButton1 = 21;

int buttonState = 0; 

void setup() {
  pinMode(RED, OUTPUT); 
  pinMode(GREEN, OUTPUT); 
  pinMode(BLUE, OUTPUT); 

   pinMode(encodbutton,INPUT_PULLUP);
  
 
  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor

  attachInterrupt(1, intHandler1, CHANGE); //encoder interupt
  attachInterrupt(0, intHandler2, RISING); //encoder button interupt
   
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



void ShowDiag(){
  Keyboard.println("SETUP Info");
  Keyboard.println("********************************************************************");
/*  for (int thispin=0; thispin <40;thispin++){
    if (!digitalRead(thispin)) {
      //digitalWrite(ledPin, HIGH);   // set the LED on
      Keyboard.print(thispin);
      Keyboard.println(" is toggled");
    } 
  }*/
  Keyboard.println("RED is: Pointless mouse movement ");
  //Keyboard.println(PhotoRead);
  Keyboard.println("Green is: Copy");
  //Keyboard.println(ledkeys());
  Keyboard.println("Blue is: Paste ");
 // Keyboard.println(int(IsNumbOn()));
  Keyboard.println("Yellow is: Alt-Tab ");
 // Keyboard.println(int(IsCapsOn()));
  Keyboard.println("Purple is: Windows key");
 // Keyboard.println(int(IsScrlOn()));
 // Keyboard.println(DIPOptions);*/
   Keyboard.println("BLINKING RED is:CTRL-ALT-L, Lock linux");
 
 
 Keyboard.set_modifier(0);
Keyboard.set_key1(0);
Keyboard.send_now();
  
 delay(1000);
}



void intHandler1(){  
	if(millis() > bounceTime)  
 {
   
   doEncoder();
     // Your code here to handle new button press …
     bounceTime = millis() + BOUNCE_DURATION;  // set whatever bounce time in ms is appropriate
}
}

void intHandler2(){  
	if(millis() > bounceTime)  
 {
   
  ShowDiag();
     // Your code here to handle new button press …
     bounceTime2 = millis() + BOUNCE_DURATION2;  // set whatever bounce time in ms is appropriate
}
}
void function0() //paste
{
		digitalWrite(BLUE, HIGH);
	if(!digitalRead(EasyButton0))
	{
	  
  // press and hold CTRL
Keyboard.set_modifier(MODIFIERKEY_CTRL);
Keyboard.send_now();


// press DELETE, while CLTR and ALT still held
Keyboard.set_key1(KEY_V);
Keyboard.send_now();

// release all the keys at the same instant
Keyboard.set_modifier(0);
Keyboard.set_key1(0);
Keyboard.send_now();
    delay(800);
  
  
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
		delay(15);
		}
		for (l=0; l<40; l++) 
		{
		Mouse.move(10, 0);
		delay(15);
		}
	}
digitalWrite(RED, LOW);
}

void function2() //copy
{	digitalWrite(GREEN, HIGH);
	if(!digitalRead(EasyButton0))
	{
		

  
  // press and hold CTRL
Keyboard.set_modifier(MODIFIERKEY_CTRL);
Keyboard.send_now();


// press DELETE, while CLTR and ALT still held
Keyboard.set_key1(KEY_C);
Keyboard.send_now();

// release all the keys at the same instant
Keyboard.set_modifier(0);
Keyboard.set_key1(0);
Keyboard.send_now();
  
  delay(800);
  
  
  
  
  
	}
digitalWrite(GREEN, LOW);
}

void function3()
{
  digitalWrite(RED,HIGH);
 digitalWrite(GREEN, HIGH);
	if(!digitalRead(EasyButton0))
	{
          // press and hold CTRL
          Keyboard.set_modifier(MODIFIERKEY_ALT);
          Keyboard.send_now();


          // press DELETE, while CLTR and ALT still held
          Keyboard.set_key1(KEY_TAB);
          Keyboard.send_now();
   Keyboard.set_key1(0);
          Keyboard.send_now();
  
          // release all the keys at the same instant
          Keyboard.set_modifier(0);
          Keyboard.set_key1(0);
          Keyboard.send_now();
  
          delay(500);
        }


 digitalWrite(RED,LOW);
 digitalWrite(GREEN, LOW);
}

void function4() // windows key
{  digitalWrite(RED,HIGH);
 digitalWrite(BLUE, HIGH);
	if(!digitalRead(EasyButton0))
	{
          // press and hold CTRL
          Keyboard.set_modifier(MODIFIERKEY_GUI);
          Keyboard.send_now();


          // press DELETE, while CLTR and ALT still held
        /*  Keyboard.set_key1(KEY_TAB);
          Keyboard.send_now();
   Keyboard.set_key1(0);
          Keyboard.send_now();
  */
          // release all the keys at the same instant
          Keyboard.set_modifier(0);
       //   Keyboard.set_key1(0);
          Keyboard.send_now();
  
          delay(500);
        }


 digitalWrite(RED,LOW);
 digitalWrite(BLUE, LOW);
}

void function5() // ctrl alt L, lock linux
{ digitalWrite(RED,HIGH);

	if(!digitalRead(EasyButton0))
	{
          // press and hold CTRL
     	  Keyboard.set_modifier(MODIFIERKEY_CTRL|MODIFIERKEY_ALT);
          // press DELETE, while CLTR and ALT still held
          Keyboard.set_key1(KEY_L);
          Keyboard.send_now();
	      Keyboard.set_key1(0);
          Keyboard.send_now();
  
        //release all the keys at the same instant
          Keyboard.set_modifier(0);
		//Keyboard.set_key1(0);
          Keyboard.send_now();
  
          delay(1500);
        }

 delay(150);
 digitalWrite(RED,LOW);
delay(150);
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
  /*
   *   digitalWrite(COLORS[i%3], on);
  i++;
  on = !on;
  delay(500);
  */
