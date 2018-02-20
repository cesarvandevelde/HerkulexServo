#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HerkulexServo.h>

#define PIN_SW_RX  8
#define PIN_SW_TX  9
#define SERVO_ID_A 10
#define SERVO_ID_B 11

SoftwareSerial   servo_serial(PIN_SW_RX, PIN_SW_TX);
HerkulexServoBus herkulex_bus(servo_serial);
HerkulexServo    servo_a(herkulex_bus, SERVO_ID_A);
HerkulexServo    servo_b(herkulex_bus, SERVO_ID_B);


void setup() {
  Serial.begin(115200);
  servo_serial.begin(115200);
  delay(500);

  // turn power on
  servo_a.setTorqueOn();
  servo_b.setTorqueOn();

  herkulex_bus.prepareIndividualMove();
  servo_a.setPosition(512, 50);
  servo_b.setPosition(512, 100);
  herkulex_bus.executeMove();

  delay(100 * 11.2f);

  // turn power off
  servo_a.setTorqueOff();
  servo_b.setTorqueOff();
}

void loop() {
  herkulex_bus.update();
}
