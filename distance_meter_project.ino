#include <Wire.h>                 // Library for I2C communication
#include <LiquidCrystal_I2C.h>    // Library for I2C LCD display

LiquidCrystal_I2C lcd(0x27, 16, 2); 
// Create LCD object with I2C address 0x27
// 16 columns and 2 rows display

const int trigPin = 9;    // Trigger pin of ultrasonic sensor connected to pin 9
const int echoPin = 10;   // Echo pin connected to pin 10
const int buzzerPin = 8;  // Buzzer connected to pin 8

long duration;            // Variable to store time taken for echo
float distance_cm;        // Variable to store distance in centimeters
int distance_mm;          // Variable to store distance in millimeters

void setup() {

  pinMode(trigPin, OUTPUT);   // Set trigPin as OUTPUT (to send signal)
  pinMode(echoPin, INPUT);    // Set echoPin as INPUT (to receive signal)
  pinMode(buzzerPin, OUTPUT); // Set buzzerPin as OUTPUT

  lcd.init();           // Initialize LCD
  lcd.backlight();      // Turn ON LCD backlight

  Serial.begin(9600);   // Start serial communication at 9600 baud rate
}

void loop() {

  // --------- SEND ULTRASONIC PULSE ---------

  digitalWrite(trigPin, LOW);      // Make sure trig is LOW
  delayMicroseconds(2);            // Small delay

  digitalWrite(trigPin, HIGH);     // Send HIGH pulse
  delayMicroseconds(10);           // Pulse width = 10 microseconds
  digitalWrite(trigPin, LOW);      // Stop pulse

  // --------- RECEIVE ECHO ---------

  duration = pulseIn(echoPin, HIGH);
  // pulseIn measures time (in microseconds)
  // for which echoPin stays HIGH

  // --------- DISTANCE CALCULATION ---------

  distance_cm = duration * 0.0343 / 2;   
  // 0.0343 = speed of sound in cm per microsecond
  // Divide by 2 because sound travels to object and comes back

  distance_mm = distance_cm * 10;  
  // Convert cm to millimeters

  // --------- DISPLAY ON LCD ---------

  lcd.setCursor(0, 0);     // Set cursor at column 0, row 0
  lcd.print("Dist:");      // Print label
  lcd.print(distance_cm, 1); // Print distance with 1 decimal value
  lcd.print(" cm ");       

  lcd.setCursor(0, 1);     // Second line
  lcd.print(distance_mm);  // Print distance in mm
  lcd.print(" mm   ");     // Extra spaces to clear old data

  // --------- BUZZER CONTROL ---------

  if (distance_cm < 5) {
    // If object is extremely close
    digitalWrite(buzzerPin, HIGH);   // Continuous sound
  }
  else if (distance_cm < 10) {
    // If object is close
    digitalWrite(buzzerPin, HIGH);   
    delay(100);                      // Short ON time
    digitalWrite(buzzerPin, LOW);    
    delay(100);                      // Short OFF time
  }
  else if (distance_cm < 20) {
    // If object is at medium distance
    digitalWrite(buzzerPin, HIGH);   
    delay(300);                      // Long ON
    digitalWrite(buzzerPin, LOW);    
    delay(300);                      // Long OFF
  }
  else {
    // If object is far (safe zone)
    digitalWrite(buzzerPin, LOW);    // Buzzer OFF
  }

  delay(100);   // Small delay before next reading
}
