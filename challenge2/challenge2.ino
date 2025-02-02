// Motor Driver Pin Definitions
#define motor1Pin1 6   // Control pin for second motor
#define motor1Pin2 7   // Control pin for second motor
//right motor

#define motor2Pin1 8   // Control pin for first motor
#define motor2Pin2 9   // Control pin for first motor
//left motor

#define EN_B 10  // PWM speed control for left
#define EN_A 11  // PWM speed control for right

// Motor speed (adjustable between 0-255)
int motorSpeedLeft = 145;  
int motorSpeedRight = 150;

// change the pin values for light sensor
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 12

int red = 0, green = 0, blue = 0;

//ultrasonic sensor
#define TRIG_PIN A4
#define ECHO_PIN A5
#define WALL_DISTANCE_THRESHOLD 200

boolean black_second_time = True;

// To insure accuracy, tracks the color 5 times, see if same color appears 5 times
#define QUEUE_SIZE 5
String colorQueue[QUEUE_SIZE];  // Store last 5 colors
int colorIndex = 0;  // Index for circular queue

//seperate value for turning 90 degrees (to be changed)
#define turn_value 700


void setup() {
    // Set motor control pins as outputs
    pinMode(EN_A, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(EN_B, OUTPUT);

    Serial.println("Motor System Initialized");
    
    //setup for ultra sonic sensor
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Serial.println("Ultra sonic sensor Initialized");

    //setup for color sensor
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);

    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    Serial.println("Color sensir Initialized");

    Serial.begin(9600);
}


void loop() {

    long duration;
    float distance = 0.0;

    red = getColorReading(LOW, LOW); // Read Red
    green = getColorReading(HIGH, HIGH); // Read Green
    blue = getColorReading(LOW, HIGH); // Read Blue

// constantly move forward until we hit a wall
    while (detect_wall(distance) != true) {
        // Send a 10-microsecond HIGH pulse to trigger the sensor
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);
        moveForward();

        // Measure the time it takes for the echo to return
        duration = pulseIn(ECHO_PIN, HIGH);

        // Convert the time to distance (in mm)
        distance = (duration * 0.343) / 2; 
        delay(400);
    }
    stopMotors();

    String detectedColor = identifyColor(red, green, blue);
    
    // Add to circular queue
    colorQueue[colorIndex] = detectedColor;
    colorIndex = (colorIndex + 1) % QUEUE_SIZE;  // Move to next index

    // Get most frequent color from queue
    String stableColor = getStableColor();

    if (stableColor == "RED") {
        if (black_second_time == True) {
            black_second_time = False;
        }
        turnRight();
        turnRight();
        loop();
    }

    else if (stableColor == "BLUE") {
        if (black_second_time == True) {
            black_second_time = False;
        }
        turnLeft();
        loop();
    }

    else if (stableColor == "GREEN") {
        if (black_second_time == True) {
            black_second_time = False;
        }
        turnRight();
        loop();
    }

    else if (stableColor == "BLACK" && black_second_time == False) {
        stopMotors();
        while (true);
    }

}

void moveForward() {
    Serial.println("Moving Forward...");
    analogWrite(EN_A, motorSpeedLeft); // Set speed
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}

void turnLeft() {
    Serial.println("Turning Left...");
    analogWrite(EN_A, motorSpeedLeft); // Set speed
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(turn_value);
    stopMotors();
}

void turnRight() {
    Serial.println("Turning Right...");
    analogWrite(EN_A, motorSpeedLeft); // Set speed
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(turn_value);
    stopMotors();
}

void stopMotors() {
    Serial.println("Stopping Motors...");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
}

String identifyColor(int r, int g, int b) {
    if (r < g - 15 && r < b - 15) return "RED";
    else if (g < r - 15 && g < b - 15) return "GREEN";
    else if (b < r - 15 && b < g - 15) return "BLUE";
    else return "BLACK";  // Default to BLACK if not RED, GREEN, or BLUE
}

int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(100);  // Allow sensor to settle
    return pulseIn(sensorOut, LOW);  // Measure pulse duration
}

String getStableColor() {
    int redCount = 0, greenCount = 0, blueCount = 0, blackCount = 0;

    // Count occurrences of each color
    for (int i = 0; i < QUEUE_SIZE; i++) {
        if (colorQueue[i] == "RED") redCount++;
        else if (colorQueue[i] == "GREEN") greenCount++;
        else if (colorQueue[i] == "BLUE") blueCount++;
        else if (colorQueue[i] == "BLACK") blackCount++;
    }
    // Return the most frequent color
    if (redCount >= greenCount && redCount >= blueCount && redCount >= blackCount) return "RED";
    if (greenCount >= redCount && greenCount >= blueCount && greenCount >= blackCount) return "GREEN";
    if (blueCount >= redCount && blueCount >= greenCount && blueCount >= blackCount) return "BLUE";
    return "BLACK";  // Default to BLACK if no majority
}

boolean detect_wall(int distance) {
     return (distance > 0 && distance <= WALL_DISTANCE_THRESHOLD);
}