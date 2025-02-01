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
        // Move both motors forward
    Serial.println("Moving Forward...");
    analogWrite(EN_A, motorSpeed); // Set speed
    analogWrite(EN_B, motorSpeed);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    // digitalWrite(motor2Pin1, HIGH);
    // digitalWrite(motor2Pin2, LOW);
    delay(2000);


    // Stop motors again
    Serial.println("Stopping Motors...");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    // digitalWrite(motor2Pin1, LOW);
    // digitalWrite(motor2Pin2, LOW);
    delay(1000);
}

void loop() {

}
