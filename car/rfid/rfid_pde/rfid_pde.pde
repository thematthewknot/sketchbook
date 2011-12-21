#include <NewSoftSerial.h>
int  val = 0; 

char
code[]={'0','0','0','0','0','0','0','0','0','0', '\0'};
char tag1[]={'1','F','0','0','B','E','3','C','1','4','\0'};
char tag2[]={'1','F','0','0','D','0','6','2','C','1','\0'};
char tag3[]={'1','B','0','0','7','C','6','C','D','5','\0'};
int bytesread = 0, valid = 0, i = 0;
// Here's a rfid reader connect to pins 3(rx) and 4(tx)
NewSoftSerial rfid(3,7);

void setup()
{
  rfid.begin(2400);
  Serial.begin(2400); // RFID reader SOUT pin connected to Serial RX pin at 2400bps 
  pinMode(2,OUTPUT);   // Set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin
  pinMode(4,OUTPUT);  // Unlock button
  digitalWrite(2,LOW);                  // Activate the RFID reader
  digitalWrite(4,LOW);
  
}  

void loop()
{
 
	
	  
  {
    if((val =  rfid.read()) == 10)  // check for header 
    {
      bytesread = 0; 
  
      while(bytesread<10)  // read 10 digit code 
      {
        if( rfid.available() > 0)
        { 
          val = rfid.read(); 
          
         
          if((val == 10)||(val == 13))  // if header or stop bytes before the 10 digit reading 
          {
            break;                       // stop reading 
          } 
    
          code[bytesread] = val;         // add the digit           
    Serial.println(code[bytesread]);
          bytesread++;                   // ready to read next digit  
           
        } 
           
      }     
      if(bytesread == 10)  // if 10 digit read is complete
      {
          
        
        for(i = 0; i < 10; i++)
        {
        Serial.print("code[i]=");
        Serial.print(code[i]);
        Serial.print("tag1[i]=");
        Serial.print(tag1[i]);
        Serial.print("tag2[i]=");
        Serial.print(tag2[i]);
     Serial.print("tag3[i]=");
     Serial.println(tag3[i]);  
     
          valid = 1;
          if(!((code[i]==tag1[i])||(code[i]==tag2[i])||(code[i]==tag3[i])))
          {
            valid = 0;
            break;
          }
//          Serial.print("valid:");   

  //        Serial.print(valid);   

        }
        
        if(valid)  // If we have a match
        {
          digitalWrite(2,HIGH);  //Stop looking for tags
          
          digitalWrite(4,HIGH);  //Send pulse to unlock door
          delay(100);
         // digitalWrite(4,LOW);  //Stop pulse
          
          delay(700);  //wait
          digitalWrite(2,LOW);  //Continue looking for tags
        }
      }
    }
    while(rfid.available())  //If there is data left over, clear the buffer
      rfid.read();
  }
}

