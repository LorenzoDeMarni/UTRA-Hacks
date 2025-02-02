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
int mappedX[] = {0, 0, 0, 0, 0, 0, 0};
int mappedY[] = {0, 0, 0, 0, 0, 0, 0};
int map_index = 0;
int x = 0;
int y = 0;
int dir = 1; // 1 = +y, 2 = +x, 3 = -y, 4 = -x

// ========== LED ==========
#define LED_PIN A2
#define LED_EXTRA_PIN A0

// ========== COLOR DETECTION FILTER ==========
#define QUEUE_SIZE 7
String colorQueue[QUEUE_SIZE];  // Store last 7 detected colors
int colorIndex = 0;

// ========== DEBOUNCE TIMER ==========
unsigned long lastColorTime = 0;

// ========== SETUP FUNCTION ==========
void setup() {
    // Setting random seed
    randomSeed(analogRead(0));
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

    // LED Setup
    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_EXTRA_PIN, OUTPUT);

    // Color Sensor Setup
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    Serial.println("Color Sensor Initialized");
    digitalWrite(LED_EXTRA_PIN, HIGH);

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
  moveForward();
  if (dir == 1) {
    y += 1;
  }
  else if (dir == 2) {
    x += 1;
  }
  else if (dir == 3) {
    y -= 1;
  }
  else {
    x -= 1;
  }
  distance = getWallDistance();

  // Read and process color using the detection system
  red = getColorReading(LOW, LOW);
  green = getColorReading(HIGH, HIGH);
  blue = getColorReading(LOW, HIGH);
  String detectedColor = identifyColor(red, green, blue);

  // Add to circular queue
  colorQueue[colorIndex] = detectedColor;
  colorIndex = (colorIndex + 1) % QUEUE_SIZE;

  // Print detected color
  Serial.print("Detected Color: ");
  Serial.println(detectedColor);

  // Ignore if not in sequence
  if (detectedColor != colorSequence[currentColorIndex]) {
    Serial.print("❌ Ignoring out-of-sequence color: ");
    Serial.println(detectedColor);
    
  }
  else {
    int cur = 0;
    int t = 1;
    while(cur < 7) {
      if ((mappedX[cur] + 1) > x && (mappedX[cur] - 1) < x) {
        if ((mappedY[cur] + 1) > y && (mappedY[cur] - 1) < y) {
          t = 0;
        }
      }
      cur ++;
    }
    if (t == 1) {
      Serial.print("✅ Correct color detected: ");
      Serial.println(detectedColor);
      blinkLED();
      currentColorIndex += 1;
      mappedX[map_index] = x;
      mappedY[map_index] = y;
      map_index += 1;
    }
  }

  // Check if we have detected the last BLUE
  if (currentColorIndex == 5) {
    Serial.println("🚀 Sequence Completed! Stopping Robot.");
    stopMotors();
    sequenceCompleted = true;
    while(true);
  }

  stopMotors();
  if (detect_wall(distance)) {
    int random = rand() % 2;
    if (random == 1) {
      turnRight(820);
      int distRand = getWallDistance();
      if(detect_wall(distance)) {
        turnRight(1640);
        if(detect_wall(distance)) { turnRight(820); }
      }
    }
    else if (random == 0) {
      turnLeft(820);
      int distRand = getWallDistance();
      if(detect_wall(distance)) {
        turnLeft(1640);
        if(detect_wall(distance)) { turnLeft(820); }
      }
    }
  }
  else if ((rand() % 20) == 1) {
    turnLeft(820);
    if (!detect_wall(distance)) {
      moveForward();
    }
    else {
      turnRight(820);
    }
  } 
  else if ((rand() % 20) == 2) {
    turnRight(820);
    if (!detect_wall(distance)) {
      moveForward();
    }
    else {
      turnLeft(820);
    }
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

void turnRight(int duration) {
  Serial.println("turning to the right");
  analogWrite(EN_A, motorSpeedLeft);
  analogWrite(EN_B, motorSpeedRight);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(duration);
  stopMotors();
  dir += 1;
  if (dir == 5) {
    dir = 1;
  }
}

void turnLeft(int duration) {
  Serial.println("turning to the left");
  analogWrite(EN_A, motorSpeedLeft);
  analogWrite(EN_B, motorSpeedRight);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(duration);
  stopMotors();
  dir -= 1;
  if (dir == 0) {
    dir = 4;
  }
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
    // Serial.println(b);

    // // Check for BLACK (all values are low)
    // if (r > 300 && g > 300 && b > 300) return "BLACK";

    // Check for dominant color
    if (r < g - 50 && r < b - 50) {
      Serial.print("r");
      return "RED";
    }
    if (g < r - 15 && g < b - 15) {
      Serial.print("G");
      return "GREEN";
    }
    if (b < r - 25 && b < g - 25) {
      Serial.print("B");
      return "BLUE";
    }

    return "BLACK";  // Default to BLACK if no dominant color
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
  stopMotors();
  Serial.println("💡 LED Blinking!");
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  moveForward();
}
