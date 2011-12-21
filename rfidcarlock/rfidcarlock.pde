#include <NewSoftSerial.h>
int  val = 0; 
char code[]={'0','0','0','0','0','0','0','0','0','0', '\0'};
char tag1[]={'1','F','0','0','B','E','3','C','1','4','\0'};
char tag2[]={'1','F','0','0','D','0','6','2','C','1','\0'};
char tag3[]={'1','B','0','0','7','C','6','C','D','5','\0'};
int bytesread = 0, valid = 0, i = 0,valid2 = 0, action=0, count1=0;
// Here's a rfid reader connect to pins 3(rx) and 4(tx)
NewSoftSerial rfid(3,7);

void setup()
{
  rfid.begin(2400);
  pinMode(2,OUTPUT);   // Set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin
  pinMode(4,OUTPUT);  // Unlock button
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);

  digitalWrite(5,LOW);
  digitalWrite(2,LOW);                  // Activate the RFID reader
  digitalWrite(4,LOW);
  digitalWrite(6,HIGH); 
}  

void loop()
{
	sense();
	lockorunlock();
}
void lockorunlock()
{
	bytesread=0;
	sense();
	if(action!=0)
	{
		lockunlock();
	}
}
void sense()
{
	  
  
    if((val =  rfid.read()) == 10)  // check for header 
    {
      bytesread = 0; 

       readtag();

      if(bytesread == 10)  // if 10 digit read is complete
      {
        
        checktag();
       
        if(valid)  // If we have a match
        {	
			action++;
        }
                       
      }
    }
    while(rfid.available())  //If there is data left over, clear the buffer
    {
		rfid.read();
	}
}
void readtag()
{ 
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
          bytesread++;                   // ready to read next digit  
           
        } 
      }
}


void checktag()
{    
	  for(i = 0; i < 10; i++)
        {
          valid = 1;
          if(!((code[i]==tag1[i])||(code[i]==tag2[i])||(code[i]==tag3[i])))
          {
            valid = 0;
           action=0;
            break;
          
          }
  
        }
      
}

void lockunlock()
        {
          digitalWrite(2,HIGH);  //Stop looking for tags
          digitalWrite(4,HIGH);  //Send pulse to unlock door
          delay(1000);
          digitalWrite(4,LOW);  //Stop unlock pulse
          delay(20000);  //wait 25 sec
          digitalWrite(5,HIGH);  //Send pulse to lock door
          delay(1000);
          digitalWrite(5,LOW);  //Stop lock pulse
          digitalWrite(2,LOW);  //Continue looking for tags
		  action=0;
        }


