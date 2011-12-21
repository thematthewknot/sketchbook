/*
The following is Irongeek's PHUKD Library
 ver 0.3
 
 To use, copy this file (phukdlib.h) and phukdlib.cpp into the <arduino folder>\libraries\PhukdLib\, then add this include line to your sketch:
 
 #include <phukdlib.h>
 
 Keep in mind, I've only been implementing for Windows so far. Linux and OS X will take other keystrokes to accomplish the same effects.
 
 Project Page:
 http://www.irongeek.com/i.php?page=security/programmable-hid-usb-keystroke-dongle 
 
 To learn more about Teensyduino see:
 http://www.pjrc.com/teensy/teensyduino.html
 Look in arduino-xxxx\hardware\teensy\cores\usb_hid\usb_api.h for key definitions
 Edit arduino-xxxx\hardware\teensy\cores\usb_hid\usb_private.h to change USB Vendor and Product ID
 */

#ifndef Phukd_h
#define Phukd_h
#include "WProgram.h"

extern void CommandAtRunBarMSWIN(char *SomeCommand); //Opens the run bar and executes the command. 
extern void CommandAtRunBarGnome(char *SomeCommand);
extern void CommandAtRunBarOSX(char *SomeCommand);
extern void ShrinkCurWin();
extern void ShrinkCurWinMSWIN();
extern void ShrinkCurWinGnome();
extern void PressAndRelease(int KeyCode,int KeyCount);
extern void ShowDiag();
extern volatile uint8_t keyboard_leds;
extern int ledkeys();
extern boolean IsNumbOn(void);
extern boolean IsCapsOn(void);
extern boolean IsScrlOn(void);
void UpDownXnumberOfTimes(int dirrection, int number)
#endif




