// ========== COLOR SENSOR PIN DEFINITIONS ==========
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 12  // Output pin for frequency signal

int red = 0, green = 0, blue = 0;

// ========== COLOR DETECTION FILTER ==========
#define QUEUE_SIZE 7
String colorQueue[QUEUE_SIZE];  // Store last 7 detected colors
int colorIndex = 0;  // Index for circular queue

// ========== SETUP FUNCTION ==========
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

    // Initialize color queue with "BLACK"
    for (int i = 0; i < QUEUE_SIZE; i++) {
        colorQueue[i] = "BLACK";
    }
}

// ========== MAIN LOOP ==========
void loop() {
    red = getColorReading(LOW, LOW);  // Read Red
    green = getColorReading(HIGH, HIGH);  // Read Green
    blue = getColorReading(LOW, HIGH);  // Read Blue

    // Print raw RGB values for debugging
    Serial.print("Raw RGB Values -> R: ");
    Serial.print(red);
    Serial.print(" G: ");
    Serial.print(green);
    Serial.print(" B: ");
    Serial.println(blue);

    String detectedColor = identifyColor(red, green, blue);
    
    // Add to circular queue
    colorQueue[colorIndex] = detectedColor;
    colorIndex = (colorIndex + 1) % QUEUE_SIZE;  // Move to next index

    // Get most frequent color from queue
    String stableColor = getStableColor();

    // Print filtered stable color
    Serial.print("Stable Detected Color: ");
    Serial.println(stableColor);

    delay(100);  // Wait before next reading
}

// ========== READ COLOR SENSOR OUTPUT ==========
int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(25);  // Allow sensor to settle
    return pulseIn(sensorOut, LOW);  // Measure pulse duration
}

// ========== COLOR CLASSIFICATION ==========
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
    if (r > g + 50 && r > b + 50) return "RED";
    if (g > r + 50 && g > b + 50) return "GREEN";
    if (b > r + 50 && b > g + 50) return "BLUE";

    return "BLACK";  // Default to BLACK if no dominant color
}

// ========== STABILIZING COLOR DETECTION ==========
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
    
    if (redCount > majorityThreshold) return "RED";
    if (greenCount > majorityThreshold) return "GREEN";
    if (blueCount > majorityThreshold) return "BLUE";
    return "BLACK";  // Default to BLACK if no majority
}
