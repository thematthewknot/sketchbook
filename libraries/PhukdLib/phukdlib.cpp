/*
The following is Irongeek's PHUKD Library
 ver 0.3
 
 Project Page:
 http://www.irongeek.com/i.php?page=security/programmable-hid-usb-keystroke-dongle 
 
 To learn more about Teensyduino see:
 http://www.pjrc.com/teensy/teensyduino.html
 Look in arduino-xxxx\hardware\teensy\cores\usb_hid\usb_api.h for key definitions
 Edit arduino-xxxx\hardware\teensy\cores\usb_hid\usb_private.h to change USB Vendor and Product ID
 
 Compile Notes: Make sure you set the correct board type under Tools->Board, and the correct Tools->USB type as well
 */

#include "WProgram.h"
#include "phukdlib.h"


/********************************************************************
 * Opens the run bar and executes the command. 
 ********************************************************************/
void CommandAtRunBarMSWIN(char *SomeCommand){
  //digitalWrite(ledPin, HIGH);   // set the LED on
  Keyboard.set_modifier(MODIFIERKEY_RIGHT_GUI); //Windows key
  Keyboard.set_key1(KEY_R); // use r key
  Keyboard.send_now(); // send strokes
  Keyboard.set_modifier(0); //prep release of  control keys
  Keyboard.set_key1(0); //have to do this to keep it from hitting key multiple times.
  Keyboard.send_now(); //Send the key changes
  delay(1500);
  Keyboard.print(SomeCommand);
  Keyboard.set_key1(KEY_ENTER); 
  Keyboard.send_now();    
  Keyboard.set_key1(0); 
  Keyboard.send_now();  
}
/********************************************************************
 * Opens the run bar and executes the command. 
 ********************************************************************/
void CommandAtRunBarGnome(char *SomeCommand){
  //digitalWrite(ledPin, HIGH);   // set the LED on
  Keyboard.set_modifier(MODIFIERKEY_ALT); //Hold Alt key
  Keyboard.set_key1(KEY_F2); // use F2 key
  Keyboard.send_now(); // send strokes
  Keyboard.set_modifier(0); //prep release of  control keys
  Keyboard.set_key1(0); //have to do this to keep it from hitting key multiple times.
  Keyboard.send_now(); //Send the key changes
  delay(1500);
  Keyboard.print(SomeCommand);
  Keyboard.set_key1(KEY_ENTER); 
  Keyboard.send_now();    
  Keyboard.set_key1(0); 
  Keyboard.send_now();  
}

/********************************************************************
 * Shrinks the active window to help hide it.
 ********************************************************************/
void ShrinkCurWin(){
  Keyboard.set_modifier(MODIFIERKEY_ALT);
  Keyboard.set_key1(KEY_SPACE);
  Keyboard.send_now(); 
  delay(250);
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();  
  Keyboard.print("n");
}

void ShrinkCurWinMSWIN(){
  ShrinkCurWin();
}

void ShrinkCurWinGnome(){
  ShrinkCurWin();
}
/********************************************************************
 * This function simplifies the pressing and releasing of a key.  
 ********************************************************************/
void PressAndRelease(int KeyCode,int KeyCount){
  int KeyCounter=0;
  for (KeyCounter=0;  KeyCounter!=KeyCount; KeyCounter++){
    Keyboard.set_key1(KeyCode); // use r key
    Keyboard.send_now(); // send strokes
    Keyboard.set_key1(0); 
    Keyboard.send_now(); // send strokes
  }
}
/*********************************************************************
 * ledkeys returns the setting of the "lock keys"
 * Num Lock = 1
 * CAPS Lock = 2
 * Scroll Lock = 4
 * Add them together to get combos, for example if all three are on, 7 would be the result
 *********************************************************************/
int ledkeys(void)
{
  return int(keyboard_leds);
}
/*********************************************************************
 * Returns TRUE if NUM Lock LED is on and FALSE otherwise. 
 *********************************************************************/
boolean IsNumbOn(void)
{
  if ((ledkeys() & 1) == 1){
    return true;
  } 
  else {
    return false;
  }      
}
/*********************************************************************
 * Returns TRUE if Caps Lock LED is on and FALSE otherwise. 
 **********************************************************************/
boolean IsCapsOn(void)
{
  if ((ledkeys() & 2) == 2){
    return true;
  } 
  else {
    return false;
  }      
}
/*********************************************************************
 * Returns TRUE if Scroll Lock LED is on and FALSE otherwise. 
 **********************************************************************/
boolean IsScrlOn(void)
{
  if ((ledkeys() & 4) == 4){    
    return true;
  } 
  else {
    return false;
  }      
}
//********************************************************************


/********************************************************************
 * Pressed up(1)/down(0) arrow x number of times 
 ********************************************************************/
void UpDownXnumberOfTimes(int dirrection, int number)
{
  //digitalWrite(ledPin, HIGH);   // set the LED on
	
	if(dirrection==1)
	{
		for(i=0;i!=number;i++)
		{
			Keyboard.set_key1(KEY_UP); // use up arrow
			Keyboard.send_now(); // send strokes
			Keyboard.set_key1(0); //have to do this to keep it from hitting key multiple times.
			Keyboard.send_now(); //Send the key changes
			delay(100);
		} 
	}
	if(dirrection==0)
	{
		for(i=0;i!=number;i++)
		{
			Keyboard.set_key1(KEY_DOWN); // use up arrow
			Keyboard.send_now(); // send strokes
			Keyboard.set_key1(0); //have to do this to keep it from hitting key multiple times.
			Keyboard.send_now(); //Send the key changes
		}
	} 
}






