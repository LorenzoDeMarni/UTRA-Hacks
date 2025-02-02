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
#define S0 2  // Frequency scaling pin
#define S1 3  // Frequency scaling pin
#define S2 5  // Color select pin
#define S3 4  // Color select pin
#define sensorOut 12  // Output pin for color sensor frequency signal

int red = 0, green = 0, blue = 0;

// ========== TURN AND TIMER CONFIGURATION ==========
#define turn_value 820  // Delay time for turning (adjust for accuracy)

#define longerRuntime 0
unsigned long startTime = 0;  // Stores the start time
unsigned long stopTime = 0;   // Stores the stop time
unsigned long elapsedTime = 0;  // Stores the total elapsed time
bool timerRunning = false;  // Flag for tracking timer status

// ========== SETUP FUNCTION ==========
void setup() {
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
    digitalWrite(S0, HIGH);  // Set frequency scaling to 20%
    digitalWrite(S1, LOW);

    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    myServo.attach(SERVO_PIN);  // Attach servo to pin A3
    
    Serial.begin(9600);
    
    pickUpFlag();  // Pick up flag at the start
    // startUp();  // Move into the first ring
}

// ========== MAIN LOOP ==========
void loop() {   

    // Move forward until black (outer ring) is detected
    do{
      red = getColorReading(LOW, LOW);  // Read Red intensity
      green = getColorReading(HIGH, HIGH);  // Read Green intensity
      blue = getColorReading(LOW, HIGH);  // Read Blue intensity
      String colorNew = identifyColor(red, green, blue);
      moveForward();
      delay(3000);
      colorNew="RED";
    } while (colorNew == "BLACK");
    
    startTime = millis();  // Start timer for tracking movement time
    stopMotors();

    timerRunning = true;
    moveForward();
    Serial.println("Timer Started!");
    
    // Continue moving forward until reaching black again
    while (colorNew != "BLACK") { 
        moveForward();
        delay(4000);
        colorNew="BLACK";
    }
    
    stopTime = millis();
    stopMotors();
    timerRunning = false;

    moveBackward();
    delay((stopTime - startTime) / 2);  // Move backward to center of the line
    turnLeft();
    delay(500);

    // Move forward until black is detected 

    while (colorNew != "BLACK") { 
        moveForward();
        delay(3000);
        colorNew="BLACK";

    }
    delay(500);
    startTime = millis();  // Start timer for tracking movement time

    // Move backward until black is detected
    while (colorNew != "BLACK") { 
        moveBackward();
        delay(4000);
        colorNew="BLACK";
    }
    
    stopTime = millis();
    stopMotors();
    delay(1000);
    
    moveForward();
    delay((stopTime - startTime) / 2);  // Move forward to center of the ring
    stopMotors();
    dropFlag();  // Drop the flag at the final position
}

// ========== MOVEMENT FUNCTIONS ==========
void moveForward() {
    Serial.println("Moving Forward...");
    analogWrite(EN_A, motorSpeedLeft);  // Set left motor speed
    analogWrite(EN_B, motorSpeedRight);  // Set right motor speed
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}

void moveBackward() {
    Serial.println("Moving Backward...");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
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

void turnRight() {
    Serial.println("Turning Right...");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(turn_value);
    stopMotors();
}

void stopMotors() {
    Serial.println("Stopping Motors...");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}

// ========== COLOR DETECTION FUNCTIONS ==========
String identifyColor(int r, int g, int b) {
    if (r < g - 15 && r < b - 15) return "RED";
    else if (g < r - 15 && g < b - 15) return "GREEN";
    else if (b < r - 15 && b < g - 15) return "BLUE";
    else return "BLACK";  // Default to BLACK if not RED, GREEN, or BLUE
}

int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(100);  // Allow sensor to settle
    return pulseIn(sensorOut, LOW);  // Measure pulse duration
}

// ========== FLAG PICK-UP AND DROP FUNCTIONS ==========
void pickUpFlag() {
    myServo.write(100);  // Open claw to grab flag
    delay(2000);  // Wait 2 seconds for alignment
    myServo.write(150);  // Close claw to hold flag
}

void dropFlag() {
    myServo.write(100);  // Open claw to release flag
}

// ========== STARTUP SEQUENCE ==========
void startUp() {
    red = getColorReading(LOW, LOW);
    green = getColorReading(HIGH, HIGH);
    blue = getColorReading(LOW, HIGH);

    String color = identifyColor(red, green, blue);

    // Move forward until black is detected
    while (color != "BLACK") {
        moveForward();
        startTime = millis();
        timerRunning = true;
        Serial.println("Timer Started!");
    }

    stopTime = millis();
    timerRunning = false;
    longerRuntime = stopTime - startTime;

    turnLeft();

    String newColor = identifyColor(red, green, blue);

    // Keep turning until a new color (not black or the same as current) is detected
    while (newColor == color || newColor == "BLACK") {
        turnLeft();
        delay(1000);
    }

    stopMotors();
}
