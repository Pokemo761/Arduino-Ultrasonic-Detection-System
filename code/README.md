# Arduino Code

All three sketches are included exactly as written. Each is in its own folder named after the file, as the Arduino IDE expects. None has been compiled or run by the person who prepared this repository, and no test results are documented.

| Sketch | What it does | Origin |
|---|---|---|
| [`radar_project/radar_project.ino`](radar_project/radar_project.ino) | Radar-style OLED sweep display and buzzer feedback. No turret or firing logic. | Original sketch |
| [`final_code_missile_Launcher_22/final_code_missile_Launcher_22.ino`](final_code_missile_Launcher_22/final_code_missile_Launcher_22.ino) | 3-servo turret (base/pan, middle/tilt, top/trigger) driven by an ultrasonic sensor. | **Adapted from the [Automatic Missile Defence System](https://www.instructables.com/Automatic-Missile-Defence-System/) Instructables project** |
| [`Combined_Air_Defense_System/Combined_Air_Defense_System.ino`](Combined_Air_Defense_System/Combined_Air_Defense_System.ino) | Integrated sketch: one sensor sweep drives the OLED display, buzzer, and trigger servo. | Merges the two sketches above |

**Libraries needed:** `Servo` for all three; `U8g2` and `Wire` for the radar and combined sketches.

## Notes from reading the code

- **Pins differ between sketches.** The radar sketch uses trigger/echo on pins 10/11, pan servo on 12, and buzzer on 6. The turret sketch uses trigger/echo on pins 2/3 and servos on 9, 10, and 5, so its pin 10 is a servo while the radar sketch's pin 10 is the trigger. The combined sketch keeps the radar sketch's sensor pins, uses pin 4 for the tilt servo, and keeps pin 5 for the trigger servo.
- **Detection behavior differs.** The turret sketch fires while `distance <= 30` and tilts the middle servo using a fixed table of positions. The combined sketch fires when `distance > 0 && distance < FIRE_RANGE_CM` (30 cm) and sways the tilt servo with `map(angle, 15, 165, 70, 110)`.
- **Header comment in the combined sketch.** Its pin list still says `tiltServoPin = 10` with a note to change it if there's a conflict. The code itself uses pin 4, and the code is what governs.
- **Attribution.** The turret sketch follows the structure of the referenced Instructables project. Check that project's page for its terms of use before reusing or redistributing it.

Partial excerpts of these sketches as they appear in the project write-up are also in [`../docs/code-excerpts.md`](../docs/code-excerpts.md).
