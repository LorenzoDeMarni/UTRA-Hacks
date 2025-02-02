#define TRIG_PIN A4
#define ECHO_PIN A5
#define WALL_DISTANCE_THRESHOLD 200

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(9600);
}

void loop() {
    long duration;
    float distance = 0.0;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure the time it takes for the echo to return
    duration = pulseIn(ECHO_PIN, HIGH);

    // Convert the time to distance (in mm)
    distance = (duration * 0.343) / 2; 
    delay(400);

    // Print the distance
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");

    if (distance > 0 && distance <= WALL_DISTANCE_THRESHOLD) {
       Serial.println("STOP OMG OMG ABORT ABORT ABORT WERE GONNA DIE");
       delay(500);
    } 
    else {
       Serial.println("aye u good bro");
    }
}