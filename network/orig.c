#include <SPI.h>  
#include <Ethernet.h>


byte mac[] = { 0x54, 0x55, 0x58, 0x10, 0x00, 0x24 };  //  MAC 
byte ip[]  = { 167, 205, 45, 177 };                  // IP-Adress
byte gateway[] = { 167, 205, 1, 1 };                // Gateway
byte subnet[]  = { 255, 255, 255, 0 };

Server server(80);
int Pir1 = 2;  //sensor pir
int Led1 = 12;  //  led output
boolean Pir1ON, Led1ON, Toggle1ON = false; 
int PirState1 = 0;

String readString = String(100);      // string for fetching data from address

void setup(){
Ethernet.begin(mac, ip, gateway, subnet);
server.begin();
 
pinMode(Pir1, INPUT);
pinMode(Led1, OUTPUT);
 }

void loop(){
int PirState1 =digitalRead(Pir1); //reading pir sensor status
int LedState1 = digitalRead(Led1); // reading led status

// automatic mode

if(PirState1 == HIGH){  
  Pir1ON = true;
 digitalWrite(Led1, HIGH); 
 }
  else{
    Pir1ON = false;
        digitalWrite(Led1, LOW);
}

 if(LedState1 == HIGH){
 Led1ON = true;
 }
 else{
   Led1ON = false;
 }

// Create a client connection
Client client = server.available();
if (client) {
while (client.connected()) {
if (client.available()) {
char c = client.read();

//read char by char HTTP request
if (readString.length() < 100) {

//store characters to string

readString = readString + c; 

}

Serial.print(c);  //output chars to serial port

if (c == '\n') {  //if HTTP request has ended


// manual mode when "toggle" button pressed
//maybe this part is the problem

if(readString.indexOf("1=set+toggle") > -1) { 
  Toggle1ON= true;
  if(LedState1 == HIGH){
  if(readString.indexOf("1=set+turn") > -1) {
    digitalWrite( Led1, LOW);
    Led1ON = false;
   }} else {
      if(readString.indexOf("1=set+turn") > -1) {
    digitalWrite( Led1, HIGH);
    Led1ON = true;
  }
   }
}

// ended manual mode   
if(readString.indexOf("1=toggleoff") > -1){
  Toggle1ON= false;
}

//--------------------------HTML------------------------
client.println("HTTP/1.1 200 OK");

client.println("Content-Type: text/html");

client.println();

client.print("<html><head>");

client.print("<title>Arduino Webserver </title>");
client.println("<meta http-equiv=\"refresh\" content=\"2\">");

client.println("</head>");

client.print("<body bgcolor='white'>");


client.println("<br><hr />");

client.println("<h1><div align='center'><font color='#2076CD'>Arduino Webserver</font color></div></h1>");


client.println("<div align='center'>");
client.println("<table border=5>");

// display manual mode (when "toggle" button pressed)
if(Toggle1ON){
  if(Pir1ON){   
client.println("<tr>");
client.println("<td bgcolor='red'>Sensor 1</td> <td bgcolor='yellow'>ON</td>");
if(Led1ON){
  client.println("<td bgcolor='white'>Led 1</td> <td bgcolor='yellow'>ON</td>");
  }
  else{
  client.println("<td bgcolor='white'>Led 1</td> <td bgcolor='red'>OFF</td>");
 }
 client.println("<td align='middle'><form method=get><input type=submit name=1 value='set turn'></form></td><td align='middle'><form method=get><input type=submit name=1 value='toggleoff'></form></td>");
 client.println("</tr>");
}
else{
  client.println("<tr>");
  client.println("<td bgcolor='white'>Sensor 1</td><td bgcolor='red'>OFF</td><td bgcolor='white'>Led 1</td> <td bgcolor='red'>OFF</td><td align='middle'><form method=get><input type=submit name=1 value='set turn'><td align='middle'><form method=get><input type=submit name=1 value='toggleoff'></form></td>");
  client.println("</tr>");
  }
}

// display automatic mode
else{
if(Pir1ON){   
client.println("<tr>");
client.println("<td bgcolor='red'>Sensor 1</td> <td bgcolor='yellow'>ON</td>");
if(Led1ON){
  client.println("<td bgcolor='white'>Led 1</td> <td bgcolor='yellow'>ON</td>");
    }
  else {
  client.println("<td bgcolor='white'>Led 1</td> <td bgcolor='red'>OFF</td>");
 }
 client.println("<td>set turn</td><td align='center'><form method=get><input type=submit name=1 value='set toggle'></form></td>");
 client.println("</tr>");
}
else{
  client.println("<tr>");
  client.println("<td bgcolor='white'>Sensor 1</td><td bgcolor='red'>OFF</td><td bgcolor='white'>Led 1</td><td bgcolor='red'>OFF</td> <td align='center'>set turn</td> <td align='center'><form method=get><input type=submit name=1 value='set toggle'></form></td>");
  client.println("</tr>");
  }
}


client.println("</table>");
client.println("</div>");

client.println("</body></html>");

//clearing string for next read
readString="";

//stopping client
delay(1);
client.stop();
}}}} 

}



/*
  Web  Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 4 Sep 2010
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,1, 177 };

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
Server server(80);

void setup()
{
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  // listen for incoming clients
  Client client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(analogRead(analogChannel));
            client.println("<br />");
          }
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
