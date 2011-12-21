// Code writen by Matthew Varian www.3too1.com
// Free for distrubution and use 


int easybutton1=20;
int easybutton2=21;

int encoderbutton=5;

 int encoder0PinA = 6;
 int encoder0PinB = 7;
 
byte RED = 4;
byte BLUE = 10;
byte GREEN = 9;
byte COLORS[] = {RED, BLUE, GREEN};
boolean on = true;
int i = 0;
int fade = 0;




 int val; 
 int encoder0Pos = 0;
 int encoder0PinALast = LOW;
 int n = LOW;


void setup() {
	
  pinMode(easybutton1,INPUT);
  pinMode(easybutton2,OUTPUT);
  digitalWrite(easybutton2, HIGH);
  pinMode(encoderbutton,INPUT);
    digitalWrite(encoderbutton, HIGH);
    
    
  pinMode(RED, OUTPUT); 
  pinMode(GREEN, OUTPUT); 
  pinMode(BLUE, OUTPUT); 
  
  



  attachInterrupt(0, blink, FALLING);
  
  
     Serial.begin (9600);
  
 
  } 

void loop() {
  
  /*
   *   digitalWrite(COLORS[i%3], on);
  i++;
  on = !on;
  delay(500);
  */
 
   n = digitalRead(encoder0PinA);
   if ((encoder0PinALast == LOW) && (n == HIGH)) {
     if (digitalRead(encoder0PinB) == LOW) {
       encoder0Pos--;
     } else {
       encoder0Pos++;
     }
     Serial.print (encoder0Pos);
         Serial.print ("/");
          Serial.print (easybutton1);
              Serial.print ("/");
               Serial.print (encoderbutton);
     Serial.print ("/");
   } 
   encoder0PinALast = n;
   
  
  

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
