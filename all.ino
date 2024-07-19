const int mq3Pin = A0; // Analog pin the sensor is connected to
int alcoholValue;

const int blinkPin = 2; // Digital pin the sensor output is connected to
unsigned long lastBlinkTime = 0; // Variable to store the time of the last blink
int blinkCount = 0; // Variable to store the number of blinks



void setup() {
  Serial.begin(9600); // Start serial communication

   pinMode(blinkPin, INPUT); // Set the sensor pin as input
  

}

void loop() {
  // for mq3 interfacing
  alcoholValue = analogRead(mq3Pin); // Read analog value from sensor
  Serial.print("Alcohol Value: ");
  Serial.println(alcoholValue); // Print the alcohol value to Serial Monitor
  delay(1000); // Delay for stability


  // for eyeblink
  if (digitalRead(blinkPin) == HIGH) { // Check if a blink is detected
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime > 200) { // Consider a blink if it's been at least 200ms since the last one
      blinkCount++;
      lastBlinkTime = currentTime;
    }
  }
  
  // Check for dizziness based on blink count
  if (blinkCount >= 5) { // Adjust the threshold as needed
    Serial.println("Dizziness Detected!");
    // Add any further actions here, such as sounding an alarm or sending a notification
    blinkCount = 0; // Reset blink count after detection
    delay(2000); // Delay to avoid repetitive detection
  }
  

}


void sendSMS() {
  // Wait until GPS module sends a valid sentence
  while (!gpsSerial.available() || gpsSerial.peek() != '$') {
    delay(500);
  }
  // Read GPS data
  String gpsData = "";
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    gpsData += c;
    if (c == '\n') {
      break;
    }
  }

  // Check if GPS data is valid
  if (gpsData.startsWith("$GPGGA")) {
    // Send SMS with GPS data
    gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
    delay(1000);
    gsmSerial.print("AT+CMGS=\"+8511092232\"\r Alcohol is detected"); // Replace +1234567890 with the recipient's phone number
    delay(1000);
    gsmSerial.print("GPS Data: ");
    gsmSerial.print(gpsData);
    gsmSerial.write(0x1A); // Send Ctrl+Z to indicate the end of the message
    delay(1000);
  }
}
