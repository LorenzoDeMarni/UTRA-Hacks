#define TRIG_PIN A4
#define ECHO_PIN A5
#define WALL_DISTANCE_THRESHOLD 200

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(9600);
}

float measureDistance() {
    long duration;
    float distance;

    // Send a 10-microsecond pulse to trigger the sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure the time taken for the pulse to return
    duration = pulseIn(ECHO_PIN, HIGH);

    // Convert duration to distance (cm)
    distance = duration * 0.0343 / 2;

    return distance;
}

void loop() {
    float leftDistance = measureDistance();

    Serial.print("Left Distance: ");
    Serial.print(leftDistance);
    Serial.println(" cm");

    if (leftDistance > 0 && leftDistance <= WALL_THRESHOLD) {
        Serial.println("⚠️ Wall detected on the left!");
    } else {
        Serial.println("✅ Left side is clear.");
    }

    delay(500);
}