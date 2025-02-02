// ========== MOTOR DRIVER PIN DEFINITIONS ==========
#define EN_A 11  // PWM speed control for motor 1
#define motor1Pin1 6   // Control pin for right motor
#define motor1Pin2 7   // Control pin for right motor
#define motor2Pin1 8   // Control pin for left motor
#define motor2Pin2 9   // Control pin for left motor
#define EN_B 10  // PWM speed control for motor 2

// Motor speed (adjustable between 0-255)
int motorSpeed = 150;  // 150 is about 60% speed

// ========== COLOR SENSOR PIN DEFINITIONS ==========
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 12  // Output pin for frequency signal

int red = 0, green = 0, blue = 0;

unsigned long timeElapsed = 0;  // Variable to store time taken between black detections

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
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    Serial.begin(9600);
    Serial.println("Initializing System...");

    // Initialize color sensor settings
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    Serial.println("System Ready.");
}

void loop() {
    Serial.println("\n--- Starting Challenge ---");

    // Step 1: Move forward until first black detection
    Serial.println("Moving until first BLACK detection...");
    moveUntilColor("BLACK");

    // Step 2: Start timer and continue moving forward until next black detection
    Serial.println("Starting timer...");
    unsigned long startTime = millis();  // Start timer

    Serial.println("Moving until second BLACK detection...");
    moveUntilColor("BLACK");

    // Step 3: Save time elapsed
    timeElapsed = millis() - startTime;
    Serial.print("Time recorded: ");
    Serial.print(timeElapsed);
    Serial.println(" ms");

    // Step 4: Perform a 180-degree turn
    Serial.println("Performing 180-degree turn...");
    turn180();

    // Step 5: Move forward for half of the recorded time
    unsigned long moveTime = timeElapsed / 2;
    Serial.print("Moving forward for half the time: ");
    Serial.print(moveTime);
    Serial.println(" ms");
    moveForward(moveTime);

    // Stop motors at the end
    stopMotors();
    Serial.println("Challenge complete.");
    while (true);  // Stop loop from restarting
}

// ========== MOTOR CONTROL FUNCTIONS ==========
void moveForward(int duration) {
    Serial.println("Moving Forward...");
    analogWrite(EN_A, motorSpeed);
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
    analogWrite(EN_A, motorSpeed);
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
    analogWrite(EN_A, motorSpeed);
    analogWrite(EN_B, motorSpeed);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(duration);
    stopMotors();
}

void turn180() {
    Serial.println("Performing 180-degree turn...");
    turnRight(1600);  // Adjust time for 180-degree turn
}

void stopMotors() {
    Serial.println("Stopping Motors...");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}

// ========== COLOR SENSOR FUNCTIONS ==========
void moveUntilColor(String targetColor) {
    while (true) {
        String detectedColor = get_color();
        Serial.print("Detected Color: ");
        Serial.println(detectedColor);

        if (detectedColor == targetColor) {
            stopMotors();
            Serial.print("Target color (");
            Serial.print(targetColor);
            Serial.println(") detected! Stopping...");
            delay(500);
            break;  // Exit the loop once color is detected
        }

        moveForward(200);  // Move in small increments to ensure accuracy
    }
}

String get_color() {
    red = getColorReading(LOW, LOW);  // Read Red
    green = getColorReading(HIGH, HIGH);  // Read Green
    blue = getColorReading(LOW, HIGH);  // Read Blue

    Serial.print("Red: ");
    Serial.print(red);
    Serial.print(" | Green: ");
    Serial.print(green);
    Serial.print(" | Blue: ");
    Serial.println(blue);

    return identifyColor(red, green, blue);
}

int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(100);  // Allow sensor to settle
    return pulseIn(sensorOut, LOW);  // Measure pulse duration
}

// Function to classify colors into RED, GREEN, BLUE, or BLACK
String identifyColor(int r, int g, int b) {
    if (r < g - 15 && r < b - 15) return "RED";
    else if (g < r - 15 && g < b - 15) return "GREEN";
    else if (b < r - 15 && b < g - 15) return "BLUE";
    else return "BLACK";  // Default to BLACK if not RED, GREEN, or BLUE
}
