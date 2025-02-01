// Motor Driver Pin Definitions
#define EN_A 11  // PWM speed control for motor 1
#define motor1Pin1 6   // Control pin for second motor
#define motor1Pin2 7   // Control pin for second motor
#define motor2Pin1 8   // Control pin for first motor
#define motor2Pin2 9   // Control pin for first motor
#define EN_B 10  // PWM speed control for motor 2

// Motor speed (adjustable between 0-255)
int motorSpeed = 150;  // 150 is about 60% speed

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
    
}

void loop() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        if (command == 'F') { // Move Forward
            digitalWrite(motor1Pin1, HIGH);
            digitalWrite(motor1Pin2, LOW);
            digitalWrite(motor2Pin1, HIGH);
            digitalWrite(motor2Pin2, LOW);
        } else if (command == 'S') { // Stop
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, LOW);
            digitalWrite(motor2Pin1, LOW);
            digitalWrite(motor2Pin2, LOW);
        }
    }
    
    // figure out where first colour is and start timer
    // keep going till hit black and stop timer
    // 180* turn, go halfway
    // 90* turn right and go straight till hit black
    // 180* turn , go till hit black
    // 180* turn, and go halfway
    // plant flag including offset
}

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

String get_color() {

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


