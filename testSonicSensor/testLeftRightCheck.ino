#define TRIG_PIN A4
#define ECHO_PIN A5
#define WALL_DISTANCE_THRESHOLD 200

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(9600);
}