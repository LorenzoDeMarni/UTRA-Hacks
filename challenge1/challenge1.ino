#include <Servo.h>  // Include Servo library

#define SERVO_PIN A3  // Servo connected to Analog Pin A3

Servo myServo;  // Create Servo object

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
#define S0 2;
#define S1 3;
#define S2 5;
#define S3 4;
#define sensorOut 12;

int red = 0, green = 0, blue = 0;

#define turn_value 820

#define longerRuntime 0
unsigned long startTime = 0; // Stores the start time
unsigned long stopTime = 0;  // Stores the stop time
unsigned long elapsedTime = 0; //total elapsed time
bool timerRunning = false;

void setup() {
    // Set motor control pins as outputs
    pinMode(EN_A, OUTPUT);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(EN_B, OUTPUT);

    //Set Color sensor pins
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    myServo.attach(SERVO_PIN);  // Attach servo to pin A3
   
    //calling to pick up flag at the start
    pickUpFlag();
    
    startUp(); //move into the first ring...
    Serial.begin(9600);
    
}

void loop() {    
    
    red = getColorReading(LOW, LOW); // Read Red
    green = getColorReading(HIGH, HIGH); // Read Green
    blue = getColorReading(LOW, HIGH); // Read Blue

    String colorNew = identifyColor(red, green, blue);

    while (colorNew != "BLACK") { //keeps going forward till we detect black (not a color)
        moveForward();
        startTime = millis();  //we wanna track the time it takes to reach start to 'black' point
        timerRunning = true;
        Serial.println("Timer Started!");
    }

    stopTime = millis();
    timerRunning = false;
    elapsedTime = stopTime - startTime; //save travel time to a variable

    if (elapsedTime > longerRuntime) {
        longerRuntime = elapsedTime
        turnLeft(); //need to make this a sharper turn with a parameter
        loop();
    }
    //we want this if statement to check if the run time was longer than before. This will let us turn
    //on a steeper angle (want to get to at least 180 90 is default)

    else {
        turnLeft();
        turnLeft(); //do a 180
        moveForward(); //make this parameter elapsedTime / 2 (midway through circle)
        dropFlag();
        while (true);
    }
    //when we reach desired state, we can return on the same line traveled and drop the flag half way
    //which should be the center

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

void pickUpFlag() {
    myServo.write(100);  //open
    delay(2000);  // wait 2 seconds to line up flag

    myServo.write(150);  //close, will hold flag
}

void dropFlag() {
    myServo.write(100); //opnes claws
}

//Starter 
void startUp() {

    red = getColorReading(LOW, LOW); // Read Red
    green = getColorReading(HIGH, HIGH); // Read Green
    blue = getColorReading(LOW, HIGH); // Read Blue

    String color = identifyColor(red, green, blue);

    while(color != "BLACK") {
        moveForward();
        startTime = millis();
        timerRunning = true;
        Serial.println("Timer Started!");
    }

    stopTime = millis();
    timerRunning = false;
    longerRuntime = stopTime - startTime;

    turnLeft();
    String newColor = identifyColor(red, green, blue);

    while(newColor == color || newColor == "BLACK") {
        turnLeft();
        delay(1000);
    }
    //we wanna keep spinning until we find a color that isnt the same as the ring we are on or black
    //same logic as the loop function
    stopMotors();
}