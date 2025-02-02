// ========== MOTOR DRIVER PIN DEFINITIONS ==========
#define EN_A 11
#define motor1Pin1 6
#define motor1Pin2 7
#define motor2Pin1 8
#define motor2Pin2 9
#define EN_B 10

// ========== COLOR SENSOR PIN DEFINITIONS ==========
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 12

// ========== ULTRASONIC SENSOR ==========
#define TRIG_PIN A4
#define ECHO_PIN A5
#define WALL_DISTANCE_THRESHOLD 200  // 20 cm

// ========== GLOBAL VARIABLES ==========
int motorSpeed = 150;  
int red = 0, green = 0, blue = 0;
float estimatedSpeed = 5.0;  // Assume robot moves ~5 cm per second
float estimatedDistance = 0;
unsigned long lastMoveTime = 0;

// Region Tracking
unsigned long lastGreenExitTime = 0, lastBlueExitTime = 0;
float lastGreenExitDistance = 0, lastBlueExitDistance = 0;
bool firstRedDetected = false;
bool firstBlueDetected = false;
bool firstGreenDetected = false;

// Sequence tracking
const char* colorSequence[] = {"Red", "Blue", "Green", "Blue", "Green"};
int currentColorIndex = 0;  // Keeps track of the correct sequence

void setup() {
    pinMode(EN_A, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(EN_B, OUTPUT);

    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Serial.begin(9600);
    Serial.println("System Ready.");

    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    lastMoveTime = millis();
}

void loop() {
    moveForward();
    checkColorSequence();
    avoidWalls();
    delay(100);
}

// ========== MOVEMENT & DISTANCE TRACKING ==========
void moveForward() {
    unsigned long currentTime = millis();
    float moveTime = (currentTime - lastMoveTime) / 1000.0; // Time in seconds
    estimatedDistance += moveTime * estimatedSpeed;
    lastMoveTime = currentTime;

    Serial.print("Estimated Distance: ");
    Serial.print(estimatedDistance);
    Serial.println(" cm");

    analogWrite(EN_A, motorSpeed);
    analogWrite(EN_B, motorSpeed);
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

// ========== COLOR DETECTION & SEQUENCE TRACKING ==========
void checkColorSequence() {
    String detectedColor = get_color();
    unsigned long currentTime = millis();

    // Ignore colors that are NOT in the sequence
    if (detectedColor != colorSequence[currentColorIndex]) {
        Serial.print("❌ Ignoring color (not in sequence): ");
        Serial.println(detectedColor);
        return;
    }

    // First Red detection
    if (detectedColor == "Red") {
        if (!firstRedDetected) {
            Serial.println("✅ First Red detected! Blinking LED.");
            blinkLED();
            firstRedDetected = true;
            currentColorIndex++;  // Move to next color in sequence (Blue)
        }
        return;  // Ignore all Reds after first detection
    }

    // Blue detection (must be at index 1 or 3)
    if (detectedColor == "Blue") {
        unsigned long timeSinceLastBlue = currentTime - lastBlueExitTime;
        float distanceSinceLastBlue = estimatedDistance - lastBlueExitDistance;

        if (!firstBlueDetected) {
            firstBlueDetected = true;
            lastBlueExitTime = currentTime;
            lastBlueExitDistance = estimatedDistance;
        } else if (timeSinceLastBlue > 5000 && distanceSinceLastBlue > 20) {
            Serial.println("✅ New Blue region detected!");
            blinkLED();
            lastBlueExitTime = currentTime;
            lastBlueExitDistance = estimatedDistance;
            currentColorIndex++;
        } else {
            Serial.println("⚠️ False Negative Blue - Skipping.");
        }
    }

    // Green detection (must be at index 2 or 4)
    if (detectedColor == "Green") {
        unsigned long timeSinceLastGreen = currentTime - lastGreenExitTime;
        float distanceSinceLastGreen = estimatedDistance - lastGreenExitDistance;

        if (!firstGreenDetected) {
            firstGreenDetected = true;
            lastGreenExitTime = currentTime;
            lastGreenExitDistance = estimatedDistance;
        } else if (timeSinceLastGreen > 5000 && distanceSinceLastGreen > 20) {
            Serial.println("✅ New Green region detected!");
            blinkLED();
            lastGreenExitTime = currentTime;
            lastGreenExitDistance = estimatedDistance;
            currentColorIndex++;
        } else {
            Serial.println("⚠️ False Negative Green - Skipping.");
        }
    }
}

// ========== COLOR SENSOR FUNCTIONS ==========
void blinkLED() {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
}

String get_color() {
    red = getColorReading(LOW, LOW);
    green = getColorReading(HIGH, HIGH);
    blue = getColorReading(LOW, HIGH);
    return identifyColor(red, green, blue);
}

int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(100);
    return pulseIn(sensorOut, LOW);
}

String identifyColor(int r, int g, int b) {
    if (r < g - 15 && r < b - 15) return "RED";
    else if (g < r - 15 && g < b - 15) return "GREEN";
    else if (b < r - 15 && b < g - 15) return "BLUE";
    else return "BLACK";
}
