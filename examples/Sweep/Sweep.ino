#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HerkulexServo.h>

#define PIN_SW_RX 8
#define PIN_SW_TX 9
#define SERVO_ID  12

SoftwareSerial   servo_serial(PIN_SW_RX, PIN_SW_TX);
HerkulexServoBus herkulex_bus(servo_serial);
HerkulexServo    my_servo(herkulex_bus, SERVO_ID);

unsigned long last_update = 0;
unsigned long now = 0;
bool toggle = false;


void setup() {
  Serial.begin(115200);
  servo_serial.begin(115200);
  delay(500);
  my_servo.setTorqueOn();  // turn power on
}

void loop() {
  herkulex_bus.update();

  now = millis();

  if ( (now - last_update) > 1000) {
    // called every 1000 ms
    if (toggle) {
      // move to -90° over a duration of 560ms, set LED to green
      // 512 - 90°/0.325 = 235
      my_servo.setPosition(235, 50, HerkulexLed::Green);
    } else {
      // move to +90° over a duration of 560ms, set LED to blue
      // 512 + 90°/0.325 = 789
      my_servo.setPosition(789, 50, HerkulexLed::Blue);
    }

    last_update = now;
    toggle = !toggle;
  }
}
