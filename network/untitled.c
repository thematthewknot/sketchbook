#include <SPI.h>  
#include <Ethernet.h>



byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x25, 0x83 };
byte ip[] = { 192,168,2,75 };
byte gateway[] = { 167, 205, 2, 1 };                // Gateway
byte subnet[]  = { 255, 255, 255, 0 };

Server server(80);

int Led1 = 9;  //  led output
boolean  Led1ON, Toggle1ON = false; 


String readString = String(100);      // string for fetching data from address

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);  
  server.begin();
  Serial.begin(9600);
  pinMode(Led1, OUTPUT);
}

void loop()
{
 // int LedState1 = digitalRead(Led1); // reading led status
  
  //if(LedState1 == HIGH)
//  {
//	Led1ON = true;
 // }
 // else
 // {
 //    Led1ON = false;
 // }

// Create a client connection
  Client client = server.available();
	if (client) 
	{
		while (client.connected()) 
		{
			if (client.available()) 
			{
				char c = client.read();

//read char by char HTTP request
				if (readString.length() < 100) 
				{

//store characters to string

					readString = readString + c; 
				}

				Serial.print(c);  //output chars to serial port

				if (c == '\n') 
				{  //if HTTP request has ended


// manual mode when "toggle" button pressed
//maybe this part is the problem

//if(readString.indexOf("1=set+toggle") > -1) { 
 // Toggle1ON= true;
					//if(LedState1 == HIGH)
					//{
						//if(readString.indexOf("1=set+turn") > -1)
						//{
							if(readString.indexOf("1=off") > -1) 
							{
								//if(digitalRead(Led1)==1)
								digitalWrite( Led1, LOW);
								//else
							//	digitalWrite(Led1, HIGH);
							}
							if(readString.indexOf("2=on") > -1) 
							{
								//if(digitalRead(Led1)==1)
								digitalWrite( Led1, HIGH);
								//else
							//	digitalWrite(Led1, HIGH);
							}	
						//		Led1ON = false;
						
							
						//} 
						//else 
						//{
  
							//if(readString.indexOf("1=set+toggle") > -1) 
							//{
						//		digitalWrite( Led1, HIGH);
							//	Led1ON = true;
					//		}
						//}
				//	}
//}

// ended manual mode   
				//	if(readString.indexOf("1=toggleoff") > -1)
			//	{
			//		Toggle1ON= false;
			//		}

//--------------------------HTML------------------------
client.println("HTTP/1.1 200 OK");

client.println("Content-Type: text/html");

client.println();

client.print("<html><head>");

client.print("<title>Arduino Websergfhsdfhgsdfgsver </title>");
//client.println("<meta http-equiv=\"refresh\" content=\"2\">");

client.println("</head>");

client.print("<body bgcolor='white'>");
/*

client.println("<br><hr />");

client.println("<h1><div align='center'><font color='#2076CD'>Arduino Webserver</font color></div></h1>");


client.println("<div align='center'>");
client.println("<table border=5>");
*/
// display manual mode (when "toggle" button pressed)
//if(Toggle1ON){
   
//client.println("<tr>");
//client.println("<td bgcolor='red'>Sensor 1</td> <td bgcolor='yellow'>ON</td>");
//if(Led1ON){
 // client.println("<td bgcolor='white'>Led 1</td> <td bgcolor='yellow'>ON</td>");
 // }
 // else{
 // client.println("<td bgcolor='white'>Led 1</td> <td bgcolor='red'>OFF</td>");
// }
 //client.println("<td align='middle'></td><td align='middle'><form method=get><input type=submit name=1 value='off'></form></td>");
// client.println("<td align='middle'></td><td align='middle'><form method=get><input type=submit name=2 value='on'></form></td>");
  client.println("<form method=get><input type=submit name=1 value='off'></form>");
 client.println("<form method=get><input type=submit name=2 value='on'></form>");
 
 //client.println("</tr>");


//}
/*
// display automatic mode
else{
client.println("<tr>");
client.println("<td bgcolor='red'>Sensor 1</td> <td bgcolor='yellow'>ON</td>");
if(Led1ON){
  client.println("<td bgcolor='white'>Led 1</td> <td bgcolor='yellow'>ON</td>");
    }
  else {
  client.println("<td bgcolor='white'>Led 1</td> <td bgcolor='red'>OFF</td>");
 }
 client.println("<td>set turn</td><td align='center'><form method=get><input type=submit name=1 value='set toggle'></form></td>");
 client.println("<td>set turn</td><td align='center'><form method=get><input type=submit name=1 value='set toggle'></form></td>");

 client.println("</tr>");

}


client.println("</table>");
client.println("</div>");
*/
client.println("</body></html>");

//clearing string for next read
readString="";

//stopping client
delay(1);
client.stop();
}}}} 


}
