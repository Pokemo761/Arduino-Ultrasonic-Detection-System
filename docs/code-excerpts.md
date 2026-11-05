# Code Excerpts from the Project Write-Up

These are the partial listings printed in [Marcos_Perez_Air_Defense_System_Project.pdf](Marcos_Perez_Air_Defense_System_Project.pdf), reproduced as shown there. **They are not complete programs.** Lines beginning `// ...` or `// (see ...)` mark code omitted from the write-up. The complete sketches are in the [`../code/`](../code/README.md) folder; use those rather than these excerpts.

## 1. Radar display and sound system (original sketch)

Handles the OLED radar-style sweep and buzzer feedback from live ultrasonic readings.

```cpp
#include <Servo.h>
#include <U8g2lib.h>
#include <Wire.h>

const int trigPin = 10;
const int echoPin = 11;
const int buzzerPin = 6;
Servo myServo;
long duration;
int distance;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  myServo.attach(12);
  u8g2.begin();
  u8g2.setContrast(100);
  playStartupSound();
}

void loop() {
  for (int angle = 15; angle <= 165; angle += 2) scanAndDraw(angle);
  for (int angle = 165; angle >= 15; angle -= 2) scanAndDraw(angle);
}
// ... full sweep, sensor, display, and sound functions
// (see radar_project.ino for complete listing)
```

## 2. Turret and firing mechanism (adapted from the reference project)

Controls the 3-servo turret (base/pan, middle/tilt, top/trigger) and fires when a target is within range. Adapted from the [Automatic Missile Defence System](https://www.instructables.com/Automatic-Missile-Defence-System/) Instructables project.

```cpp
#include <Servo.h>
#define trigPin 2
#define echoPin 3
Servo servo_base, servo_middle, servo_top;
long duration; int distance;

void setup() {
  servo_base.attach(9);
  servo_middle.attach(10);
  servo_top.attach(5);
  servo_top.write(80);
  servo_middle.write(90);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  for (int i = 0; i <= 180; i += 5) {
    ultrasonic();
    servo_base.write(i);
    // tilt table sets servo_middle position based on i ...
    while (distance <= 30) {
      servo_top.write(40);  // fire
      delay(500);
      ultrasonic();
    }
    servo_top.write(80);  // reset
  }
  // mirrored sweep back from 180 to 0 ...
}
// (see final_code_missile_Launcher_22.ino for complete listing)
```

## 3. Combined system (integrated sketch)

Merges both systems into one program: a single sweep drives the radar-style display, sound, and automatic firing.

```cpp
#include <Servo.h>
#include <U8g2lib.h>
#include <Wire.h>

const int trigPin = 10, echoPin = 11, buzzerPin = 6;
const int panServoPin = 12, tiltServoPin = 4, fireServoPin = 5;
const int FIRE_RANGE_CM = 30, FIRE_ANGLE = 40, REST_ANGLE = 80;
Servo panServo, tiltServo, fireServo;
long duration; int distance;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT); pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  panServo.attach(panServoPin);
  tiltServo.attach(tiltServoPin);
  fireServo.attach(fireServoPin);
  fireServo.write(REST_ANGLE);
  tiltServo.write(90);
  u8g2.begin();
  u8g2.setContrast(100);
  playStartupSound();
}

void loop() {
  for (int angle = 15; angle <= 165; angle += 2) scanAndRespond(angle);
  for (int angle = 165; angle >= 15; angle -= 2) scanAndRespond(angle);
}

void scanAndRespond(int angle) {
  panServo.write(angle);
  tiltServo.write(map(angle, 15, 165, 70, 110));
  delay(15);
  distance = calculateDistance();
  radarBeep();
  drawRadar(angle, distance);
  if (distance > 0 && distance < FIRE_RANGE_CM) {
    playDetectionAlert(distance);
    fire();
  }
}

void fire() {
  fireServo.write(FIRE_ANGLE);
  delay(500);
  fireServo.write(REST_ANGLE);
}
// calculateDistance(), drawRadar(), playStartupSound(),
// radarBeep(), and playDetectionAlert() carried over from the radar sketch above.
```
