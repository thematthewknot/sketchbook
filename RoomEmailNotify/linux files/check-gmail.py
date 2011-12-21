import serial, sys, feedparser, time
#Settings - Change these to match your account details
USERNAME="123321mail@gmail.com"
PASSWORD="dwlp ntjf lywg fuvr"
PROTO="https://"
SERVER="mail.google.com"
PATH="/gmail/feed/atom"
SERIALPORT = "/dev/ttyUSB0" # Change this to your serial port!
TIME=time.localtime(None)
TIME2=TIME.tm_hour
# Set up serial port
try:
	ser = serial.Serial(SERIALPORT, 9600)
except serial.SerialException:
	print "no device connected - exiting"
	sys.exit()

newmails = int(feedparser.parse(PROTO + USERNAME + ":" + PASSWORD + "@" + SERVER + PATH)["feed"]["fullcount"])

print TIME2
# Output data to serial port

if (newmails > 0)&(5<TIME2<24): 
	ser.write("M")
	print "some mail"
#else if: 
if (newmails == 0)&(5<TIME2<24): 
	ser.write("N")
	print "no mail" 
#print data to terminal


# Close serial port
ser.close()
 
