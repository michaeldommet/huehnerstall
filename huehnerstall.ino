#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

const int relayPin = 4; // Connect the relay module signal pin to digital pin 4

const int DoorDownMotorPin = 2; // wind motor down
const int DoorUpMotorPin = 3;   // wind motor up
bool DoorOpen = false;

void setup()
{
    Serial.begin(9600);

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (1)
            ;
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    pinMode(relayPin, OUTPUT);
    pinMode(DoorUpMotorPin, OUTPUT);
    pinMode(DoorDownMotorPin, OUTPUT);
    delay(3000);
}

void loop()
{
    DateTime now = rtc.now();

    // Set the activation time every day between 13:00 and 19:00
    DateTime startTime(now.year(), now.month(), now.day(), 13, 0, 0);
    DateTime endTime(now.year(), now.month(), now.day(), 19, 0, 0);

    // Check if the current time is within the specified range
    if (now >= startTime && now <= endTime)
    {
        // Activate the relay for the entire duration
        digitalWrite(relayPin, HIGH);
    }
    else
    {
        // Deactivate the relay outside the specified range
        digitalWrite(relayPin, LOW);
    }

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
    else if (brightens < 200 && DoorOpen == true)
    {
        Serial.println("Door should be down");
        lowerDoor();
        stopDoor();
        DoorOpen = false;
    }
    else
    {
        Serial.println("I am not doing anything");
    }
}

// Wind the Door Up
void raiseDoor()
{
    digitalWrite(DoorUpMotorPin, HIGH);
    digitalWrite(DoorDownMotorPin, LOW);
    Serial.println("Door Raising");
    delay(11000);
}

// Wind The Door Down
void lowerDoor()
{
    digitalWrite(DoorDownMotorPin, HIGH);
    digitalWrite(DoorUpMotorPin, LOW);
    Serial.println("Door Lowering");
    delay(10900);
}

void stopDoor()
{
    digitalWrite(DoorUpMotorPin, LOW);
    digitalWrite(DoorDownMotorPin, LOW);
    Serial.println("Door Stop");
}
