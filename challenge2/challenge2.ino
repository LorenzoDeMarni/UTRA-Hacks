// Motor Driver Pin Definitions
#define EN_A 11  // PWM speed control for right
#define motor1Pin1 6   // Control pin for second motor
#define motor1Pin2 7   // Control pin for second motor
//left motor
#define motor2Pin1 8   // Control pin for first motor
#define motor2Pin2 9   // Control pin for first motor
//right method
#define EN_B 10  // PWM speed control for left

// Motor speed (adjustable between 0-255)
int motorSpeed = 150;  // 150 is about 60% speed

# https://www.instructables.com/Running-DC-Motor-With-Arduino-Using-L298N-Motor-Dr/

// change the pin values for light sensor
#define S0 2;
#define S1 3;
#define S2 4;
#define S3 5;
#define sensorOut 8;

int red = 0, green = 0, blue = 0;

//ultrasonic sensor
#define TRIG_PIN 0
#define ECHO_PIN 1
#define WALL_DISTANCE_THRESHOLD 100

boolean black_second_time = True;

//seperate value for turning 90 degrees (to be changed)
#define turn_value 100
void color_setup() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    Serial.begin(9600);
}

void setup() {
    // Set motor control pins as outputs
    pinMode(EN_A, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(EN_B, OUTPUT);

    Serial.begin(9600);
    Serial.println("Motor System Initialized");
    
    //setup for sensor
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(9600);
}

void loop() {

    long duration;
    float distance = 0.0;

// constantly move forward until we hit a wall
    while (detect_wall(distance) != True) {
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
        delay(10);
    }
    stopMotors();

    String current_color = get_color();

    if (current_color == "RED") {
        if (black_second_time == True) {
            black_second_time = False;
        }
        turnRight();
        turnRight();
        loop();
    }

    else if (current_color == "BLUE") {
        if (black_second_time == True) {
            black_second_time = False;
        }
        turnLeft();
        loop();
    }

    else if (current_color == "GREEN") {
        if (black_second_time == True) {
            black_second_time = False;
        }
        turnRight();
        loop();
    }

    else if (current_color == "BLACK" && black_second_time == False) {
        stopMotors();
        break;
    }

}

void moveForward() {
    Serial.println("Moving Forward...");
    analogWrite(EN_A, motorSpeed); // Set speed
    analogWrite(EN_B, motorSpeed);
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}

void turnLeft() {
    Serial.println("Turning Left...");
    analogWrite(EN_A, motorSpeed); // Set speed
    analogWrite(EN_B, motorSpeed);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    delay(turn_value);
    stopMotors();
}

void turnRight() {
    Serial.println("Turning Right...");
    analogWrite(EN_A, motorSpeed); // Set speed
    analogWrite(EN_B, motorSpeed);
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

String get_color() {

    // Read Red
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red = pulseIn(sensorOut, LOW);

    // Read Green
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    green = pulseIn(sensorOut, LOW);

    // Read Blue
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blue = pulseIn(sensorOut, LOW);

    Serial.print("Red: ");
    Serial.print(red);
    Serial.print("  Green: ");
    Serial.print(green);
    Serial.print("  Blue: ");
    Serial.println(blue);

    String color = identifyColor(red, green, blue);
    return color;

    delay(200);

}

String identifyColor(int r, int g, int b) {
    if (r < g && r < b) {
        return "RED";
    } 
    else if (g < r && g < b) {
        return "GREEN";
    } 
    else if (b < r && b < g) {
        return "BLUE";
    } 
    else if (r > 200 && g > 200 && b > 200) {
        return "WHITE";
    } 
    else if (r < 50 && g < 50 && b < 50) {
        return "BLACK";
    } 
    else {
        return "UNKNOWN";
    }

boolean detect_wall(int distance) {
     if (distance > 0 && distance <= WALL_DISTANCE_THRESHOLD) {
        return True;
    } 
    else {
       return False;
    }
}