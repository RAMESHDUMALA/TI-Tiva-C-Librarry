
#include <Ultrasonic.h>

Ultrasonic ultrasonic(8, 9); // (Trig PIN,Echo PIN)

int distance = 0;
int pirPin = 10;    //the digital pin connected to the PIR sensor's output

int maxdistance=45;


#define street_light 4

void setup() {
  Serial.begin (9600);
  pinMode(pirPin, INPUT);
  pinMode(street_light, OUTPUT);

}

void loop() {

  distance = ultrasonic.Ranging(CM);
  Serial.print(distance);
  Serial.println(" cm" );
  delay(500);
  if (distance <= maxdistance && digitalRead(pirPin) == HIGH) {
    Serial.print("MOTION");
    digitalWrite(street_light, HIGH);     // Street Light ON
    delay(500);
  }
  else if (distance >= maxdistance && digitalRead(pirPin) == LOW)
  { 
    digitalWrite(street_light, LOW);     // Street Light OFF
    delay(500);
  }

}
