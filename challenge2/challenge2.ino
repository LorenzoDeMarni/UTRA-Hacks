#define SERVO_PIN A3  // Servo connected to Analog Pin A3


// ========== MOTOR DRIVER PIN DEFINITIONS ==========
#define motor1Pin1 6   // Right motor forward control
#define motor1Pin2 7   // Right motor backward control
#define motor2Pin1 8   // Left motor forward control
#define motor2Pin2 9   // Left motor backward control
#define EN_B 10  // PWM speed control for left motor
#define EN_A 11  // PWM speed control for right motor

// ========== MOTOR SPEED CONFIGURATION ==========
int motorSpeedLeft = 145;  
int motorSpeedRight = 150;

// ========== COLOR SENSOR PIN DEFINITIONS ==========
#define S0 2  // Frequency scaling pin
#define S1 3  // Frequency scaling pin
#define S2 5  // Color select pin
#define S3 4  // Color select pin
#define sensorOut 12  // Output pin for color sensor frequency signal

// SONAR SENSOR
#define trigPin A4
#define echoPin A5




int red = 0, green = 0, blue = 0;

// Circular queue for last detected colors
#define QUEUE_SIZE 7
String colorQueue[QUEUE_SIZE];  
int colorIndex = 0;

// ========== TURN AND TIMER CONFIGURATION ==========
#define turn_value 820  // Delay time for turning (adjust for accuracy)

unsigned long startTime = 0;  
unsigned long stopTime = 0;   
bool timerRunning = false;  

int sequence = 0;
String stableColor = "BLACK";


void setup() {
    delay(5000);
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
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT); 

    delay(2000);



    Serial.begin(9600);

    // Initialize color queue
    for (int i = 0; i < QUEUE_SIZE; i++) {
        colorQueue[i] = "BLACK";
    }

    delay(3000);
    Serial.println("After delay");


}

void loop() {   
    // Read color values
    long distance = getDistance();
    Serial.println(distance);
    red = getColorReading(LOW, LOW);  
    green = getColorReading(HIGH, HIGH);
    blue = getColorReading(LOW, HIGH);
    
    
     if (distance > 25) {
        Serial.println("Moving forward.");
        moveForward();
    } 
    else {
        Serial.println("Wall detected! Stopping.");
        stopMotors();
        for(int i=0;i<10;i++){
          String detectedColor = identifyColor(red, green, blue);
          colorQueue[colorIndex] = detectedColor;
          colorIndex = (colorIndex + 1) % QUEUE_SIZE;
          stableColor = getStableColor();
          
        }
        Serial.println("Stable Detected Color: ");
        Serial.println(stableColor);
        
        if(stableColor=="RED"){
          turnLeft();
          turnLeft();
          delay(1000);


        }
        else if(stableColor=="BLUE"){
          turnLeft();
          delay(1000);

        }
        else if(stableColor=="GREEN"){
          turnRight();
          delay(1000);

        }
        else if(stableColor=="BLACK"){
          stopMotors();
          Serial.println("FINISHED");
          delay(20000);

        }
    }
    delay(100);


      
    
    
    
}

// SONAR
long getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    return duration * 0.0343 / 2; // Convert to cm
}
// ========== COLOR DETECTION FUNCTIONS ==========



int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(25);
    return pulseIn(sensorOut, LOW);
}

String identifyColor(int r, int g, int b) {
    Serial.print("Processing Color -> R: ");
    Serial.print(r);
    Serial.print(" G: ");
    Serial.print(g);
    Serial.print(" B: ");
    Serial.println(b);

    // Check for BLACK (all values are low)
    if (r < 300 && g < 300 && b < 300) return "BLACK";

    // Check for dominant color
    if (r > g + 50 && r > b + 50) return "GREEN";
    if (g > r + 50 && g > b + 50) return "BLUE";
    if (b > r + 50 && b > g + 50) return "RED";

    return "BLACK";  // Default to BLACK if no dominant color
}

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

void reverseMotors(unsigned long duration) {
    Serial.println("Reversing...");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(duration);
    stopMotors();
}

void turnLeft() {
    Serial.println("Turning Left...");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(turn_value);
    stopMotors();
}

void turnRight() {
    Serial.println("Turning Left...");
    analogWrite(EN_A, motorSpeedLeft);
    analogWrite(EN_B, motorSpeedRight);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    delay(turn_value);
    stopMotors();
}