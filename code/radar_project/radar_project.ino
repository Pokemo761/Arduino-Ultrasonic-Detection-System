#include <Servo.h>
#include <U8g2lib.h>
#include <Wire.h>

// Ultrasonic Sensor pins
const int trigPin = 10;
const int echoPin = 11;

// Buzzer pin
const int buzzerPin = 6;

// Servo pin
Servo myServo;

// Variables
long duration;
int distance;

// OLED Display (I2C 0x3C)
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

  Serial.println("Radar initialized");
}

// Main loop
void loop() {
  // Sweep forward
  for (int angle = 15; angle <= 165; angle += 2) {
    scanAndDraw(angle);
  }

  // Sweep backward
  for (int angle = 165; angle >= 15; angle -= 2) {
    scanAndDraw(angle);
  }
}

void scanAndDraw(int angle) {
  myServo.write(angle);
  delay(15);
  distance = calculateDistance();

  // Radar beep
  radarBeep();

  // Update radar display
  drawRadar(angle, distance);

  // Alert if object close
  if (distance > 0 && distance < 40) {
    playDetectionAlert(distance);
  }

  Serial.print(angle);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
}

int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int dist = duration * 0.034 / 2;
  return dist;
}

// ==================== DISPLAY FUNCTIONS ====================

void drawRadar(int angle, int dist) {
  u8g2.clearBuffer();

  // Draw radar base
  int cx = 64;  // center x
  int cy = 63;  // center y
  u8g2.drawCircle(cx, cy, 20, U8G2_DRAW_ALL);
  u8g2.drawCircle(cx, cy, 40, U8G2_DRAW_ALL);
  u8g2.drawLine(cx, cy, cx, cy - 45);  // reference line

  // Draw sweep line
  float rad = radians(angle);
  int x2 = cx + 45 * cos(rad);
  int y2 = cy - 45 * sin(rad);
  u8g2.drawLine(cx, cy, x2, y2);

  // If object detected within 40 cm → draw dot
  if (dist > 0 && dist < 40) {
    // Map distance (max 40cm) to 45px radius
    int r = map(dist, 0, 40, 0, 45);
    int dotX = cx + r * cos(rad);
    int dotY = cy - r * sin(rad);
    u8g2.drawDisc(dotX, dotY, 2, U8G2_DRAW_ALL);
  }

  // Display angle and distance text
  u8g2.setFont(u8g2_font_6x10_tr);
  char info[20];
  sprintf(info, "A:%d  D:%dcm", angle, dist);
  u8g2.drawStr(2, 10, info);

  u8g2.sendBuffer();
}

// ==================== BUZZER FUNCTIONS ====================

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
