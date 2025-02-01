// Define sensor control pins
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 12  // Output pin for frequency signal

int red = 0, green = 0, blue = 0;

void setup() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    Serial.begin(9600);
    Serial.println("Initializing Color Sensor...");

    // Set frequency scaling to 20% (recommended setting)
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
}

void loop() {
    Serial.println("\n--- Color Test ---");

    red = getColorReading(LOW, LOW); // Read Red
    green = getColorReading(HIGH, HIGH); // Read Green
    blue = getColorReading(LOW, HIGH); // Read Blue

    Serial.print("Red: ");
    Serial.print(red);
    Serial.print(" | Green: ");
    Serial.print(green);
    Serial.print(" | Blue: ");
    Serial.println(blue);

    String detectedColor = identifyColor(red, green, blue);
    Serial.print("Detected Color: ");
    Serial.println(detectedColor);

    delay(500);  // Wait before next reading
}

int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(100);  // Allow sensor to settle
    return pulseIn(sensorOut, LOW);  // Measure pulse duration
}

String identifyColor(int r, int g, int b) {
    if (r < g - 15 && r < b - 15) return "RED";
    else if (g < r - 15 && g < b - 15) return "GREEN";
    else if (b < r - 15 && b < g - 15) return "BLUE";
    else return "BLACK";  // Default to BLACK if not RED, GREEN, or BLUE
}

