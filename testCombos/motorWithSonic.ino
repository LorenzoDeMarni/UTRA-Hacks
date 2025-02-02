// Motor Driver Pin Definitions
#define BUTTON_PIN 13  // Pin where the button is connected
#define EN_A 11  // PWM speed control for motor 1
#define motor1Pin1 6   // Control pin for right motor
#define motor1Pin2 7   // Control pin for right motor
#define motor2Pin1 8   // Control pin for left motor
#define motor2Pin2 9   // Control pin for left motor
#define EN_B 10  // PWM speed control for motor 2

// Motor speed (adjustable between 0-255)
int motorSpeedLeft = 145;  
int motorSpeedRight = 150;

// Ultra sonic sensor
#define TRIG_PIN A4
#define ECHO_PIN A5
#define WALL_DISTANCE_THRESHOLD 300

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Set motor control pins as outputs
    pinMode(EN_A, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(EN_B, OUTPUT);

    pinMode(BUTTON_PIN, INPUT_PULLUP); // Use internal pull-up resistor

    Serial.begin(9600);
    Serial.println("Motor System Initialized");
}

void loop() {
    moveForward();

    long duration;
    float distance = 0.0;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure the time it takes for the echo to return
    duration = pulseIn(ECHO_PIN, HIGH);

    // Convert the time to distance (in mm)
    distance = (duration * 0.343) / 2; 
    delay(200);

    if (distance > 0 && distance <= WALL_DISTANCE_THRESHOLD) {
       stopMotors();
       delay(300);
       turnRight(700);
    } 
    else {
       Serial.println("aye u good bro");
    }

}

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
    Serial.println("STOPPPPPPPPPP! IVE PLAYED THESE GAMES BEFORE");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}

void turnRight(int duration) {
    Serial.println("To the right to the right");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(duration);
    stopMotors();
}