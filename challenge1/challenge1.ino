const int motor1Pin1 = 9;  // Motor 1 forward
const int motor1Pin2 = 10; // Motor 1 backward
const int motor2Pin1 = 5;  // Motor 2 forward
const int motor2Pin2 = 6;  // Motor 2 backward

void setup() {
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    Serial.begin(9600);
}
<<<<<<< Updated upstream
lorenzo test
=======
>>>>>>> Stashed changes
void loop() {
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
