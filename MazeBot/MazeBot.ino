// Motor Driver (L298)
const int enA = 11;
const int in1 = 6;
const int in2 = 7;
const int enB = 10;
const int in3 = 8;
const int in4 = 9;
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 12

int red = 0, green = 0, blue =0;

#define QUEUE_SIZE 5
String colorQueue[QUEUE_SIZE];  // Store last 5 colors
int colorIndex = 0; //Index for circular queue

// Sonar Sensor (HC-SR04)
const int trigPin = 12;
const int echoPin = 11;

// Function to move forward
void moveForward(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, (speed+5));
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

// Function to move backward
void moveBackward(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, (speed+5));
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

// Function to turn left
void turnLeft(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, (speed+5));
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(700); // Adjust turning time
}

// Function to turn right
void turnRight(int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, (speed+5));
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
    return duration * 0.0343 / 2; // Convert to cm
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
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

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

    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
      for (int i = 0; i < QUEUE_SIZE; i++) {
        colorQueue[i] = "BLACK";
    }

}
//ChatGPT find Mode of String Array
String findMode(String arr[]) {
    int maxCount = 0;
    String modeValue = "";

    for (int i = 0; i < 5; i++) {
        int count = 0;

        // Count occurrences of arr[i]
        for (int j = 0; j < 5; j++) {
            if (arr[i] == arr[j]) {
                count++;
            }
        }

        // Update mode if a higher count is found
        if (count > maxCount) {
            maxCount = count;
            modeValue = arr[i];
        }
    }

    return modeValue;
}

void loop() {
    long distance = getDistance();
      // Add to circular queue
    colorQueue[colorIndex] = detectedColor;
    colorIndex = (colorIndex + 1) % QUEUE_SIZE;  // Move to next index

    // Get most frequent color from queue
    String stableColor = findMode(colorQueue);

    // Print filtered color
    Serial.print("Stable Detected Color: ");
    Serial.println(stableColor);

    delay(100);  // Wait before next reading
    if (distance > 10) {
        Serial.println("Moving forward.");
        moveForward(180);
    } 
    else {
        Serial.println("Wall detected! Stopping.");
        stopRobot();
        delay(500);
        
        // Read color after stopping
        // array with 5 items
        
        for(int i=0;i<5;i++){
          red = getColorReading(LOW, LOW); // Read Red
          green = getColorReading(HIGH, HIGH); // Read Green
          blue = getColorReading(LOW, HIGH); // Read Blue
    
          String detectedColor = detectColor(red, green, blue);
          colorQueue[i]=detectedColor
        }

        String color = findMode();
        Serial.print("Detected Tile Color: ");
        Serial.println(color);



        // Decision-making based on color
        if (color == "Red") {
            Serial.println("U-Turn");
            turnLeft(180);
        }
        else if (color == "Blue") {
            Serial.println("Turn Left");
            turnRight(180);
        }
        else if (color == "Green") {
            Serial.println("Turn Right");
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