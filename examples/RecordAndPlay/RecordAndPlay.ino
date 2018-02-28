#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HerkulexServo.h>
#include <Bounce2.h>

#define SAMPLE_SIZE     50
#define SAMPLE_INTERVAL 100
#define PLAYBACK_SPEED  (SAMPLE_INTERVAL / 11.2 * 2)

#define PIN_SW_RX    8
#define PIN_SW_TX    9
#define PIN_BTN_REC  4
#define PIN_BTN_PLAY 5

SoftwareSerial   servo_serial(PIN_SW_RX, PIN_SW_TX);
HerkulexServoBus herkulex_bus(servo_serial);
HerkulexServo    my_servo(herkulex_bus, 12);
Bounce           btn_rec;
Bounce           btn_play;

uint16_t         samples[SAMPLE_SIZE] = {};
uint8_t          sample_idx = 0;
unsigned long    last_sample = 0;


enum class State {
  None = 0,
  Idle,
  Record,
  Play
};

State current_state = State::Idle;
State last_state = State::None;


void setup() {
  pinMode(PIN_BTN_REC, INPUT_PULLUP);
  pinMode(PIN_BTN_PLAY, INPUT_PULLUP);

  btn_rec.attach(PIN_BTN_REC);
  btn_rec.interval(50);

  btn_play.attach(PIN_BTN_PLAY);
  btn_play.interval(50);

  Serial.begin(115200);
  servo_serial.begin(115200);
  delay(500);
}

void loop() {
  herkulex_bus.update();
  btn_rec.update();
  btn_play.update();

  bool just_entered_state = (current_state != last_state);
  last_state = current_state;

  unsigned long now = millis();

  switch (current_state) {
    case State::Idle:
      if (just_entered_state) {
        my_servo.setLedColor(HerkulexLed::Off);
        my_servo.setTorqueOff();
      }

      if (btn_rec.fell()) {
        current_state = State::Record;
      }
      if (btn_play.fell()) {
        current_state = State::Play;
      }
      break;

    case State::Record:
      if (just_entered_state) {
        my_servo.setLedColor(HerkulexLed::Red);
        my_servo.setTorqueOff();
        last_sample = now;
        sample_idx = 0;
      }

      if ( (now - last_sample) > SAMPLE_INTERVAL ) {
        last_sample = now;

        if (sample_idx == SAMPLE_SIZE) {
          current_state = State::Idle;
          return;
        } else {
          samples[sample_idx] = my_servo.getPosition();
          sample_idx++;
        }
      }
      break;

    case State::Play:
      if (just_entered_state) {
        my_servo.setLedColor(HerkulexLed::Green);
        my_servo.setTorqueOn();
        last_sample = now;
        sample_idx = 0;
      }

      if ( (now - last_sample) > SAMPLE_INTERVAL ) {
        last_sample = now;

        if (sample_idx == SAMPLE_SIZE) {
          current_state = State::Idle;
          return;
        } else {
          my_servo.setPosition(samples[sample_idx], PLAYBACK_SPEED);
          sample_idx++;
        }
      }
      break;
  }
}
