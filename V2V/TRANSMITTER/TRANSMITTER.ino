#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

// Define ultrasonic sensor pins
const int trigPin = 10;
const int echoPin = 6;

long duration;
int distance;
// Define motor driver pins
const int in1 = 9;
const int in2 = 3;
const int in3 = 5;
const int in4 = 4;

//define accelerometer pins 
const int xPin = A0;
const int yPin = A1;
const int zPin = A2;

const int accelerometerThreshold = 500;

// Define minimum distance to stop the motor (in cm)
const int minDistance = 10;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  // Set motor driver pins as outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Set ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  
}

void loop() {
  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the echo time to calculate distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Print distance to serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  radio.write(&distance,sizeof(distance));
  

  // Check if the distance is less than the minimum distance
  if (distance < minDistance) {
    // Stop the motor
    stopMotor();
  } else {
    // Move the car forward
    moveForward();
  }

  // Send data over NRF24L01
  
  

  // Delay for stability
  delay(1000);

   int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int zValue = analogRead(zPin);

  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print("\tY: ");
  Serial.print(yValue);
  Serial.print("\tZ: ");
  Serial.println(zValue);

   if (abs(xValue) > accelerometerThreshold || abs(yValue) > accelerometerThreshold || abs(zValue) > accelerometerThreshold) {
    // Display a message indicating an accident
    Serial.println("Accident Detected!");
    
    // Stop the motor (you may want to add additional actions here)
    stopMotor();
    }

  delay(500);  // Adjust delay as needed
}

void moveForward() {
  // Set motor driver pins to move the car forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stopMotor() {
  // Set motor driver pins to stop the motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
