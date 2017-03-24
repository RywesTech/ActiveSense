/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Ryan Westcott <ryan@westcott.email> wrote this file.  As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 * 
 * PINS:
 * 2 - sensor
 * 4 - input
 * 5 - output
 * 
 * Stable release: 1
 * Overall Build:  25
 *
 */

#include <Servo.h>

Servo output;

const int sensorPin = 2;
const int inputPin = 6;
const int outputPin = 5;

int rawInVal, speedVal, outputVal, distanceCM;

bool systemEnabled;

void setup() {
  Serial.begin(9600); // Pour a bowl of Serial
  Serial.println("INIT");

  pinMode(inputPin, INPUT);
  output.attach(outputPin);
}

void loop() {

  // See how far away we are from the nearest obstacle
  distanceCM = distance(sensorPin);

  // See what our current user input is
  rawInVal = pulseIn(inputPin, HIGH, 25000);
  int in = map(rawInVal, 1100, 1900, -100, 100);
  if (in < 120 && in > -120) {
    speedVal = in;
  }

  // Run the compensation algorithim and output our new data
  //outputVal = map(compensateVal(), -100, 100, 0, 255); //25, 160
  outputVal = compensateVal();
  //output.write(map(outputVal, 0, 255, 25, 160);
  output.write(map(outputVal, -100, 100, 27, 157));

  // Log some data

  Serial.print("Compensate Val: ");
  Serial.print(compensateVal());
  Serial.print(", Distance:");
  Serial.print(distanceCM);
  Serial.print("cm , Input: ");
  Serial.print(speedVal);
  Serial.print(", Output: ");
  Serial.println(compensateVal());


  Serial.println(millis());
}

int compensateVal() {
  int val;
  if (distanceCM < 200) { // If we are close to an obstical, take over control
    val = map(distanceCM, 0, 200, 100, 0);
  } else {
    val = speedVal;
  }
  return val;
}

int distance(int pin) {
  long duration, cm;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);

  pinMode(pin, INPUT);
  duration = pulseIn(pin, HIGH);

  cm = microsecondsToCentimeters(duration);
  return cm;
}

long microsecondsToInches(long microseconds) {
  // Speed of sound = 73.7 microseconds per inch
  // Sound must travel there and back so devide by 2
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // Speed of sound = 29 microseconds per centimeter
  // Sound must travel there and back so devide by 2
  return microseconds / 29 / 2;
}
