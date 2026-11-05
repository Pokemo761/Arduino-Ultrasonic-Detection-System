#include <Servo.h>

#define trigPin 2
#define echoPin 3

Servo servo_base;
Servo servo_middle;
Servo servo_top;

long duration;
int distance;

void setup()
{
  servo_base.attach(9);
  servo_middle.attach(10);
  servo_top.attach(5);

  servo_top.write(80);
  servo_middle.write(90);
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

int i, j;
void loop()
{

  for ( i = 0; i <= 180; i += 5)
  {

    ultrasonic();

    servo_base.write(i);

    if (i == 0)
    {
      j = 160;
    }
    else if (i == 10)
    {
      j = 140;
    }
    else if (i == 30)
    {
      j = 160;
    }
    else if (i == 50)
    {
      j = 140;
    }
    else if (i == 70)
    {
      j = 160;
    }
    else if (i == 90)
    {
      j = 140;
    }
    else if (i == 110)
    {
      j = 160;
    }
    else if (i == 130)
    {
      j = 140;
    }
    else if (i == 150)
    {
      j = 160;
    }
    else if (i == 170)
    {
      j = 140;
    }


    servo_middle.write(j);
    delay(100);
    while (distance <= 30)
    {

      servo_top.write(40);
      delay(500);
      ultrasonic();

    }

    servo_top.write(80);
    //delay(100);


  }



  for ( i = 180; i >= 0; i -= 5)
  {

    ultrasonic();

    servo_base.write(i);
    

    if (i == 170)
    {
      j = 160;
    }
    else if (i == 150)
    {
      j = 140;
    }
    else if (i == 130)
    {
      j = 160;
    }
    else if (i == 110)
    {
      j = 140;
    }
    else if (i == 90)
    {
      j = 160;
    }
    else if (i == 70)
    {
      j = 140;
    }
    else if (i == 50)
    {
      j = 160;
    }
    else if (i == 30)
    {
      j = 140;
    }
    else if (i == 10)
    {
      j = 160;
    }
   
    servo_middle.write(j);
    delay(100);


    while (distance <= 30)
    {
      servo_top.write(40);
      delay(500);
      ultrasonic();

    }
    servo_top.write(80);
    //delay(100);

  }

}

void ultrasonic()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;
  Serial.println(distance);

}
