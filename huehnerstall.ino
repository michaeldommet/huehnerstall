#include <Wire.h>

// Define constants
const int DOOR_DOWN_MOTOR_PIN = 2;    // Wind motor down
const int DOOR_UP_MOTOR_PIN = 3;      // Wind motor up
const int LIGHT_SENSOR_PIN = A1;      // Light sensor pin
const int BRIGHTNESS_HIGH_THRESHOLD = 400;  // Threshold for raising door
const int BRIGHTNESS_LOW_THRESHOLD = 100;   // Threshold for lowering door
const int DOOR_MOVEMENT_DELAY = 12000;      // Time required for door movement
const int SENSOR_READ_DELAY = 2000;         // Delay between sensor readings
const int SERIAL_BAUD_RATE = 9600;          // Serial communication speed

// Door state tracking
bool isDoorOpen = true;

void setup() {
    // Initialize serial communication
    Serial.begin(SERIAL_BAUD_RATE);
    
    // Set pin modes
    pinMode(DOOR_UP_MOTOR_PIN, OUTPUT);
    pinMode(DOOR_DOWN_MOTOR_PIN, OUTPUT);
    
    // Ensure door is stopped at startup
    stopDoor();
    
    Serial.println("Door control system initialized");
}

void loop() {
    // Read brightness value from sensor
    int brightness = analogRead(LIGHT_SENSOR_PIN);
    Serial.print("Current brightness: ");
    Serial.println(brightness);
    
    // Check if door needs to be opened (bright and door is closed)
    if (brightness > BRIGHTNESS_HIGH_THRESHOLD && !isDoorOpen) {
        Serial.println("Bright conditions detected - opening door");
        operateDoor(true);
        isDoorOpen = true;
    } 
    // Check if door needs to be closed (dark and door is open)
    else if (brightness < BRIGHTNESS_LOW_THRESHOLD && isDoorOpen) {
        Serial.println("Low light conditions detected - confirming reading...");
        delay(SENSOR_READ_DELAY);  // Double-check before closing
        
        // Read brightness again to confirm it's still dark
        brightness = analogRead(LIGHT_SENSOR_PIN);
        if (brightness < BRIGHTNESS_LOW_THRESHOLD) {
            Serial.println("Confirmed low light - closing door");
            operateDoor(false);
            isDoorOpen = false;
        } else {
            Serial.println("Light conditions changed - maintaining door position");
        }
    } 
    else {
        Serial.println("No action needed - maintaining door position");
    }
    
    // Wait before next reading
    delay(SENSOR_READ_DELAY);
}

/**
 * Operates the door (open or close)
 * @param raise true to raise the door, false to lower it
 */
void operateDoor(bool raise) {
    if (raise) {
        digitalWrite(DOOR_UP_MOTOR_PIN, HIGH);
        digitalWrite(DOOR_DOWN_MOTOR_PIN, LOW);
        Serial.println("Door raising...");
        delay(DOOR_MOVEMENT_DELAY - 100);  // Slight adjustment for raising
    } else {
        digitalWrite(DOOR_DOWN_MOTOR_PIN, HIGH);
        digitalWrite(DOOR_UP_MOTOR_PIN, LOW);
        Serial.println("Door lowering...");
        delay(DOOR_MOVEMENT_DELAY);
    }
    
    // Stop door after movement completes
    stopDoor();
}

/**
 * Stops the door movement
 */
void stopDoor() {
    digitalWrite(DOOR_UP_MOTOR_PIN, LOW);
    digitalWrite(DOOR_DOWN_MOTOR_PIN, LOW);
    Serial.println("Door stopped");
}
