
/*
 This is my program for the small device thing
 */
 
/* struct DIP {
  int DIP_1;
  int DIP_2 ;
  int DIP_3 ;
  int DIP_4; 
};
 
 */
 
int DIP_1 = 0;
int DIP_2 = 1;
int DIP_3 = 2;
int DIP_4 = 3;

int Button_1=5;
int Button_2=7;
int IntSel=0;

#define BOUNCE_DURATION 50   // define an appropriate bounce time in ms for your switches
volatile unsigned long bounceTime=0; // variable to hold ms count to debounce a pressed switch


	//DIP Pos={0,0,0,0};


void setup() 
{ 
	
	
 pinMode(DIP_1,INPUT_PULLUP);
 pinMode(DIP_2,INPUT_PULLUP);
 pinMode(DIP_3,INPUT_PULLUP);
 pinMode(DIP_4,INPUT_PULLUP);
 pinMode(Button_1,INPUT_PULLUP);
 pinMode(Button_2,INPUT_PULLUP);
 
 
 // attachInterrupt(Button_1, intHandler1, RISING);
  
 // attachInterrupt(Button_2, intHandler2, RISING);


} 

void loop() 
{
  int pos=0;
  

  
	if(!digitalRead(DIP_1))
	{
		pos=1000; 
	}
		
	if(!digitalRead(DIP_2))
	{
		
                pos=100+pos;
	}
		
	if(!digitalRead(DIP_3))
	{
		pos=10+pos;
	}
		
	if(!digitalRead(DIP_4))
	{
			pos=1+pos;

	}
	
	sort(pos);
	
	
	

  
  
}

void sort(int pos)
{
	switch(pos)
	{
		case 0000:
			function0();
			break;
		case 0001:
			function1();
			break;		
		case 0010:
			function2();
			break;
		case 0011:
			function3();
			break;	
		case 0100:
			function4();
			break;
		case 0101:
			function5();
			break;
		case 0110:
			function6();
			break;		
		case 0111:
			function7();
			break;
		case 1000:
			function8();
			break;	
		case 1001:
			function9();
			break;	
		case 1010:
			function10();
			break;
		case 1011:
			function11();
			break;		
		case 1100:
			function12();
			break;
		case 1101:
			function13();
			break;	
		case 1110:
			function14();
			break;
		case 1111:
			function15();
			break;

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




void ShowDiag(){
  Keyboard.println("Diagnostic Info");
  Keyboard.println("********************************************************************");
  for (int thispin=0; thispin <40;thispin++){
    if (!digitalRead(thispin)) {
      //digitalWrite(ledPin, HIGH);   // set the LED on
      Keyboard.print(thispin);
      Keyboard.println(" is toggled");
    } 
  }
  Keyboard.print("Analog pin 0 is: ");
  //Keyboard.println(PhotoRead);
  Keyboard.print("Keyboard LEDS ");
  //Keyboard.println(ledkeys());
  Keyboard.print("Num Lock: ");
 // Keyboard.println(int(IsNumbOn()));
  Keyboard.print("Caps Lock: ");
 // Keyboard.println(int(IsCapsOn()));
  Keyboard.print("Scroll Lock: ");
 // Keyboard.println(int(IsScrlOn()));
 // Keyboard.println(DIPOptions);
}
void function0()
{
}

void function1()
{
  if(!digitalRead(Button_1))
	move1();
  if(!digitalRead(Button_2))
	move2();
}

void function2()
{
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

void function11()
{
}

void function12()
{
}

void function13()
{
}

void function14()
{
	move2();
}

void function15()
{
}



//Old things!!
 /*
 if(!digitalRead(Pos.DIP_2))
  {
	int i;
	for (i=0; i<40; i++)
	{
		Mouse.move(0, -2);
		delay(25);
	}
	for (i=0; i<40; i++)
	{
		Mouse.move(0, 2);
		delay(25);
	}
 
  }
  
    
 if(!digitalRead(Pos.DIP_3))
  {
	int i;
	for (i=0; i<40; i++)
	{
		Mouse.move(4, 0);
		delay(25);
	}
	for (i=0; i<40; i++) 
	{
		Mouse.move(-4, 0);
		delay(25);
	}

  }
  
    
 if(!digitalRead(Pos.DIP_4))
  {
	int i;
	for (i=0; i<40; i++) 
	{
		Mouse.move(0, -4);
		delay(25);
	}
	for (i=0; i<40; i++) 
	{
		Mouse.move(0, 4);
		delay(25);
	}

  }
  */
