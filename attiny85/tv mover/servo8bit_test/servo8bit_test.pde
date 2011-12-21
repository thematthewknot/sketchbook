#include "Servo8Bit.h"

Servo8Bit myServo;  //create a servo object.
                    //a maximum of five servo objects can be created


void setup()
{
    myServo.attach(1);  //attach the servo to pin PB1

    //the timer is configured inside the servo8Bit library, but some code elsewhere
    //outside the library is overwritting it. As a hack, configure the timer again here.

    TCCR1 = 0;  //clear the "Timer Counter Control Register 1"
    //set counter1 prescaler to 8
    //our F_CPU is 1mhz so this makes each timer tick be 8 microseconds long
    TCCR1 &= ~(1<< CS13); //clear
    TCCR1 |=  (1<< CS12); //set
    TCCR1 &= ~(1<< CS11); //clear
    TCCR1 &= ~(1<< CS10); //clear
}


void loop()
{





    for(int pos = 0; pos < 180; pos++)  // goes from 0 degrees to 180 degrees
    {                                   // in steps of 1 degree
        myServo.write(pos);             // tell servo to go to position in variable 'pos'
        delay(15);                      // waits 15ms for the servo to reach the position
    }

    for(int pos = 180; pos > 1; pos--)  // goes from 180 degrees to 0 degrees
    {
        myServo.write(pos);             // tell servo to go to position in variable 'pos'
        delay(15);                      // waits 15ms for the servo to reach the position
    }

}
