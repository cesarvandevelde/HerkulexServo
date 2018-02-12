#include "HerkulexServo.h"

HerkulexServoBus::HerkulexServoBus() : m_serial(nullptr) {

}

void HerkulexServoBus::begin(Stream &serial_connection) {
  m_serial = &serial_connection;
}

void HerkulexServoBus::sendPacket(uint8_t id, uint8_t cmd, const uint8_t* data = nullptr, uint8_t data_len = 0) {
  if (!m_serial) {
    return;
  }

  uint8_t checksum1;
  uint8_t checksum2;
  uint8_t packet_size = 7 + data_len;
  uint8_t packet[7] = {0xFF, 0xFF, packet_size, id, cmd, 0x00, 0x00};

  checksum1 = packet_size ^ id ^ cmd;

  if (data && data_len > 0) {
     for (uint8_t i = 0; i < data_len; i++) {
       checksum1 ^= data[i];
     }
  }

  checksum1 = checksum1 & 0xFE;
  checksum2 = (~checksum1) & 0xFE;

  packet[5] = checksum1;
  packet[6] = checksum2;
  m_serial->write(packet, 7);

  if (data && data_len > 0) {
    m_serial->write(data, data_len);
  }

  m_serial->flush();
}

HerkulexServo::HerkulexServo(HerkulexServoBus &bus, uint8_t id) : m_bus(&bus), m_id(id) {}

void HerkulexServo::writeRam(uint8_t reg, uint8_t val) {
  uint8_t data[] = {reg, 1, val};
  m_bus->sendPacket(m_id, DRS_CMD_RAM_WRITE, data, 3);
}
