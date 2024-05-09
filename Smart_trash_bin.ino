#include <Servo.h>   //servo library

Servo servo;     
int trigPin = 5;    
int echoPin = 6;   
int servoPin = 7;
int ledPin = 10;

long duration, dist, average;   
long aver[3];   //array for average
bool objectDetected = false;   //flag variable to keep track of object presence

void setup() {       
  Serial.begin(9600);
  servo.attach(servoPin);  
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);  
  pinMode(ledPin, OUTPUT);  // set LED pin as output
  servo.write(0);         //close cap on power on
  delay(100);
  servo.detach(); 
} 

void measure() {  
  digitalWrite(ledPin, HIGH);   // turn on LED when measuring distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  dist = (duration/2) / 29.1;    //obtain distance
}

void loop() { 
  for (int i=0;i<=2;i++) {   //average distance
    measure();               
    aver[i]=dist;            
    delay(10);              //delay between measurements
  }
  dist=(aver[0]+aver[1]+aver[2])/3;    

  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.print(", Object Detected: ");
  Serial.println(objectDetected);

  if (dist < 50) {   //Change distance as per your need
    if (!objectDetected) {
      objectDetected = true;   //set flag variable to true if object detected
      servo.attach(servoPin);
      delay(1);
      for (int i = 0; i <=150; i++) {
        servo.write(i);
        delay(10); // delay in milliseconds
      }        
      delay(1000);       //delay before opening cap
    }
  } else {
    if (objectDetected) {
      objectDetected = false;   //set flag variable to false if object moved away
      for (int i =150; i >=0; i--) {
        servo.write(i);
        delay(10); // delay in milliseconds
      }          
      delay(1000);       //delay before closing cap
      servo.detach();      // detach servo to save power
    }
  }

  digitalWrite(ledPin, LOW);   // turn off LED after measuring distance and moving the cap
}
