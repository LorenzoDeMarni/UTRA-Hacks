// Motor driver pins
const int EN_A = 11; // Right motor speed control
const int IN1 = 9;   // Right motor forward
const int IN2 = 8;   // Right motor backward
const int IN3 = 7;   // Left motor forward
const int IN4 = 6;   // Left motor backward
const int EN_B = 10; // Left motor speed control

// Color sensor pins
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 12

// LED for feedback
#define LED 13

// Color sequence: Red -> Green -> Blue -> Green -> Blue
const int SEQUENCE_LENGTH = 5;
const char* required_sequence[SEQUENCE_LENGTH] = {"Red", "Green", "Blue", "Green", "Blue"};
int current_step = 0;
bool visited_colors[SEQUENCE_LENGTH] = {false, false, false, false, false}; // Track visited colors

void setup() {
    Serial.begin(9600);

    // Set motor pins as output
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(EN_A, OUTPUT);
    pinMode(EN_B, OUTPUT);

    // Set color sensor pins
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    // Set frequency scaling for TCS3200 color sensor
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    // LED as output
    pinMode(LED, OUTPUT);
}

void moveForward() {
    analogWrite(EN_A, 150); // Adjust speed
    analogWrite(EN_B, 150);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void stopRobot() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void blinkLED() {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
}

// Read color sensor and return detected color
String readColor() {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    int red = pulseIn(sensorOut, LOW);
    
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    int green = pulseIn(sensorOut, LOW);
    
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    int blue = pulseIn(sensorOut, LOW);
    
    if (red > green && red > blue) return "Red";
    else if (green > red && green > blue) return "Green";
    else if (blue > red && blue > green) return "Blue";
    
    return "Unknown";
}

void loop() {
    moveForward();
    String detectedColor = readColor();
    Serial.println(detectedColor);

    if (detectedColor == required_sequence[current_step]) {
        if (current_step == 3 || current_step == 4) { // Second Green or Second Blue
            if (visited_colors[current_step]) {
                Serial.println("Already visited this color. Searching for a new tile...");
                return; // Skip if already visited
            }
        }

        Serial.print("Detected Correct Color: ");
        Serial.println(detectedColor);
        blinkLED();
        visited_colors[current_step] = true;
        current_step++;

        if (current_step >= SEQUENCE_LENGTH) {
            Serial.println("Color sequence completed!");
            stopRobot();
            while (true); // Stop execution
        }
    }
    
    delay(500);
}