/*
  The following is Irongeek's sample code, last tested on a Teensy 2.0 with an RGB LED
 
 Project Page:
 http://www.irongeek.com/i.php?page=security/programmable-hid-usb-keystroke-dongle 
 
 To learn more about Teensyduino see:
 http://www.pjrc.com/teensy/teensyduino.html
 Look in arduino-xxxx\hardware\teensy\cores\usb_hid\usb_api.h for key definitions
 Edit arduino-xxxx\hardware\teensy\cores\usb_hid\usb_private.h to change USB Vendor and Product ID
 
 Compile Notes: Make sure you set the correct board type under Tools->Board, and the correct Tools->USB type as well
 */
#include <phukdlib.h>

// Header Section
//You will want to change the pins below to match your board.

//End Header Section
int ledPin=11;
boolean ledPinState = true;
int firstrun=1;

// The setup() method runs once, when the sketch starts
void setup()   {             
  pinMode(ledPin, OUTPUT);    
  delay(5000); 
}

// the loop() method runs over and over again, checking for events
void loop()                     
{
  if (firstrun){
    CommandAtRunBarMSWIN("notepad.exe");
    delay(2000);
    firstrun=0;
  };
  Keyboard.println("Adrian was here!");
  digitalWrite(ledPin, ledPinState); //This line and the next are just there for blinking the LED so you know the code is running.
  ledPinState=!ledPinState;
  delay(1000);
}

