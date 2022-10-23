
#include <Wire.h>

// Define what pins are used
const int DoorDownMotorPin = 2; // wind motor down
const int DoorUpMotorPin = 3;    // wind motor up
bool DoorOpen = false;

// the setup function runs once when you press reset or power the board
void setup()
{
    // Set the mode the pins will operate in.
    Serial.begin(9600);
    pinMode(DoorUpMotorPin, OUTPUT);
    pinMode(DoorDownMotorPin, OUTPUT);
    delay(3000);
}

// the loop function runs over and over again until power down or reset
void loop()
{
    int brightens = analogRead(A1);
    delay(2000);
    Serial.println(brightens);
    if (brightens > 400 && DoorOpen == false)
    {
        Serial.println("Door should be up");
        raiseDoor();
        stopDoor();
        DoorOpen = true;
     
    }
    else if (brightens < 300  && DoorOpen == true)
    {
        Serial.println("Door should be down");
        lowerDoor();
        stopDoor();
        DoorOpen = false;
    }
    else {
        Serial.println("I am not doing anything");
    }

    
}

// Wind the Door Up
void raiseDoor()
{
    digitalWrite(DoorUpMotorPin, HIGH);
    digitalWrite(DoorDownMotorPin, LOW);
    Serial.println("Door Raising");
    delay(12000);
}

// Wind The Door Down
void lowerDoor()
{
    digitalWrite(DoorDownMotorPin, HIGH);
    digitalWrite(DoorUpMotorPin, LOW);
    Serial.println("Door Lowering");
    delay(12000);
}

void stopDoor()
{
    digitalWrite(DoorUpMotorPin, LOW);
    digitalWrite(DoorDownMotorPin, LOW);
    Serial.println("Door Stop");
}
