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

int red = 0, green = 0, blue = 0;

// Sonar Sensor (HC-SR04)
const int trigPin = 12;
const int echoPin = 11;

#define SAMPLE_SIZE 10  // Number of samples for color detection
String colorSamples[SAMPLE_SIZE];  // Store 5 color readings

// Function to move forward
void moveForward(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, speed+5);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

// Function to move backward
void moveBackward(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, speed+5);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

// Function to turn left
void turnLeft(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, speed+5);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(700);  // Adjust turn duration
}

// Function to turn right
void turnRight(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, speed+5);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(700);  // Adjust turn duration
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
    return duration * 0.0343 / 2; // Convert to cm
}

// Function to get color reading
int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(75);  // Allow sensor to settle
    return pulseIn(sensorOut, LOW);  // Measure pulse duration
}

// Function to detect color using the TCS3200 sensor
String detectColor() {
    red = getColorReading(LOW, LOW);    // Read Red
    blue = getColorReading(HIGH, HIGH); // Read Blue
    green = getColorReading(LOW, HIGH); // Read Green

    if (red < blue && red < green) return "Red";
    if (blue < red && blue < green) return "Blue";
    if (green < red && green < blue) return "Green";

    return "Black";  // Default to "Black" if no color is dominant
}

// Function to find the mode (most frequent color) in an array
String findMode(String arr[], int size) {
    int maxCount = 0;
    String modeValue = "Black";  // Default to "Black"

    for (int i = 0; i < size; i++) {
        int count = 0;
        for (int j = 0; j < size; j++) {
            if (arr[i] == arr[j]) {
                count++;
            }
        }
        if (count > maxCount) {
            maxCount = count;
            modeValue = arr[i];
        }
    }

    return modeValue;
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
    long distance = getDistance();

 // Print raw RGB values for debugging
    Serial.print("Raw RGB Values -> R: ");
    Serial.print(red);
    Serial.print(" G: ");
    Serial.print(green);
    Serial.print(" B: ");
    Serial.println(blue);


    if (distance > 10) {
        Serial.println("Moving forward.");
        moveForward(145);
    } 
    else {
        Serial.println("Wall detected! Stopping.");
        stopRobot();
        delay(500);

        // **Take 5 color samples and store them**
        for (int i = 0; i < SAMPLE_SIZE; i++) {
            colorSamples[i] = detectColor();
            delay(100);  // Small delay between readings
        }

        // **Find the most common color**
        String detectedColor = findMode(colorSamples, SAMPLE_SIZE);

        // **Print detected color**
        Serial.print("Detected Tile Color: ");
        Serial.println(detectedColor);

        // **Decision-making based on color**
        if (detectedColor == "Red") {
            Serial.println("U-Turn");
            turnLeft(145);
            turnLeft(145);  // Ensures full U-Turn
        }
        else if (detectedColor == "Blue") {
            Serial.println("Turning Left");
            turnLeft(145);
        }
        else if (detectedColor == "Green") {
            Serial.println("Turning Right");
            turnRight(145);
        }
        else {
            Serial.println("Black detected, stopping.");
            stopRobot();
        }
    }

    delay(100);
}