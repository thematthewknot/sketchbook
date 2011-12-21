#include <SPI.h>  
#include <Ethernet.h>
#include <WString.h>


byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x25, 0x83 };
byte ip[] = { 192,168,2,75 };
byte gateway[] = { 167, 205, 2, 1 };                // Gateway
byte subnet[]  = { 255, 255, 255, 0 };

Server server(80);
int Led2 = 8;
int Led1 = 9;  //  led output

boolean currentLineIsBlank = true;


String readString = String(100);      // string for fetching data from address
char c;
void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);  
  server.begin();
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
}

void loop()
{

// Create a client connection
  Client client = server.available();

	if (client) 
	{
		while (client.connected()) 
		{		
			
			if (client.available()) 
			{
			     c = client.read();//read char by char HTTP request
			     
				if (readString.length() < 100) 
				{
					readString = readString + c; //store characters to string
				}

				if (c == '\n' && currentLineIsBlank) //if HTTP request has ended
				{  

							if(readString.indexOf("1=off") > -1) 
							{
					
								digitalWrite( Led1, LOW);
						
							}
							if(readString.indexOf("2=on") > -1) 
							{
								
								digitalWrite( Led1, HIGH);
						
							}	

					html();

					
					 break;				
					 
					 	//clearing string for next read
					//readString="";

					//stopping client
					//delay(1);
					//client.stop();
				}
				if (c == '\n') 
				{
				  // you're starting a new line
				  currentLineIsBlank = true;
				} 
				else if (c != '\r') 
				{
				  // you've gotten a character on the current line
				  currentLineIsBlank = false;
				}
			}
			
		} 
		
		//stopping client
		readString="";
		delay(1000);	
		client.flush(); 
		client.stop();
	}

}

void html()
{
	
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	client.println();
	client.print("<html><head>");
	client.print("<title>test1234test</title>");
	//client.println("<meta http-equiv=\"refresh\" content=\"2\">");
	client.println("</head>");
	client.print("<body bgcolor='white'>");
	if(digitalRead(Led1)==1)
	{
		client.print("LED is ON<br>");
	}
	else
	{
		client.print("LED is OFF<br>");
	}
	client.println("<form method=get><input type=submit name=1 value='off'><input type=submit name=2 value='on'></form>");
	client.println("</body></html>");
} 
