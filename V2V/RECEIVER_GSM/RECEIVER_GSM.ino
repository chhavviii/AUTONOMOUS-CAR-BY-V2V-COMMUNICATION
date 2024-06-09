#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>

SoftwareSerial MYGSM(3, 6);

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

// Define motor driver pins
const int in1 = 2;
const int in2 = 10;
const int in3 = 5;
const int in4 = 4;

const int xPin = A0;
const int yPin = A1;
const int zPin = A2;

const int accelerometerThreshold = 400;

bool accidentDetected = false;

int distance;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  MYGSM.begin(9600);
  
}

void loop() {
  if (radio.available()) {
    radio.read(&distance, sizeof(distance));
    Serial.print("Received Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 600 && !accidentDetected) {
      // Perform actions upon detecting an accident
      Serial.println("Accident detected!");
      stopMotor(); // Stop the motor

      // Send SMS if not already sent
      if (!accidentDetected) {
        SendMessage("Accident detected!");
        accidentDetected = true;
      }
    }
  }

  // Read accelerometer values
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int zValue = analogRead(zPin);

  // Check for acceleration exceeding threshold
  if (abs(xValue) > accelerometerThreshold || abs(yValue) > accelerometerThreshold || abs(zValue) > accelerometerThreshold) {
    Serial.println("Accident Detected!");
    stopMotor(); // Stop the motor

    if (!accidentDetected) {
      SendMessage("Accident detected!");
      accidentDetected = true;
    }
  }

  // Display accelerometer values
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print("\tY: ");
  Serial.print(yValue);
  Serial.print("\tZ: ");
  Serial.println(zValue);

  delay(500);
}

void SendMessage(String message) {
  Serial.println("Sending message...");
  MYGSM.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  MYGSM.print("AT+CMGS=\"+917488143244\"\r"); // Set recipient number
  delay(1000);
  MYGSM.println(message); // Message content
  delay(100);
  MYGSM.println((char)26); // Send message
  delay(1000);
  Serial.println("Message sent");
}

void stopMotor() {
  // Implement motor stop logic here
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
