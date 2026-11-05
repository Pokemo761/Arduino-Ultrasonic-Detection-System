/*
  AUTOMATIC AIR DEFENSE SYSTEM — Combined Sketch
  Marcos Perez

  This sketch merges two previously separate programs into one working system:
    1) "radar_project.ino"      -> OLED radar sweep display + buzzer sounds
    2) "final_code_missile_Launcher_22.ino" -> 3-servo turret + firing action

  How it works:
    - The pan servo sweeps the ultrasonic sensor back and forth (like the radar_project code).
    - As it sweeps, the OLED screen draws a radar sweep line and a dot for any object it finds
      (reused from radar_project.ino), and the buzzer beeps once per sweep step.
    - If an object is detected within FIRE_RANGE_CM, the system:
        a) plays a rising-pitch alert tone (closer = higher pitch),
        b) moves the tilt servo to track the last known angle,
        c) fires the trigger servo (the "launch" motion from the missile launcher code),
        d) returns the trigger servo to rest position.

  Hardware pins (matches your breadboard photos):
    trigPin      = 10   (ultrasonic sensor)
    echoPin      = 11   (ultrasonic sensor)
    buzzerPin    = 6    (piezo buzzer)
    panServoPin  = 12   (sweeping base servo - was "myServo" in radar_project.ino)
    tiltServoPin = 10   (middle/tilt servo from launcher code - NOTE: change if pin 10
                          conflicts with trigPin on your board; move tilt servo to an
                          unused pin such as 4 if needed)
    fireServoPin = 5    (top/trigger servo - "servo_top" in launcher code)
*/

#include <Servo.h>
#include <U8g2lib.h>
#include <Wire.h>

// ---------- Pins ----------
const int trigPin = 10;
const int echoPin = 11;
const int buzzerPin = 6;

const int panServoPin  = 12;
const int tiltServoPin = 4;   // moved off pin 10 to avoid clashing with trigPin
const int fireServoPin = 5;

// ---------- Behavior settings ----------
const int FIRE_RANGE_CM = 30;   // distance threshold that triggers a "launch"
const int FIRE_ANGLE    = 40;   // trigger servo angle when firing
const int REST_ANGLE    = 80;   // trigger servo angle at rest

Servo panServo;
Servo tiltServo;
Servo fireServo;

long duration;
int distance;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);

  panServo.attach(panServoPin);
  tiltServo.attach(tiltServoPin);
  fireServo.attach(fireServoPin);
  fireServo.write(REST_ANGLE);
  tiltServo.write(90);

  u8g2.begin();
  u8g2.setContrast(100);

  playStartupSound();
  Serial.println("Air Defense System initialized");
}

void loop() {
  // Sweep forward
  for (int angle = 15; angle <= 165; angle += 2) {
    scanAndRespond(angle);
  }
  // Sweep backward
  for (int angle = 165; angle >= 15; angle -= 2) {
    scanAndRespond(angle);
  }
}

void scanAndRespond(int angle) {
  panServo.write(angle);
  tiltServo.write(map(angle, 15, 165, 70, 110));  // gentle tilt sway for visual effect
  delay(15);

  distance = calculateDistance();
  radarBeep();
  drawRadar(angle, distance);

  if (distance > 0 && distance < FIRE_RANGE_CM) {
    playDetectionAlert(distance);
    fire();
  }

  Serial.print(angle);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
}

// ==================== FIRE CONTROL ====================
void fire() {
  fireServo.write(FIRE_ANGLE);
  delay(500);
  fireServo.write(REST_ANGLE);
}

// ==================== SENSOR ====================
int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

// ==================== DISPLAY ====================
void drawRadar(int angle, int dist) {
  u8g2.clearBuffer();

  int cx = 64, cy = 63;
  u8g2.drawCircle(cx, cy, 20, U8G2_DRAW_ALL);
  u8g2.drawCircle(cx, cy, 40, U8G2_DRAW_ALL);
  u8g2.drawLine(cx, cy, cx, cy - 45);

  float rad = radians(angle);
  int x2 = cx + 45 * cos(rad);
  int y2 = cy - 45 * sin(rad);
  u8g2.drawLine(cx, cy, x2, y2);

  if (dist > 0 && dist < 40) {
    int r = map(dist, 0, 40, 0, 45);
    int dotX = cx + r * cos(rad);
    int dotY = cy - r * sin(rad);
    u8g2.drawDisc(dotX, dotY, 2, U8G2_DRAW_ALL);
  }

  u8g2.setFont(u8g2_font_6x10_tr);
  char info[24];
  sprintf(info, "A:%d  D:%dcm", angle, dist);
  u8g2.drawStr(2, 10, info);

  u8g2.sendBuffer();
}

// ==================== SOUND ====================
void playStartupSound() {
  for (int i = 0; i < 3; i++) {
    tone(buzzerPin, 800, 100);
    delay(150);
  }
  tone(buzzerPin, 1200, 300);
  delay(500);
}

void radarBeep() {
  tone(buzzerPin, 600, 10);
  delay(10);
}

void playDetectionAlert(int dist) {
  int pitch = map(dist, 5, 40, 2000, 800);
  int beepDuration = map(dist, 5, 40, 50, 200);
  tone(buzzerPin, pitch, beepDuration);
  delay(beepDuration + 50);

  if (dist < 15) {
    tone(buzzerPin, 2500, 100);
    delay(120);
  }
}
