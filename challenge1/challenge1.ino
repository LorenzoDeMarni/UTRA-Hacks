// Motor Driver Pin Definitions
#define EN_A 11  // PWM speed control for motor 1
#define motor1Pin1 6   // Control pin for second motor
#define motor1Pin2 7   // Control pin for second motor
#define motor2Pin1 8   // Control pin for first motor
#define motor2Pin2 9   // Control pin for first motor
#define EN_B 10  // PWM speed control for motor 2

// Motor speed (adjustable between 0-255)
int motorSpeed = 150;  // 150 is about 60% speed

void setup() {
    // Set motor control pins as outputs
    pinMode(EN_A, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(EN_B, OUTPUT);

    Serial.begin(9600);
    Serial.println("Motor System Initialized");
    
}

void loop() {
    // Move forward for 2 seconds
    moveForward(2000);
    // Turn left for 1 second
    turnLeft(1000);
    // Move forward for 2 seconds
    moveForward(2000);
    // Turn right for 1 second
    turnRight(1000);
    // Move forward for 2 seconds
    moveForward(2000);
    // Stop motors for 1 second
    stopMotors();
    delay(1000);
    break;

}

void moveForward(int duration) {
    Serial.println("Moving Forward...");
    analogWrite(EN_A, motorSpeed); // Set speed
    analogWrite(EN_B, motorSpeed);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(duration);
    stopMotors();
}

void turnLeft(int duration) {
    Serial.println("Turning Left...");
    analogWrite(EN_A, motorSpeed); // Set speed
    analogWrite(EN_B, motorSpeed);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(duration);
    stopMotors();
}

void turnRight(int duration) {
    Serial.println("Turning Right...");
    analogWrite(EN_A, motorSpeed); // Set speed
    analogWrite(EN_B, motorSpeed);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(duration);
    stopMotors();
}

void stopMotors() {
    Serial.println("Stopping Motors...");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}