// Define sensor control pins
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 12  // Output pin for frequency signal

int red = 0, green = 0, blue = 0;

// Circular queue for last 5 detected colors
#define QUEUE_SIZE 5
String colorQueue[QUEUE_SIZE];  // Store last 5 colors
int colorIndex = 0;  // Index for circular queue

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

    // Initialize color queue with "BLACK" to avoid errors
    for (int i = 0; i < QUEUE_SIZE; i++) {
        colorQueue[i] = "BLACK";
    }
}

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

    String detectedColor = classifyColor(red, green, blue);  // Use formula-based classification

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

// Reads color sensor output for Red, Green, or Blue detection
int getColorReading(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(25);  // Allow sensor to settle
    return pulseIn(sensorOut, LOW);  // Measure pulse duration
}

// **Formula-Based Color Classification**
String classifyColor(int r, int g, int b) {
    int total = r + g + b;  // Total intensity

    // Prevent division by zero
    if (total == 0) return "BLACK";

    // Normalize RGB values (relative intensity)
    float R_norm = (float)r / total;
    float G_norm = (float)g / total;
    float B_norm = (float)b / total;

    // Print normalized values for debugging
    Serial.print("Normalized RGB -> R: ");
    Serial.print(R_norm, 2);
    Serial.print(" G: ");
    Serial.print(G_norm, 2);
    Serial.print(" B: ");
    Serial.println(B_norm, 2);

    // Classify based on dominant color
    if (R_norm > 0.5 && G_norm < 0.3 && B_norm < 0.3) return "RED";
    if (G_norm > 0.5 && R_norm < 0.3 && B_norm < 0.3) return "GREEN";
    if (B_norm > 0.5 && R_norm < 0.3 && G_norm < 0.3) return "BLUE";

    return "BLACK";  // Default to BLACK if no dominant color
}

// Function to find the most common color in the last 5 detections
String getStableColor() {
    int redCount = 0, greenCount = 0, blueCount = 0, blackCount = 0;

    // Count occurrences of each color
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

    // Return the most frequent color
    if (redCount >= greenCount && redCount >= blueCount && redCount >= blackCount) return "RED";
    if (greenCount >= redCount && greenCount >= blueCount && greenCount >= blackCount) return "GREEN";
    if (blueCount >= redCount && blueCount >= greenCount && blueCount >= blackCount) return "BLUE";
    return "BLACK";  // Default to BLACK if no majority
}