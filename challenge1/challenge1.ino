// Motor Driver Pin Definitions
#define EN_A 11  // PWM speed control for right
#define motor1Pin1 6   // Control pin for second motor
#define motor1Pin2 7   // Control pin for second motor
//left motor
#define motor2Pin1 8   // Control pin for first motor
#define motor2Pin2 9   // Control pin for first motor
//right method
#define EN_B 10  // PWM speed control for left

// Motor speed (adjustable between 0-255)
int motorSpeed = 150;  // 150 is about 60% speed

# https://www.instructables.com/Running-DC-Motor-With-Arduino-Using-L298N-Motor-Dr/

// change the pin values for light sensor
#define S0 2;
#define S1 3;
#define S2 4;
#define S3 5;
#define sensorOut 8;

int red = 0, green = 0, blue = 0;

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

void color_setup() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    Serial.begin(9600);
}

void motor_setup() {
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    Serial.begin(9600);
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


void base_move() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        if (command == 'F') { // Move Forward
            digitalWrite(motor1Pin1, HIGH);
            digitalWrite(motor1Pin2, LOW);
            digitalWrite(motor2Pin1, HIGH);
            digitalWrite(motor2Pin2, LOW);
        } else if (command == 'S') { // Stop
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, LOW);
            digitalWrite(motor2Pin1, LOW);
            digitalWrite(motor2Pin2, LOW);
        }
    }
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

String get_color() {

    // Read Red
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red = pulseIn(sensorOut, LOW);

    // Read Green
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    green = pulseIn(sensorOut, LOW);

    // Read Blue
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blue = pulseIn(sensorOut, LOW);

    Serial.print("Red: ");
    Serial.print(red);
    Serial.print("  Green: ");
    Serial.print(green);
    Serial.print("  Blue: ");
    Serial.println(blue);

    String color = identifyColor(red, green, blue);
    return color;

    delay(200);

}

String identifyColor(int r, int g, int b) {
    if (r < g && r < b) {
        return "RED";
    } 
    else if (g < r && g < b) {
        return "GREEN";
    } 
    else if (b < r && b < g) {
        return "BLUE";
    } 
    else if (r > 200 && g > 200 && b > 200) {
        return "WHITE";
    } 
    else if (r < 50 && g < 50 && b < 50) {
        return "BLACK";
    } 
    else {
        return "UNKNOWN";
    }