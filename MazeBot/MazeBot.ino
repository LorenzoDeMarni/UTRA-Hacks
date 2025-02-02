// Motor Driver (L298)
const int enA = 11;
const int in1 = 6;
const int in2 = 7;
const int enB = 10;
const int in3 = 8;
const int in4 = 9;

// Sonar Sensor (HC-SR04)
//const int trigPin = 12;
//const int echoPin = 11;

// RGB Sensor (TCS3200)
//const int S2 = 4;
//const int S3 = 3;
//const int sensorOut = 2;

// Function to move forward
void moveForward(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, speed);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

// Function to move backward
void moveBackward(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, speed);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

// Function to turn left
void turnLeft(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, speed);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(700); // Adjust turning time
}

// Function to turn right
void turnRight(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, speed);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(700); // Adjust turning time
}

// Function to stop
void stopRobot() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

// Function to measure distance (HC-SR04)
long getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2; // Convert to cm
}

// Function to read color (TCS3200)
String detectColor() {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    int red = pulseIn(sensorOut, LOW);

    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    int blue = pulseIn(sensorOut, LOW);

    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    int green = pulseIn(sensorOut, LOW);

    if (red < blue && red < green) return "Red";
    if (blue < red && blue < green) return "Blue";
    if (green < red && green < blue) return "Green";

    return "Unknown";
}

void setup() {
    Serial.begin(9600);

    // Set motor pins as outputs
    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    // Set sonar sensor pins
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Set RGB sensor pins
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);
}

void loop() {
    long distance = getDistance();
    
    if (distance > 10) {
        Serial.println("Moving forward.");
        moveForward(180);
    } 
    else {
        Serial.println("Wall detected! Stopping.");
        stopRobot();
        delay(500);
        
        // Read color after stopping
        String color = detectColor();
        Serial.print("Detected Tile Color: ");
        Serial.println(color);

        // Decision-making based on color
        if (color == "Red") {
            Serial.println("Turning left.");
            turnLeft(180);
        }
        else if (color == "Blue") {
            Serial.println("Turning right.");
            turnRight(180);
        }
        else if (color == "Green") {
            Serial.println("Reversing.");
            moveBackward(180);
            delay(700);  // Reverse time
        }
        else {
            Serial.println("Unknown tile! Stopping.");
            stopRobot();
        }
    }

    delay(100);
}