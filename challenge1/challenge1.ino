#include <Servo.h>  // Include Servo library

#define SERVO_PIN A3  // Servo connected to Analog Pin A3

Servo myServo;  // Create Servo object

// Motor Driver Pin Definitions
#define motor1Pin1 6   // Control pin for second motor
#define motor1Pin2 7   // Control pin for second motor
//right motor

#define motor2Pin1 8   // Control pin for first motor
#define motor2Pin2 9   // Control pin for first motor
//left motor

#define EN_B 10  // PWM speed control for left
#define EN_A 11  // PWM speed control for right

// Motor speed (adjustable between 0-255)
int motorSpeedLeft = 145;  
int motorSpeedRight = 150;

// change the pin values for light sensor
#define S0 2;
#define S1 3;
#define S2 5;
#define S3 4;
#define sensorOut 12;

int red = 0, green = 0, blue = 0;

#define turn_value 820

void setup() {
    // Set motor control pins as outputs
    pinMode(EN_A, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(EN_B, OUTPUT);

    //Set Color sensor pins
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    myServo.attach(SERVO_PIN);  // Attach servo to pin A3
   
    //calling to pick up flag at the start
    pickUpFlag();
    

    Serial.begin(9600);
    
}

void loop() {
     b

}

void moveForward() {
    Serial.println("Moving Forward...");
    analogWrite(EN_A, motorSpeedLeft); // Set speed
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}

void turnLeft() {
    Serial.println("Turning Left...");
    analogWrite(EN_A, motorSpeedLeft); // Set speed
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
    analogWrite(EN_A, motorSpeedLeft); // Set speed
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

void pickUpFlag() {
    myServo.write(100);  //open
    delay(2000);  // wait 2 seconds to line up flag

    myServo.write(150);  //close
}