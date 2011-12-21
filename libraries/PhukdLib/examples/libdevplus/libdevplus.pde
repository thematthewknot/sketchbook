/*
  The following is Irongeek's sample code, last tested on a Teensy++ 2.0 but should work fine on a Teensy 2.0
 with changes to pins assignments.
 
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
int thispin;
int PhotoRead = analogRead(0);
int OldPhotoRead = PhotoRead;
int ledPin =  6;   
boolean ledPinState = false;
boolean CapsLockTrapSet = false;
int DIP_1 = 8;
int DIP_2 = 9;
int DIP_3 = 10;
int DIP_4 = 11;
int DIP_5 = 12;
int DIP_6 = 13;
int DIP_7 = 14;
int DIP_8 = 15;
int DIP_9 = 16;

char *DIPOptions=
"DIP 1 Show Diag \n\r" 
"Dip 2 Type Adrian Was Here in Notepad \n\r"
"Dip 3 Go to Irongeek.com \n\r"
"Dip 4 Set Caps lock trap \n\r"
"Dip 5 Try to make a facebook post \n\r"
"Dip 6 Finds a drive called MYTHUMB and runs a script called myscript.bat from it \n\r"
"Dip 7 Brightness Detection \n\r"
"Dip 8 Motion Detection \n\r"
;

//End Header Section

// The setup() method runs once, when the sketch starts
void setup()   {                
  // initialize the digital pin as an output:
  for (int thispin=0; thispin <=40;thispin++){
    pinMode(thispin, INPUT_PULLUP); // Dip
  }
  pinMode(ledPin, OUTPUT);     
}


// the loop() method runs over and over again, checking for events
void loop()                     
{
  //Please note: I use negative logic here, when a pin goes to ground the code us run.
  if (!digitalRead(0)) { /*I only use this so I can control the Phukd when I do a demo, other times you would not want to have to wait for this sort of inten
   ional interaction. */

    if (!digitalRead(DIP_1)) {
      ShowDiag();
    }  
    if (!digitalRead(DIP_2)) {
      CommandAtRunBarMSWIN("notepad.exe");
      delay(2000);
      Keyboard.print("Adrian was here!");
    }

    if (!digitalRead(DIP_3)) {
      CommandAtRunBarMSWIN("cmd /c start http://irongeek.com");
    }  

    //Thanks to KennyG for this idea. All we do is turn on Caps lock, and when we see it go off we know whe can do some stuff :)
    if (!digitalRead(DIP_4)) {
      if (!CapsLockTrapSet && !IsCapsOn()){
        PressAndRelease(KEY_CAPS_LOCK,1);
        delay(500);
        CapsLockTrapSet = true;
      }
    }  
    //Let's try to make a facebook post if the peson chose to stay logged in
    if (!digitalRead(DIP_5)) {
      CommandAtRunBarMSWIN("cmd /c start http://m.facebook.com");
      delay(6000);
      PressAndRelease(KEY_TAB, 8);
      Keyboard.print("Test from Phukd device, more info at http://www.irongeek.com/i.php?page=security/programmable-hid-usb-keystroke-dongle");
      PressAndRelease(KEY_TAB, 1);
      PressAndRelease(KEY_ENTER, 1);
    }

    /* This section sends a command to the run bar, finds the drive letter by its volume name (MYTHUMB in
     this example, and case sensitive), then runs your script. Thanks to Tim Medin for this more
     elegant command line than what I had for finding the thumbdrive by volume name.
     */
    if (!digitalRead(DIP_6)) {
      CommandAtRunBarMSWIN("cmd /c for /F %i in ('WMIC logicaldisk where \"DriveType=2\" list brief ^| find \"MYTHUMB\"') do %i\\myscript.bat");
      delay(2000);
      ShrinkCurWinMSWIN();
    } 

    if (!digitalRead(DIP_7)) {
      CommandAtRunBarMSWIN("notepad.exe");
      delay(1000);
      if ( PhotoRead < 400 ) {
        Keyboard.print("I'm scared of the dark");
      }
      if ( PhotoRead >= 400 && PhotoRead <= 900 ) {
        Keyboard.print("The lights are on it seems");
      }
      if (PhotoRead > 900) {
        Keyboard.print("What the hell?!?!?! Is that a laser or a nuke flash?");
      }
    }
  }

  //The code below depends less on input 0 going low, and more on events.
  PhotoRead = analogRead(0);
  if (!digitalRead(DIP_8)) {
    if (abs(PhotoRead - OldPhotoRead) > 100 ) {
      CommandAtRunBarMSWIN("notepad.exe");
      delay(1000);
      Keyboard.println("Motion Detected!!!");
      Keyboard.print("PhotoRead ");
      Keyboard.println(PhotoRead);
      Keyboard.print("OldPhotoRead ");
      Keyboard.println(OldPhotoRead);
      OldPhotoRead = analogRead(0);
    }
  } 

  if (CapsLockTrapSet && !IsCapsOn()) {
    CommandAtRunBarMSWIN("notepad.exe");
    delay(2000);
    Keyboard.print("Someone hit Caps lock, so I know someone is there at the keyboard. Num lock can also be helpful as it sometimes changes states on login.");
    CapsLockTrapSet = false;
  }
  digitalWrite(ledPin, ledPinState); //This line and the next are just there for blinking the LED so you know the code is running.
  ledPinState=!ledPinState;
  //delay(1000);
}

/********************************************************************
 * Just sends diagnostic info out the keyboard interface.
 ********************************************************************/
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
  Keyboard.println(PhotoRead);
  Keyboard.print("Keyboard LEDS ");
  Keyboard.println(ledkeys());
  Keyboard.print("Num Lock: ");
  Keyboard.println(int(IsNumbOn()));
  Keyboard.print("Caps Lock: ");
  Keyboard.println(int(IsCapsOn()));
  Keyboard.print("Scroll Lock: ");
  Keyboard.println(int(IsScrlOn()));
  Keyboard.println(DIPOptions);
}

