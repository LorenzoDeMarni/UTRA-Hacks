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

// Circular queue for last detected colors
#define QUEUE_SIZE 7
String colorQueue[QUEUE_SIZE];  
int colorIndex = 0;

// ========== TURN AND TIMER CONFIGURATION ==========
#define turn_value 820  // Delay time for turning (adjust for accuracy)

unsigned long startTime = 0;  
unsigned long stopTime = 0;   
bool timerRunning = false;  

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
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    myServo.attach(SERVO_PIN);  
    myServo.write(100); 
    delay(2000);

    myServo.write(150);
    delay(5000);

    myServo.write(100);  
    delay(1000);

    Serial.begin(9600);

    // Initialize color queue
    for (int i = 0; i < QUEUE_SIZE; i++) {
        colorQueue[i] = "BLACK";
    }
}

void loop() {   
    // Read color values
    
    red = getColorReading(LOW, LOW);  
    green = getColorReading(HIGH, HIGH);
    blue = getColorReading(LOW, HIGH);
    
    String detectedColor = identifyColor(red, green, blue);
    colorQueue[colorIndex] = detectedColor;
    colorIndex = (colorIndex + 1) % QUEUE_SIZE;
    String stableColor = getStableColor();
    
    Serial.print("Stable Detected Color: ");
    Serial.println(stableColor);
    moveForward();
    if (stableColor == "BLACK"){
        Serial.println("Color Detected");
        stopMotors();
    }
    
}

// ========== COLOR DETECTION FUNCTIONS ==========



int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(25);
    return pulseIn(sensorOut, LOW);
}

String identifyColor(int r, int g, int b) {
    if (r > 600 && g > 1000 && b > 1000) return "BLACK";
    else if (r < g - 15 && r < b - 15) return "RED";
    else if (g < r - 15 && g < b - 15) return "GREEN";
    else if (b < r - 15 && b < g - 15) return "BLUE";
    else return "BLACK";
}

String getStableColor() {
    int redCount = 0, greenCount = 0, blueCount = 0, blackCount = 0;
    
    for (int i = 0; i < QUEUE_SIZE; i++) {
        if (colorQueue[i] == "RED") redCount++;
        else if (colorQueue[i] == "GREEN") greenCount++;
        else if (colorQueue[i] == "BLUE") blueCount++;
        else blackCount++;
    }
    
    if (redCount >= greenCount && redCount >= blueCount && redCount >= blackCount) return "RED";
    if (greenCount >= redCount && greenCount >= blueCount && greenCount >= blackCount) return "GREEN";
    if (blueCount >= redCount && blueCount >= greenCount && blueCount >= blackCount) return "BLUE";
    return "BLACK";
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
