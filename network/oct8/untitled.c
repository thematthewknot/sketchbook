/*
 * This is a web server that allows you to host a fully functional html
 *  webpage on the World Wide Web. 
 * Initial coding was done with the help from the many people in the Arduino community. 
 *  Thanks guys!!
 *
 * Arduino Setup: You need an Ethernet Shield SD, and and Arduino. 
 *  Optionally pins 8 and 9 can be used for two LEDs that can indicate traffic.
 *
 * SD Card Setup: On the SD card, which must be formated as fat, you must have 
 *  an HTML file with the file name of 'index.htm'. All file names on this card 
 *  must be written in the old 8.3 file format. In other words, all files must be 
 *  named with no more then 8 characters for the name, and 3 for the extension.
 *
 * Note: If you don't know HTML, a good place that I found useful was w3schools
 *  http://www.w3schools.com/html/html_examples.asp 
 *
 */


#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <Ethernet.h>
#define BUFSIZ 128
#define greenLEDandBEEP 2
#define redLEDpin 3
boolean led2 = true;
int hits = 0; // Set the number of hits the hit counter will start at.
int units = 0;
int count = 0;
int photocellPin = 2;
int photocellReading;


//Local ethernet setup
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x25, 0x83 };
byte ip[] = { 192,168,2,75 };
char rootFileName[] = "old.htm";
Server server(80);

//SD card stuff
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;
#define error(s) error_P(PSTR(s))
void error_P(const char* str) {
  PgmPrint("error: ");
  SerialPrintln_P(str);
  if (card.errorCode()) {
    PgmPrint("SD error: ");
    Serial.print(card.errorCode(), HEX);
    Serial.print(',');
    Serial.println(card.errorData(), HEX);
  }
  while(1);
}

void setup() {


  Serial.begin(9600);

  pinMode(greenLEDandBEEP, OUTPUT);
  pinMode(redLEDpin, OUTPUT);

  PgmPrint("Free RAM: ");
  Serial.println(FreeRam());  

  pinMode(10, OUTPUT);              
  digitalWrite(10, HIGH);              

  if (!card.init(SPI_HALF_SPEED, 4)) error("card.init failed!");

  if (!volume.init(&card)) error("vol.init failed!");

  PgmPrint("Volume is FAT");
  Serial.println(volume.fatType(),DEC);
  Serial.println();

  if (!root.openRoot(&volume)) error("openRoot failed");

  PgmPrintln("Files found in root:");
  root.ls(LS_DATE | LS_SIZE);
  Serial.println();

  PgmPrintln("Files found in all dirs:");
  root.ls(LS_R);

  Serial.println();
  PgmPrintln("Done");

  Ethernet.begin(mac, ip);
  server.begin();
}


void loop()
{
  if(count >=500)
  {  
    led2 = !led2;
    digitalWrite(redLEDpin, led2);
    count = 0;
  }
  count +=1;

  char clientline[BUFSIZ];
  char *filename;
  int image = 0;
  int index = 0;

  Client client = server.available();
  if (client) {
    boolean current_line_is_blank = true;

    index = 0;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c != '\n' && c != '\r') {
          clientline[index] = c;
          index++;
          if (index >= BUFSIZ) 
            index = BUFSIZ -1;

          continue;
        }

        clientline[index] = 0;
        filename = 0;

        Serial.println(clientline);
       
        if (strstr(clientline, "GET /?") != 0) 
        {  
			if (!filename) filename = clientline + 5; 
			{
			Serial.println(filename);
			
                     
			int a=atoi(&filename[1]);
			Serial.println(a);
			
		int pin_num=0;
		int on_off=0;
		
			switch (a)
			{
				case 10
				pin_num=1;
				on_off=LOW;
				break;
				case 11
				pin_num=1;
				on_off=HIGH;
				break;
				
				case 20
				pin_num=2;
				on_off=LOW;
				break;
				case 21
				pin_num=2;
				on_off=HIGH;
				break;
				
				case 30
				pin_num=3;
				on_off=LOW;
				break;
				case 31
				pin_num=3;
				on_off=HIGH;
				break;
				
				case 40
				pin_num=4;
				on_off=LOW;
				break;
				case 41
				pin_num=4;
				on_off=HIGH;
				break;
				
				case 50
				pin_num=5;
				on_off=LOW;
				break;
				case 51
				pin_num=5;
				on_off=HIGH;
				break;
				
				case 60
				pin_num=6;
				on_off=LOW;
				break;
				case 61
				pin_num=6;
				on_off=HIGH;
				break;
				
				case 70
				pin_num=7;
				on_off=LOW;
				break;
				case 71
				pin_num=7;
				on_off=HIGH;
				break;
				
				case 80
				pin_num=8;
				on_off=LOW;
				break;
				case 81
				pin_num=8;
				on_off=HIGH;
				break;
				
			}
		
		if(pin_num!=0)
		{
			Serial.print("Pin:");
			Serial.print(pin_num);
			Serial.print(" State:");
			Serial.print(on_off);
			digitalWrite(pin_num, on_off);
		}
		
		
		
			
			
			}

        } 
        if (strstr(clientline, "GET / ") != 0) {
          filename = rootFileName;

        } 
        if (strstr(clientline, "GET /") != 0) {
          if (!filename) filename = clientline + 5; 

          (strstr(clientline, " HTTP"))[0] = 0;

          Serial.println(filename);

          if (! file.open(&root, filename, O_READ)) {
            client.println("HTTP/1.1 4041 Not Found");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<h2>Error 4041</h2>");
            client.println("<s2>The file does not exist.<s2>");
            client.println("");
            break;
          }

          Serial.println("Opened!");
          //File types
          client.println("HTTP/1.1 200 OK");
          if (strstr(filename, ".htm") != 0)
            client.println("Content-Type: text/html");
          else if (strstr(filename, ".css") != 0)
            client.println("Content-Type: text/css");
          else if (strstr(filename, ".png") != 0)
            client.println("Content-Type: image/png");
          else if (strstr(filename, ".jpg") != 0)
            client.println("Content-Type: image/jpeg");
          else if (strstr(filename, ".gif") != 0)
            client.println("Content-Type: image/gif");
          else if (strstr(filename, ".3gp") != 0)
            client.println("Content-Type: video/mpeg");
          else if (strstr(filename, ".pdf") != 0)
            client.println("Content-Type: application/pdf");
          else if (strstr(filename, ".js") != 0)
            client.println("Content-Type: application/x-javascript");
          else if (strstr(filename, ".xml") != 0)
            client.println("Content-Type: application/xml");
          else
            client.println("Content-Type: text");
          client.println();

          int16_t c;
          while ((c = file.read()) >= 0) {
            //Serial.print((char)c); //Prints all HTML code to serial (For debuging)
            client.print((char)c); //Prints all HTML code for web page
          }

          //Hit counter math
          if(units >= 2)
          {
            hits ++;
            units = 0;
          }
          units +=1;
          //End hit counter math


          client.print("<html><body>"); //HTML code starts here
          client.print("<P align=\"center\">"); 
          client.print("Hits since reset: <b>");   
          client.print(hits); //Print hits to client
          client.print("</b><br>");

          photocellReading = analogRead(photocellPin); 
          client.print("Light reading: "); 
          client.print(photocellReading); //Prints light reading to client

          // A few threshholds
          if (photocellReading < 10) {
            client.print(" - Dark");
          } 
          else if (photocellReading < 200) {
            client.print(" - Dim");
          } 
          else if (photocellReading < 500) {
            client.print(" - Light");
          } 
          else if (photocellReading < 800) {
            client.print(" - Bright");
          } 
          else {
            client.print(" - Very bright");
          }

          client.print("</p></body></html>"); //HTML code ends here
          //End hit counter and light value

          file.close();

        } 
        else {
          client.println("HTTP/1.1 4042 Not Found");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<h2>Error 4042</h2>");
          client.println("");
        }
        break;
      }
    }
    digitalWrite(greenLEDandBEEP, HIGH);
    delay(1);
    digitalWrite(greenLEDandBEEP, LOW);
    client.stop();
  }

}

//The End /* 
