#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C  // Check if your display uses 0x3D instead
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// BME280 sensor
Adafruit_BME280 bme;
float temperature, humidity, pressure;
bool bmeInitialized = false;

// Define constants
const int DOOR_DOWN_MOTOR_PIN = 2;    // Wind motor down
const int DOOR_UP_MOTOR_PIN = 3;      // Wind motor up
const int LIGHT_SENSOR_PIN = A1;      // Light sensor pin
const int BRIGHTNESS_HIGH_THRESHOLD = 400;  // Threshold for raising door
const int BRIGHTNESS_LOW_THRESHOLD = 100;   // Threshold for lowering door
const int DOOR_MOVEMENT_DELAY = 11000;      // Time required for door movement
const int SENSOR_READ_DELAY = 2000;         // Delay between sensor readings
const int DISPLAY_REFRESH_INTERVAL = 5000;  // Refresh display every 5 seconds
const int SERIAL_BAUD_RATE = 9600;          // Serial communication speed

// Door state tracking
bool isDoorOpen = true;
unsigned long lastDisplayUpdate = 0;

void setup() {
    // Initialize serial communication
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println(F("Door Control System Starting..."));
    
    // Set pin modes
    pinMode(DOOR_UP_MOTOR_PIN, OUTPUT);
    pinMode(DOOR_DOWN_MOTOR_PIN, OUTPUT);
    
    // Ensure door is stopped at startup
    stopDoor();

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    Serial.println(F("Initializing OLED display..."));
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        while(1); // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen
    display.display();
    delay(1000); // Pause for 1 second
    
    // Initialize BME280 sensor
    Serial.println(F("Initializing BME280 sensor..."));
    if (!bme.begin(0x76)) {  // Try 0x77 if 0x76 doesn't work
        Serial.println(F("Could not find BME280 sensor!"));
        // Clear the buffer
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println(F("BME280 not found!"));
        display.display();
    } else {
        bmeInitialized = true;
        Serial.println(F("BME280 initialized"));
    }
    
    // Display startup message
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Door Control System"));
    display.setCursor(0, 16);
    display.println(F("Status: Ready"));
    display.setCursor(0, 32);
    display.print(F("Door: "));
    if (isDoorOpen) {
        display.println(F("Open"));
    } else {
        display.println(F("Closed"));
    }
    display.display(); // Make sure to call display() to update screen
    
    Serial.println(F("Door control system initialized"));
}

void loop() {
    // Read sensor data
    int brightness = analogRead(LIGHT_SENSOR_PIN);
    
    // Read BME280 sensor if available
    if (bmeInitialized) {
        temperature = bme.readTemperature();  // Celsius
        humidity = bme.readHumidity();        // %
        pressure = bme.readPressure() / 100.0F; // hPa
        
        Serial.print(F("Temperature: "));
        Serial.print(temperature);
        Serial.print(F("°C, Humidity: "));
        Serial.print(humidity);
        Serial.print(F("%, Pressure: "));
        Serial.print(pressure);
        Serial.println(F(" hPa"));
    }
    
    Serial.print(F("Current brightness: "));
    Serial.println(brightness);
    
    // Update display periodically
    unsigned long currentMillis = millis();
    if (currentMillis - lastDisplayUpdate >= DISPLAY_REFRESH_INTERVAL) {
        updateDisplay(brightness);
        lastDisplayUpdate = currentMillis;
    }
    
    // Check if door needs to be opened (bright and door is closed)
    if (brightness > BRIGHTNESS_HIGH_THRESHOLD && !isDoorOpen) {
        Serial.println(F("Bright conditions detected - opening door"));
        updateDoorStatusOnDisplay(F("Opening..."));
        operateDoor(true);
        isDoorOpen = true;
        updateDoorStatusOnDisplay(F("Open"));
    } 
    // Check if door needs to be closed (dark and door is open)
    else if (brightness < BRIGHTNESS_LOW_THRESHOLD && isDoorOpen) {
        Serial.println(F("Low light conditions detected - confirming reading..."));
        updateDoorStatusOnDisplay(F("Checking..."));
        delay(SENSOR_READ_DELAY);  // Double-check before closing
        
        // Read brightness again to confirm it's still dark
        brightness = analogRead(LIGHT_SENSOR_PIN);
        if (brightness < BRIGHTNESS_LOW_THRESHOLD) {
            Serial.println(F("Confirmed low light - closing door"));
            updateDoorStatusOnDisplay(F("Closing..."));
            operateDoor(false);
            isDoorOpen = false;
            updateDoorStatusOnDisplay(F("Closed"));
        } else {
            Serial.println(F("Light conditions changed - maintaining door position"));
            if (isDoorOpen) {
                updateDoorStatusOnDisplay(F("Open"));
            } else {
                updateDoorStatusOnDisplay(F("Closed"));
            }
        }
    } 
    else {
        Serial.println(F("No action needed - maintaining door position"));
    }
    
    // Wait before next reading
    delay(SENSOR_READ_DELAY);
}

/**
 * Updates the information on the OLED display
 */
void updateDisplay(int brightness) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Door Control System"));
    
    // Display environmental data
    display.setCursor(0, 16);
    if (bmeInitialized) {
        display.print(F("Temp: "));
        display.print(temperature, 1);
        display.println(F("C"));
        
        display.setCursor(0, 26);
        display.print(F("Humidity: "));
        display.print(humidity, 0);
        display.println(F("%"));
    } else {
        display.println(F("BME280 not available"));
    }
    
    // Display light level
    display.setCursor(0, 40);
    display.print(F("Light: "));
    display.println(brightness);
    
    // Door status
    display.setCursor(0, 54);
    display.print(F("Door: "));
    if (isDoorOpen) {
        display.println(F("Open"));
    } else {
        display.println(F("Closed"));
    }
    
    display.display(); // Make sure to call display()
}

/**
 * Updates only the door status line on the display
 */
void updateDoorStatusOnDisplay(const __FlashStringHelper* status) {
    // Preserve most of the display, just update the door status
    display.fillRect(30, 54, 98, 10, SSD1306_BLACK);  // Clear just the status text area
    display.setCursor(30, 54);
    display.print(status);
    display.display(); // Make sure to call display()
}

/**
 * Operates the door (open or close)
 * @param raise true to raise the door, false to lower it
 */
void operateDoor(bool raise) {
    if (raise) {
        digitalWrite(DOOR_UP_MOTOR_PIN, HIGH);
        digitalWrite(DOOR_DOWN_MOTOR_PIN, LOW);
        Serial.println(F("Door raising..."));
        delay(DOOR_MOVEMENT_DELAY - 100);  // Slight adjustment for raising
    } else {
        digitalWrite(DOOR_DOWN_MOTOR_PIN, HIGH);
        digitalWrite(DOOR_UP_MOTOR_PIN, LOW);
        Serial.println(F("Door lowering..."));
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
    Serial.println(F("Door stopped"));
}
