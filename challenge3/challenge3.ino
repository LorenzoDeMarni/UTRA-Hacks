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
float estimatedSpeed = 5.0; // Assume robot moves ~5 cm per second
float estimatedDistance = 0;
unsigned long lastMoveTime = 0;

// Time-based region tracking
unsigned long lastGreenExitTime = 0;
unsigned long lastBlueExitTime = 0;
bool firstGreenDetected = false;
bool firstBlueDetected = false;

// Color sequence
const char* colorSequence[] = {"Red", "Green", "Blue", "Green", "Blue"};
int currentColorIndex = 0;

void setup() {
    // Motor setup
    pinMode(EN_A, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(EN_B, OUTPUT);

    // Color sensor setup
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    // Ultrasonic sensor setup
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Serial.begin(9600);
    Serial.println("System Ready.");

    // Color sensor settings
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

// ========== COLOR DETECTION & REGION TRACKING ==========
void checkColorSequence() {
    String detectedColor = get_color();
    unsigned long currentTime = millis();

    if (detectedColor == "Red") return;  // Always ignore after first Red

    if (detectedColor == "Green") {
        if (!firstGreenDetected) {  
            firstGreenDetected = true;
            lastGreenExitTime = currentTime;
        } else {
            unsigned long timeSinceLastGreen = currentTime - lastGreenExitTime;
            float distanceSinceLastGreen = estimatedDistance;

            if (timeSinceLastGreen > 5000 && distanceSinceLastGreen > 20) {  
                digitalWrite(LED_PIN, HIGH);
                delay(500);
                digitalWrite(LED_PIN, LOW);
                lastGreenExitTime = currentTime;
                estimatedDistance = 0;  
                currentColorIndex++;
            }
        }
    } 

    if (detectedColor == "Blue") {
        if (!firstBlueDetected) {  
            firstBlueDetected = true;
            lastBlueExitTime = currentTime;
        } else {
            unsigned long timeSinceLastBlue = currentTime - lastBlueExitTime;
            float distanceSinceLastBlue = estimatedDistance;

            if (timeSinceLastBlue > 5000 && distanceSinceLastBlue > 20) {  
                digitalWrite(LED_PIN, HIGH);
                delay(500);
                digitalWrite(LED_PIN, LOW);
                lastBlueExitTime = currentTime;
                estimatedDistance = 0;  
                currentColorIndex++;
            }
        }
    }
}

// ========== COLOR SENSOR FUNCTIONS ==========
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

// ========== ULTRASONIC SENSOR (OBSTACLE AVOIDANCE) ==========
void avoidWalls() {
    long duration;
    float distance = 0.0;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration * 0.343) / 2; 

    Serial.print("Distance to Wall: ");
    Serial.print(distance);
    Serial.println(" mm");

    if (distance > 0 && distance <= WALL_DISTANCE_THRESHOLD) {
       Serial.println("STOP! Obstacle ahead!");
       stopMotors();
       delay(500);
       turnRight(1000);
    }
}

void turnRight(int duration) {
    analogWrite(EN_A, motorSpeed);
    analogWrite(EN_B, motorSpeed);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(duration);
    stopMotors();
}
