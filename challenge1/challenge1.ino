#include <Servo.h>

// Motor Driver Pin Definitions
#define EN_A 11  // PWM speed control for motor 1
#define motor1Pin1 6   // Control pin for motor 1
#define motor1Pin2 7   // Control pin for motor 1
#define motor2Pin1 8   // Control pin for motor 2
#define motor2Pin2 9   // Control pin for motor 2
#define EN_B 10  // PWM speed control for motor 2

// Color Sensor Pins
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 12  // Output pin for frequency signal

// Servo for turning
Servo servoMotor;
#define SERVO_PIN A3

// Motor speed (adjustable)
int motorSpeedLeft = 145;  // Left motor speed
int motorSpeedRight = 150; // Right motor speed

// Color Detection Variables
String detectedColors[10];  // Store detected sequence
int colorIndex = 0;

// Degrees to turn based on ring count
int turnDegrees[] = {0, 60, 90, 120, 150};

// Movement Control
void moveForward() {
    analogWrite(EN_A, motorSpeedRight);  // Right motor at 150
    analogWrite(EN_B, motorSpeedLeft);   // Left motor at 145
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}

void stopMotors() {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}

void turnLeft(int degrees) {
    Serial.print("Turning Left: ");
    Serial.print(degrees);
    Serial.println(" degrees");

    servoMotor.write(90 + degrees);  // Adjust for servo movement
    delay(500);
    moveForward();
}

// Color Sensor Setup
void colorSensorSetup() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    Serial.begin(9600);
}

// Get Detected Color
String getColor() {
    int red, green, blue;

    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red = pulseIn(sensorOut, LOW);

    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    green = pulseIn(sensorOut, LOW);

    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blue = pulseIn(sensorOut, LOW);

    Serial.print("Red: ");
    Serial.print(red);
    Serial.print(" Green: ");
    Serial.print(green);
    Serial.print(" Blue: ");
    Serial.println(blue);

    if (red < green && red < blue) return "RED";
    if (green < red && green < blue) return "GREEN";
    if (blue < red && blue < green) return "BLUE";
    return "BLACK";
}

// Main Setup
void setup() {
    pinMode(EN_A, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(EN_B, OUTPUT);

    servoMotor.attach(SERVO_PIN);

    Serial.begin(9600);
    Serial.println("System Initialized");

    colorSensorSetup();
}

// Main Loop
void loop() {
    moveForward();
    String currentColor = getColor();

    if (currentColor != "BLACK") {  // Start counting when a color is detected
        Serial.println("Color Detected! Algorithm Starting...");
        delay(100);
        break;
    }
}

// Main Algorithm Loop
void algorithmLoop() {
    int ringCount = 0;

    while (true) {
        moveForward();
        String currentColor = getColor();

        if (currentColor == "BLACK") {  
            Serial.println("Outer Black Ring Reached. Stopping...");
            stopMotors();
            break;
        }

        if (colorIndex > 0 && currentColor == detectedColors[colorIndex - 2]) {  
            int turnAngle = turnDegrees[ringCount % 5]; 
            turnLeft(turnAngle);
            ringCount = 0;
        } else {  
            detectedColors[colorIndex] = currentColor;
            colorIndex++;
            ringCount++;
        }

        delay(500);
    }
}
