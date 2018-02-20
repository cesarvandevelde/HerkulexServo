#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HerkulexServo.h>

#define PIN_SW_RX  8
#define PIN_SW_TX  9

SoftwareSerial   servo_serial(PIN_SW_RX, PIN_SW_TX);
HerkulexServoBus herkulex_bus(servo_serial);


void setup() {
  Serial.begin(115200);
  servo_serial.begin(115200);
  delay(500);
  Serial.println("Press 's' to scan the bus for servos.");
}

void loop() {
  herkulex_bus.update();

  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c == 's') {
      Serial.println("Scanning...");
      Serial.println("Addresses are displayed in hexadecimal");
      uint8_t servos_found = 0;

      for (uint8_t id = 0; id <= 0xFD; id++) {
        HerkulexPacket resp;
        bool success = herkulex_bus.sendPacketAndReadResponse(resp, id, HerkulexCommand::Stat);

        if (success) {
          servos_found++;

          if (id <= 0x0F) {
            Serial.print("0");
          }

          Serial.print(id, HEX);
        } else {
          Serial.print("--");
        }

        if ( ( (id+1) % 0x0F) == 0) {
          Serial.println();
        } else {
          Serial.print(" ");
        }
      }

      Serial.println();
      Serial.println("Done!");
      Serial.print("Found ");
      Serial.print(servos_found);
      Serial.println(" servos.");
    }
  }
}
