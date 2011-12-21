// Code writen by Matthew Varian www.3too1.com
// Free for distrubution and use 

int a=0;
int b=900; //defines photocell trigger value
void setup() {
   pinMode(a, INPUT);  
  } // no setup needed

void loop() {
  
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
  
}

