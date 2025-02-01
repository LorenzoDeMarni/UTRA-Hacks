const int EN_A = 11; //Enable pin for first motor
const int IN1 = 9; //control pin for first motor
const int IN2 = 8; //control pin for first motor
const int IN3 = 7; //control pin for second motor
const int IN4 = 6; //control pin for second motor
const int EN_B = 10; //Enable pin for second motor
# https://www.instructables.com/Running-DC-Motor-With-Arduino-Using-L298N-Motor-Dr/

// change the pin values for light sensor
#define S0 2;
#define S1 3;
#define S2 4;
#define S3 5;
#define sensorOut 8;

int red = 0, green = 0, blue = 0;

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
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    Serial.begin(9600);
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
