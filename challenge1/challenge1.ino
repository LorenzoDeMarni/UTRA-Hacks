// Motor Driver (L298)
const int enA = 11;
const int in1 = 6;
const int in2 = 7;
const int enB = 10;
const int in3 = 8;
const int in4 = 9;

// Color Sensor (TCS3200)
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 12

// Sonar Sensor (HC-SR04) - Optional for obstacle detection
const int trigPin = 12;
const int echoPin = 11;

int red = 0, green = 0, blue = 0;
unsigned long travelTime = 0;

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

// Function to turn left (90 degrees)
void turnLeft() {
    analogWrite(enA, 150);
    analogWrite(enB, 150);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(600); // Adjust for precise 90-degree turn
}

// Function to turn right (90 degrees)
void turnRight() {
    analogWrite(enA, 150);
    analogWrite(enB, 150);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(600); // Adjust for precise 90-degree turn
}

// Function to stop
void stopRobot() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

// Function to get color reading
int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(50); // Allow sensor to settle
    return pulseIn(sensorOut, LOW);
}

// Function to detect color using the TCS3200 sensor
String detectColor() {
    red = getColorReading(LOW, LOW);    // Read Red
    blue = getColorReading(HIGH, HIGH); // Read Blue
    green = getColorReading(LOW, HIGH); // Read Green

    if (red < blue && red < green) return "Red";
    if (blue < red && blue < green) return "Blue";
    if (green < red && green < blue) return "Green";

    return "Black"; // Default to "Black" if no color is dominant
}

void setup() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    Serial.begin(9600);

    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
}

void loop() {
    String startColor = detectColor();
    Serial.print("Starting color: ");
    Serial.println(startColor);

    if (startColor != "Black") {
        Serial.println("Not starting on Black! Stopping.");
        stopRobot();
        while (true); // Stop execution if not starting on black
    }

    // **Step 1: Move Forward Until First Color Change**
    Serial.println("Moving forward to detect color change...");
    moveForward(180);
    unsigned long startTime = millis();

    while (detectColor() == "Black") {
        // Wait until the robot moves onto a color
    }

    unsigned long colorChangeTime = millis();
    Serial.println("Color detected! Timing started.");

    // **Step 2: Continue Moving Until Back to Black**
    while (detectColor() != "Black") {
        // Keep moving forward
    }

    travelTime = millis() - colorChangeTime; // Time taken to cross bullseye
    Serial.print("Time to cross bullseye: ");
    Serial.println(travelTime);

    stopRobot();
    delay(500);

    // **Step 3: Move Back to Approximate Center**
    Serial.println("Moving backward to estimated center...");
    moveBackward(180);
    delay(travelTime / 2); // Move back half of the travel time

    stopRobot();
    delay(500);

    // **Step 4: Turn 90 Degrees Perpendicular**
    Serial.println("Turning 90 degrees to cross the bullseye perpendicular...");
    turnLeft();  // Adjust if needed for proper orientation

    // **Step 5: Move Forward Again to Verify Center Alignment**
    Serial.println("Moving forward again to verify center alignment...");
    moveForward(180);
    delay(travelTime / 2);  // Move back to approximate center

    stopRobot();
    Serial.println("Reached estimated center of bullseye!");

    // **Step 6: Drop Flag Here (If Applicable)**
    Serial.println("Placing flag at center!");
    while (true); // Stop execution after flag placement
}
