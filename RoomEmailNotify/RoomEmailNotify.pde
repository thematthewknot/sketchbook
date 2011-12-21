

#include <IRremote.h>

IRsend irsend;
int mail = LOW; // Is there new mail?
int val; // Value read from the serial port
int settog=LOW;
int settog2=HIGH;
void setup()
{
   Serial.begin(9600);
    Serial.flush();
    
}

void loop() {
	
	if (Serial.available())
    {
        val = Serial.read();
        Serial.println(val);
        if (val == 'M' && settog==LOW)
        {
			mail = HIGH;
	 settog=HIGH;
 
		}
        else if (val == 'N' && settog==HIGH) 
        {
			mail = LOW;
			 settog=LOW;
 
		}
    }
    
	if(settog!=settog2)
	  blink(mail);
	  
  settog2=settog;

}
int blink(int a)
{
  if (a==HIGH)
  {
	  turnon();
	  Blue();

  }
 else
  {
    turnoff();
  
  }
}




void turnon()
{
irsend.sendNEC(0xFFB04F,32);
delay(100);
}
void Blue()
{
irsend.sendNEC(0xFF8877,32);
delay(1000);
}
void turnoff()
{
irsend.sendNEC(0xFFF807,32);
delay(1000);
}
void Red()
{
irsend.sendNEC(0xFF9867,32);
delay(1000);
}
void Green()
{
irsend.sendNEC(0xFFD827,32);
delay(1000);
}
void Flash()
{
irsend.sendNEC(0xFFB24D,32);
delay(1000);
}
void Dim()
{
irsend.sendNEC(0xFFB847,32);
delay(1000);
}
void Brighten()
{
irsend.sendNEC(0xFF906F,32);
delay(1000);
}
void White()
{
irsend.sendNEC(0xFFA857,32);
delay(1000);
}
