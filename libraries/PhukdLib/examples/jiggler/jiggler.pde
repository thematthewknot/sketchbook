/*
  The following is Irongeek's sample code, based on Paul Stoffregen's. Last tested on a Teensy 2.0.
 It has little to nothing to do with the PHUKDLib, but I wanted it in the pack for class to demo mouse functions.
 
 Project Page:
 http://www.irongeek.com/i.php?page=security/programmable-hid-usb-keystroke-dongle 
 
 To learn more about Teensyduino see:
 http://www.pjrc.com/teensy/teensyduino.html
 Look in arduino-xxxx\hardware\teensy\cores\usb_hid\usb_api.h for key definitions
 Edit arduino-xxxx\hardware\teensy\cores\usb_hid\usb_private.h to change USB Vendor and Product ID
 
 Compile Notes: Make sure you set the correct board type under Tools->Board, and the correct Tools->USB type as well
 */
// Header Section
//You will want to change the pins below to match your board.
int ledPin=11;
boolean ledPinState = true;
int firstrun=1;
//End Header Section

// The setup() method runs once, when the sketch starts
void setup()   {             
  pinMode(ledPin, OUTPUT);    
  delay(5000); 
}

void loop() {
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
  //Mouse.click();
  //Mouse.set_buttons(1, 0, 1);
  //Mouse.set_buttons(0, 0, 0);
  //Mouse.scroll(-3);

  digitalWrite(ledPin, ledPinState); //This line and the next are just there for blinking the LED so you know the code is running.
  ledPinState=!ledPinState;
  //delay(1000);
}

