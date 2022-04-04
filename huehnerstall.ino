/***********************************************************************************************
 * Chickencoop door controller based on github project https://github.com/chrisuthe/CoopDoor   *
 * Switch on pin 3 to override and keep door closed                                            *
 ***********************************************************************************************/

#include <Wire.h>

// Define what pins are used
const int doorUpSensorPin = 4;   // reedswitch door open
const int doorDownSensorPin = 5; // reedswitch door down
const int doorDownMotorPin = 10; // wind motor down
const int DoorUpMotorPin = 9;    // wind motor up
const int builtInLED = 12;       // LED idicating door closed (reedswitch on pin 5)
const byte CloseDoorManual = 3;  // Close door by hand
int buttonstateclose = 0;

// the setup function runs once when you press reset or power the board
void setup()
{
    // Set the mode the pins will operate in.
    pinMode(DoorUpMotorPin, OUTPUT);
    pinMode(doorDownMotorPin, OUTPUT);
    pinMode(CloseDoorManual, INPUT);

    // switches setup
    pinMode(doorUpSensorPin, INPUT);
    pinMode(doorDownSensorPin, INPUT);
    pinMode(builtInLED, OUTPUT);
    if (digitalRead(doorUpSensorPin) == HIGH)
    {
        Serial.println("Door is open");
    }

    if (digitalRead(doorDownSensorPin) == HIGH)
    {
        Serial.println("Door is closed");
    }
    delay(3000);
    // TO DO , if the door is hanged some where , we need to open it and then close it .
}

// the loop function runs over and over again until power down or reset
void loop()
{
    delay(2000);
    buttonstateclose = digitalRead(CloseDoorManual);
    if (buttonstateclose == HIGH)
    {
        ManualClose();
    }

    // lets start comparisons, if the door should be up....
    // delay of 30 minutes after sunset time to make sure all chickens are inside before closing the door.
    if ((sunrise + 30 < currentMins && currentMins < sunset + 30) && (buttonstateclose != HIGH))
    {
        Serial.println("Door should be up");
        while (digitalRead(doorUpSensorPin) == LOW)
            raiseDoor();
    }

    else
    {
        Serial.println("Door should be down");
        while (digitalRead(doorDownSensorPin) == LOW)
            lowerDoor();
    }

    int DoorUp = digitalRead(doorUpSensorPin);
    int DoorDown = digitalRead(doorDownSensorPin);
    /* This is just temporary switch debug code*/
    if (DoorUp == HIGH)
    {
        Serial.println("Door Opened All The Way");
        digitalWrite(builtInLED, LOW);
        stopDoor();
    }

    else
    {
        digitalRead(doorDownSensorPin == HIGH);
        Serial.println("Door Closed All The Way");
        digitalWrite(builtInLED, HIGH);
        stopDoor();
    }
}

// Wind the Door Up
void raiseDoor()
{
    digitalWrite(DoorUpMotorPin, HIGH);
    digitalWrite(doorDownMotorPin, LOW);
    Serial.println("Door Raising");
}

// Wind The Door Down
void lowerDoor()
{
    digitalWrite(doorDownMotorPin, HIGH);
    digitalWrite(DoorUpMotorPin, LOW);
    Serial.println("Door Lowering");
}

// Stop the Door
void stopDoor()
{
    digitalWrite(DoorUpMotorPin, LOW);
    digitalWrite(doorDownMotorPin, LOW);
    Serial.println("Door Stop");
}

// Close door manual
void ManualClose()
{
    digitalWrite(doorDownMotorPin, HIGH);
    digitalWrite(DoorUpMotorPin, LOW);
}
