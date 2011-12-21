#include <SPI.h>  
#include <Ethernet.h>



byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x25, 0x83 };
byte ip[] = { 192,168,2,75 };
byte gateway[] = { 167, 205, 2, 1 };                // Gateway
byte subnet[]  = { 255, 255, 255, 0 };

Server server(80);
int Led2 = 8;
int Led1 = 9;  //  led output
//boolean  Led1ON, Toggle1ON = false; 


String readString = String(100);      // string for fetching data from address

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);  
  //server.begin();
  //Serial.begin(9600);
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
			//digitalWrite( Led2, HIGH); //client connected status led
			if (client.available()) 
			{
				char c = client.read();

//read char by char HTTP request
				if (readString.length() < 100) 
				{

//store characters to string

					readString = readString + c; 
				}

			//	Serial.print(c);  //output chars to serial port

				if (c == '\n') 
				{  //if HTTP request has ended

 //Serial.println(readString);
							if(readString.indexOf("1=off") > -1) 
							{
					
								digitalWrite( Led1, LOW);
						
							}
							if(readString.indexOf("2=on") > -1) 
							{
								
								digitalWrite( Led1, HIGH);
						
							}	


//--------------------------HTML------------------------

//client.println("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");


client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println();

client.print("<html><head>");

client.print("<title>test1234test</title>");
client.println("<meta http-equiv=\"refresh\" content=\"2\">");

client.println("</head>");

client.print("<body bgcolor='white'>");
//client.print("<div style=\"margin: 0 auto\">")
if(digitalRead(Led1)==1)
{
	client.print("LED is ON<br>");
}
else
{
	client.print("LED is OFF<br>");

}
client.println("<form method=get><input type=submit name=1 value='off'><input type=submit name=2 value='on'></form>");

// client.println("<form method=get><input type=submit name=2 value='on'></form>");
//client.println("</div>");
client.println("</body></html>");

//clearing string for next read
readString="";

//stopping client
delay(1);
client.stop();
}
}
} 




	


}



//digitalWrite( Led2, LOW); // if no client connnected

}
