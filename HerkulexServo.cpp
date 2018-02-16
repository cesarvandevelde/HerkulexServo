#include "HerkulexServo.h"


HerkulexServoBus::HerkulexServoBus() : m_serial(nullptr) {}


void HerkulexServoBus::begin(Stream &serial_connection) {
  m_serial = &serial_connection;
}


void HerkulexServoBus::sendPacket(uint8_t id, HerkulexCommand cmd, const uint8_t* data, uint8_t data_len) {
  if (!m_serial) {
    return;
  }

  uint8_t checksum1;
  uint8_t checksum2;
  uint8_t packet_size = 7 + data_len;
  uint8_t packet[7] = {0xFF, 0xFF, packet_size, id, static_cast<uint8_t>(cmd), 0x00, 0x00};

  checksum1 = packet_size ^ id ^ static_cast<uint8_t>(cmd);

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

bool HerkulexServoBus::sendPacketAndReadResponse(HerkulexPacket &resp, uint8_t id, HerkulexCommand cmd, const uint8_t* data, uint8_t data_len){
  bool success = false;

  update();
  m_packets.clear();

  for (uint8_t attempts = 0; attempts < DRS_PACKET_RETRIES; attempts++) {
    sendPacket(id, cmd, data, data_len);

    while (!getPacket(resp)) {
      update();
    }

    if ( (resp.error == HerkulexPacketError:: None) && (resp.id == id) && (resp.cmd == cmd) ) {
      success = true;
      break;
    } else {
      delayMicroseconds(DRS_PACKET_RESEND_DELAY);
    }
  }

  return success;
}


void HerkulexServoBus::update() {
  if (m_serial->available() > 0) {
    // push data to buffer
    while (m_serial->available()) {
      m_buffer.push(m_serial->read());
      last_serial = micros();
    }

    // remove characters before header
    while (m_buffer.size() >= 2) {
      if (m_buffer[0] != 0xFF || m_buffer[1] != 0xFF) {
        m_buffer.shift();
      } else {
        break;
      }
    }

    // see if packet is complete
    if (m_buffer.size() >= 9) {  // 7 (min packet length) + 2 (status data)
      uint8_t packet_len = m_buffer[2];
      if (m_buffer.size() >= packet_len) {
        processPacket(false);
        return;
      }
    }
  }

  if (m_buffer.size() > 0) {
    unsigned long now = micros();

    if (now - last_serial > DRS_PACKET_RX_TIMEOUT) {
      processPacket(true);
      last_serial = now;
    }
  }
}


namespace {
  enum class HerkulexParserState : uint8_t {
    Header1 = 0,
    Header2,
    Length,
    ID,
    Command,
    Checksum1,
    Checksum2,
    Data
  };
}

void HerkulexServoBus::processPacket(bool timeout) {
  HerkulexPacket packet = {};

  if (timeout) {
    packet.error |= HerkulexPacketError::Timeout;
  }

  uint8_t bytes_to_process;
  uint8_t checksum1;
  uint8_t checksum2;

  if (m_buffer.size() >= 9) {
    // check length field
    if (m_buffer[2] > m_buffer.size()) {
      bytes_to_process = m_buffer.size();
      packet.error |= HerkulexPacketError::Length;
    } else {
      bytes_to_process = m_buffer[2];
    }
  } else {
    bytes_to_process = m_buffer.size();
    packet.error |= HerkulexPacketError::Length;
  }

  HerkulexParserState state = HerkulexParserState::Header1;
  uint8_t data_idx = 0;

  while (bytes_to_process > 0) {
    uint8_t b = m_buffer.shift();
    bytes_to_process--;

    switch (state) {
      case HerkulexParserState::Header1:
        if (b == 0xFF) {
          state = HerkulexParserState::Header2;
        }
        break;

      case HerkulexParserState::Header2:
        if (b == 0xFF) {
          state = HerkulexParserState::Length;
        } else {
          state = HerkulexParserState::Header1;
        }
        break;

      case HerkulexParserState::Length:
        packet.size = b;
        checksum1 = b;
        state = HerkulexParserState::ID;
        break;

      case HerkulexParserState::ID:
        packet.id = b;
        checksum1 ^= b;
        state = HerkulexParserState::Command;
        break;

      case HerkulexParserState::Command:
        if (b > 0x40) {
          packet.cmd = static_cast<HerkulexCommand>(b - 0x40);
        } else {
          packet.cmd = static_cast<HerkulexCommand>(b);
          packet.error |= HerkulexPacketError::Command;
        }
        checksum1 ^= b;

        state = HerkulexParserState::Checksum1;
        break;

      case HerkulexParserState::Checksum1:
        packet.checksum1 = b;
        state = HerkulexParserState::Checksum2;
        break;

      case HerkulexParserState::Checksum2:
        packet.checksum2 = b;
        state = HerkulexParserState::Data;
        break;

      case HerkulexParserState::Data:
        packet.data[data_idx] = b;
        checksum1 ^= b;
        data_idx++;

        // clamp idx to size of data array
        if (data_idx >= DRS_PACKET_RX_MAX_DATA) {
          data_idx = DRS_PACKET_RX_MAX_DATA - 1;
        }
        break;
    }
  }

  if (data_idx >= 2) {
    packet.status_error = packet.data[data_idx - 2];
    packet.status_detail = packet.data[data_idx - 1];
  }

  checksum1 = checksum1 & 0xFE;
  checksum2 = (~checksum1) & 0xFE;

  if (packet.checksum1 != checksum1 || packet.checksum2 != checksum2) {
    packet.error |= HerkulexPacketError::Checksum;
  }

  m_packets.push(packet);
}


bool HerkulexServoBus::getPacket(HerkulexPacket &packet) {
  if (m_packets.size() == 0) {
    return false;
  }

  packet = m_packets.shift();
  return true;
}


HerkulexServo::HerkulexServo(HerkulexServoBus &bus, uint8_t id) : m_bus(&bus), m_id(id) {}


void HerkulexServo::writeRam(HerkulexRamRegister reg, uint8_t val) {
  uint8_t data[] = {static_cast<uint8_t>(reg), 1, val};
  m_bus->sendPacket(m_id, HerkulexCommand::RamWrite, data, 3);
}


void HerkulexServo::writeRam2(HerkulexRamRegister reg, uint16_t val) {
  uint8_t val1, val2;
  val1 = static_cast<uint8_t>(val);
  val2 = static_cast<uint8_t>(val >> 8);

  uint8_t data[] = {static_cast<uint8_t>(reg), 2, val1, val2};
  m_bus->sendPacket(m_id, HerkulexCommand::RamWrite, data, 4);
}


void HerkulexServo::writeEep(HerkulexEepRegister reg, uint8_t val) {
  uint8_t data[] = {static_cast<uint8_t>(reg), 1, val};
  m_bus->sendPacket(m_id, HerkulexCommand::EepWrite, data, 3);
}


void HerkulexServo::writeEep2(HerkulexEepRegister reg, uint16_t val) {
  uint8_t val1, val2;
  val1 = static_cast<uint8_t>(val);
  val2 = static_cast<uint8_t>(val >> 8);

  uint8_t data[] = {static_cast<uint8_t>(reg), 2, val1, val2};
  m_bus->sendPacket(m_id, HerkulexCommand::EepWrite, data, 4);
}


uint8_t HerkulexServo::readRam(HerkulexRamRegister reg) {
  uint8_t data[] = {static_cast<uint8_t>(reg), 1};
  m_bus->sendPacketAndReadResponse(m_response, m_id, HerkulexCommand::RamRead, data, 2);

  return m_response.data[2];
}


uint16_t HerkulexServo::readRam2(HerkulexRamRegister reg) {
  uint8_t data[] = {static_cast<uint8_t>(reg), 2};
  m_bus->sendPacketAndReadResponse(m_response, m_id, HerkulexCommand::RamRead, data, 2);

  uint16_t ret = m_response.data[3];
  ret = ret << 8;
  ret = ret | m_response.data[2];

  return ret;
}


uint8_t HerkulexServo::readEep(HerkulexEepRegister reg) {
  uint8_t data[] = {static_cast<uint8_t>(reg), 1};
  m_bus->sendPacketAndReadResponse(m_response, m_id, HerkulexCommand::EepRead, data, 2);

  return m_response.data[2];
}

uint16_t HerkulexServo::readEep2(HerkulexEepRegister reg) {
  uint8_t data[] = {static_cast<uint8_t>(reg), 2};
  m_bus->sendPacketAndReadResponse(m_response, m_id, HerkulexCommand::EepRead, data, 2);

  uint16_t ret = m_response.data[3];
  ret = ret << 8;
  ret = ret | m_response.data[2];

  return ret;
}


void HerkulexServo::getStatus(HerkulexStatusError &error, HerkulexStatusDetail &detail) {
  m_bus->sendPacketAndReadResponse(m_response, m_id, HerkulexCommand::Stat);
  error = static_cast<HerkulexStatusError>(m_response.data[0]);
  detail = static_cast<HerkulexStatusDetail>(m_response.data[1]);
}


void HerkulexServo::reboot() {
  m_bus->sendPacket(m_id, HerkulexCommand::Reboot);
}


void HerkulexServo::rollbackToFactoryDefaults(bool skipID, bool skipBaud) {
  uint8_t data[2] = {};
  data[0] = skipID ? 1 : 0;
  data[1] = skipBaud ? 1 : 0;
  m_bus->sendPacket(m_id, HerkulexCommand::Rollback, data, 2);
}


void HerkulexServo::setLedColor(HerkulexLed color) {
  writeRam(HerkulexRamRegister::LedControl, static_cast<uint8_t>(color));
}


void HerkulexServo::setTorqueOn() {
  writeRam(HerkulexRamRegister::TorqueControl, 0x60);
}


void HerkulexServo::setTorqueOff() {
  writeRam(HerkulexRamRegister::TorqueControl, 0x00);
}


void HerkulexServo::setBrake() {
  writeRam(HerkulexRamRegister::TorqueControl, 0x40);
}
