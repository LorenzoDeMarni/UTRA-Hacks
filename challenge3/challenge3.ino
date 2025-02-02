// ========== MOTOR DRIVER PIN DEFINITIONS ==========
#define motor1Pin1 6   // Right Motor Control 1
#define motor1Pin2 7   // Right Motor Control 2
#define motor2Pin1 8   // Left Motor Control 1
#define motor2Pin2 9   // Left Motor Control 2
#define EN_A 11        // PWM speed control for Right
#define EN_B 10        // PWM speed control for Left

// ========== MOTOR SPEED ==========
int motorSpeedLeft = 145;  
int motorSpeedRight = 150;

// ========== COLOR SENSOR PIN DEFINITIONS ==========
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 12

int red = 0, green = 0, blue = 0;

// ========== ULTRASONIC SENSOR ==========
#define TRIG_PIN A4
#define ECHO_PIN A5
#define WALL_DISTANCE_THRESHOLD 200  // 20 cm

// ========== SEQUENCE TRACKING ==========
const String colorSequence[] = {"RED", "GREEN", "BLUE", "GREEN", "BLUE"};
int currentColorIndex = 0;  // Keeps track of expected color
bool sequenceCompleted = false;  // Flag to stop movement after last blue

// ========== COLOR DETECTION FILTER ==========
#define QUEUE_SIZE 7
String colorQueue[QUEUE_SIZE];  // Store last 7 detected colors
int colorIndex = 0;

// ========== DEBOUNCE TIMER ==========
unsigned long lastColorTime = 0;

// ========== SETUP FUNCTION ==========
void setup() {
    // Motor Setup
    pinMode(EN_A, OUTPUT);
    pinMode(EN_B, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);

    Serial.begin(9600);
    Serial.println("Motor System Initialized");

    // Ultrasonic Sensor Setup
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.println("Ultrasonic Sensor Initialized");

    // Color Sensor Setup
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    Serial.println("Color Sensor Initialized");

    // Initialize color queue with "BLACK"
    for (int i = 0; i < QUEUE_SIZE; i++) {
        colorQueue[i] = "BLACK";
    }
}

// ========== MAIN LOOP ==========
void loop() {
    if (sequenceCompleted) {
        stopMotors();
        return;  // Prevents further execution after sequence completion
    }

    long duration;
    float distance = 0.0;

    // Move forward until a wall is detected
    while (!detect_wall(distance)) {
        moveForward();
        distance = getWallDistance();
    }
    stopMotors();

    // Read and process color using the detection system
    red = getColorReading(LOW, LOW);
    green = getColorReading(HIGH, HIGH);
    blue = getColorReading(LOW, HIGH);
    String detectedColor = identifyColor(red, green, blue);

    // Add to circular queue
    colorQueue[colorIndex] = detectedColor;
    colorIndex = (colorIndex + 1) % QUEUE_SIZE;

    // Get the most stable color
    String stableColor = getStableColor();

    // Print detected color
    Serial.print("Detected Color: ");
    Serial.println(stableColor);

    // Debounce logic: Ignore colors detected too soon after the last one
    if (millis() - lastColorTime < 500) {
        Serial.println("⏳ Waiting for color to stabilize...");
        return;
    }
    lastColorTime = millis();  // Update last color detection time

    // Ignore if not in sequence
    if (stableColor != colorSequence[currentColorIndex]) {
        Serial.print("❌ Ignoring out-of-sequence color: ");
        Serial.println(stableColor);
        return;
    }

    // If the color is correct, print and move to next step in sequence
    Serial.print("✅ Correct color detected: ");
    Serial.println(stableColor);
    blinkLED();
    currentColorIndex++;
 
    // Check if we have detected the last BLUE
    if (currentColorIndex == 5) {
        Serial.println("🚀 Sequence Completed! Stopping Robot.");
        stopMotors();
        sequenceCompleted = true;
        while(true); //stop the infinite loop
    }
}

// ========== MOVEMENT FUNCTIONS ==========
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
    Serial.println("Stopping Motors...");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}

// ========== COLOR DETECTION FUNCTIONS ==========
int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(25);
    return pulseIn(sensorOut, LOW);
}

// ========== IMPROVED COLOR CLASSIFICATION ==========
String identifyColor(int r, int g, int b) {
    Serial.print("Processing Color -> R: ");
    Serial.print(r);
    Serial.print(" G: ");
    Serial.print(g);
    Serial.print(" B: ");
    Serial.println(b);

    // Check for BLACK (all values are low)
    if (r > 600 && g > 1000 && b > 1000) return "BLACK";

    // Check for dominant color
    if (r > g + 50 && r > b + 50) return "GREEN";
    if (g > r + 50 && g > b + 50) return "BLUE";
    if (b > r + 50 && b > g + 50) return "RED";

    return "BLACK";  // Default to BLACK if no dominant color
}

// ========== STABILIZING COLOR DETECTION ==========
String getStableColor() {
    int redCount = 0, greenCount = 0, blueCount = 0, blackCount = 0;

    // Count occurrences of each color in the queue
    for (int i = 0; i < QUEUE_SIZE; i++) {
        if (colorQueue[i] == "RED") redCount++;
        else if (colorQueue[i] == "GREEN") greenCount++;
        else if (colorQueue[i] == "BLUE") blueCount++;
        else if (colorQueue[i] == "BLACK") blackCount++;
    }

    // Print occurrences for debugging
    Serial.print("Color Counts -> R: ");
    Serial.print(redCount);
    Serial.print(" G: ");
    Serial.print(greenCount);
    Serial.print(" B: ");
    Serial.print(blueCount);
    Serial.print(" Black: ");
    Serial.println(blackCount);

    // Return the most frequent color (only if it appears in at least 50% of the queue)
    int majorityThreshold = QUEUE_SIZE / 2;

    if (redCount > majorityThreshold) return "GREEN";
    if (greenCount > majorityThreshold) return "BLUE";
    if (blueCount > majorityThreshold) return "RED";
    return "BLACK";  // Default to BLACK if no majority
}

// ========== WALL DETECTION FUNCTIONS ==========
boolean detect_wall(float distance) {
    return (distance > 0 && distance <= WALL_DISTANCE_THRESHOLD);
}

float getWallDistance() {
    long duration;
    float distance = 0.0;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration * 0.343) / 2;

    return distance;
}

// ========== LED FUNCTION ==========
void blinkLED() {
    Serial.println("💡 LED Blinking!");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
}
