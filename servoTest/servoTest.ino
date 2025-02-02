#include <Servo.h>  // Include Servo library

#define SERVO_PIN A3  // Servo connected to Analog Pin A3

Servo myServo;  // Create Servo object

void setup() {
    myServo.attach(SERVO_PIN);  // Attach servo to pin A3
    Serial.begin(9600);
    Serial.println("Servo Test Initialized...");
    
    testServo();  // Run the test function once
}

void loop() {
    // Empty loop, only runs test once in setup()
}

void testServo() {
    myServo.write(100);  // Move servo back to 0°
    delay(2000);  // Wait 1 second

    myServo.write(150);  // Move servo to 30°
    delay(5000);  // Wait 1 second

    myServo.write(100);  // Move servo back to 0°
    delay(1000);  // Wait 1 second

    Serial.println("Servo Test Complete.");
}
