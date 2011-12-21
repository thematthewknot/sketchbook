/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>
int val = 0; 
IRsend irsend;
int but=7;
void setup()
{
  Serial.begin(9600);
  pinMode(but,INPUT);
}

void loop() {
  val = digitalRead(but);
if(val==HIGH)
{
    for (int i = 0; i < 3; i++) {
      irsend.sendNEC(0xFFB04F, 32); // Sony TV power code
      delay(100);
    }
}
}

