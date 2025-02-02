#include <Servo.h>  // Include Servo library

#define SERVO_PIN A3  // Servo connected to Analog Pin A3

Servo myServo;  // Create Servo object

// ========== MOTOR DRIVER PIN DEFINITIONS ==========
#define motor1Pin1 6   // Right motor forward control
#define motor1Pin2 7   // Right motor backward control
#define motor2Pin1 8   // Left motor forward control
#define motor2Pin2 9   // Left motor backward control
#define EN_B 10  // PWM speed control for left motor
#define EN_A 11  // PWM speed control for right motor

// ========== MOTOR SPEED CONFIGURATION ==========
int motorSpeedLeft = 145;  
int motorSpeedRight = 150;

// ========== COLOR SENSOR PIN DEFINITIONS ==========
#define S0 2  
#define S1 3  
#define S2 5  
#define S3 4  
#define sensorOut 12  

int red = 0, green = 0, blue = 0;

// ========== COLOR NAVIGATION VARIABLES ==========
String currentColor = "BLACK";
String previousColor = "BLACK";
int detectedRings = 0;
bool reachedCenter = false;

// ========== MOVEMENT CONFIGURATION ==========
#define turn_value 700  // Adjust turn time for accuracy
#define adjust_angle 300  // Small angle adjustment to avoid continuous detection

void setup() {
    delay(5000);
    
    // Set motor control pins as outputs
    pinMode(EN_A, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(EN_B, OUTPUT);

    // Set color sensor pins
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    myServo.attach(SERVO_PIN);  
    myServo.write(100);  
    delay(2000);

    Serial.begin(9600);

    Serial.println("Starting Challenge...");
    moveForward();
}

void loop() {   
    if (reachedCenter) return;

    red = getColorReading(LOW, LOW);  
    green = getColorReading(HIGH, HIGH);
    blue = getColorReading(LOW, HIGH);

    String detectedColor = identifyColor(red, green, blue);

    Serial.print("Detected Color: ");
    Serial.println(detectedColor);

    if (detectedColor != "BLACK" && detectedColor != currentColor) {
        // If we move to a new color, update tracking
        previousColor = currentColor;
        currentColor = detectedColor;
        detectedRings++;

        Serial.print("New Color Detected: ");
        Serial.println(currentColor);
        Serial.print("Total Rings Passed: ");
        Serial.println(detectedRings);
    }

    if (detectedRings >= 5) {
        Serial.println("🚀 Reached the Center! Dropping Flag.");
        stopMotors();
        myServo.write(100);  
        delay(1000);
        reachedCenter = true;
        return;
    }

    if (detectedColor == currentColor) {
        // If robot keeps seeing the same color, adjust slightly
        Serial.println("⚠️ Adjusting to find next color...");
        turnLeftSmall();
    }

    if (detectedColor == previousColor) {
        // If robot moves back into a previous color, reverse and adjust
        Serial.println("⚠️ Moved Backwards! Adjusting...");
        reverseMotors(400);
        turnRight();
    }

    moveForward();
}

// ========== COLOR DETECTION FUNCTIONS ==========
int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(25);
    return pulseIn(sensorOut, LOW);
}

String identifyColor(int r, int g, int b) {
    Serial.print("Processing Color -> R: ");
    Serial.print(r);
    Serial.print(" G: ");
    Serial.print(g);
    Serial.print(" B: ");
    Serial.println(b);

    if (r < 300 && g < 300 && b < 300) return "BLACK";  // Check for BLACK

    if (r > g + 50 && r > b + 50) return "RED";
    if (g > r + 50 && g > b + 50) return "GREEN";
    if (b > r + 50 && b > g + 50) return "BLUE";

    return "BLACK";  // Default to BLACK if no dominant color
}

// ========== MOVEMENT FUNCTIONS ==========
void moveForward() {
    Serial.println("Moving Forward...");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}

void stopMotors() {
    Serial.println("Stopping Motors...");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}

void reverseMotors(unsigned long duration) {
    Serial.println("Reversing...");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(duration);
    stopMotors();
}

void turnLeft() {
    Serial.println("Turning Left...");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(turn_value);
    stopMotors();
}

void turnLeftSmall() {
    Serial.println("Adjusting Left Slightly...");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(adjust_angle);
    stopMotors();
}

void turnRight() {
    Serial.println("Turning Right...");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    delay(turn_value);
    stopMotors();
}
