#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

// Define motor driver pins
const int in1 = 2;
const int in2 = 10;
const int in3 = 5;
const int in4 = 4;

// Define accelerometer pins
const int xPin = A0;
const int yPin = A1;
const int zPin = A2;

const int accelerometerThreshold = 400;

// Define minimum distance to trigger "speed is decreasing" message (in cm)
//const int mindistance = 50;
int distance;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // Set motor driver pins as outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

 
  
}

void loop() {
  

  
  
  if (radio.available()) {

    // Read distance data
    radio.read(&distance, sizeof(distance));

    // Print received distance data
    Serial.print("Received Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    //delay(1000);
    digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  

 

  if(distance<50){
     digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  }
    }

   int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int zValue = analogRead(zPin);

  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print("\tY: ");
  Serial.print(yValue);
  Serial.print("\tZ: ");
  Serial.println(zValue);

    
  delay(500);
    
}
