#include "HerkulexServo.h"


HerkulexServoBus::HerkulexServoBus(Stream &serial_connection) : m_serial(&serial_connection) {}


void HerkulexServoBus::sendPacket(uint8_t id, HerkulexCommand cmd, const uint8_t* data, uint8_t data_len) {
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

bool HerkulexServoBus::sendPacketAndReadResponse(HerkulexPacket &resp, uint8_t id, HerkulexCommand cmd, const uint8_t* data, uint8_t data_len) {
  bool success = false;
  unsigned long time_started;

  update();
  m_rx_packet_ready = false;

#ifdef HERKULEX_DEBUG_RX_STATS
  m_stats_totals++;
#endif

  for (uint8_t attempts = 0; attempts < HERKULEX_PACKET_RETRIES; attempts++) {
    sendPacket(id, cmd, data, data_len);

    time_started = millis();
    while ( !getPacket(resp) && ((millis() - time_started) < HERKULEX_PACKET_RX_TIMEOUT) ) {
    //while ( !getPacket(resp) ) {
      update();
    }

    if ( (resp.error == HerkulexPacketError:: None) && (resp.id == id) && (resp.cmd == cmd) ) {
      success = true;

#ifdef HERKULEX_DEBUG_RX_STATS
      m_stats_attempts[attempts]++;
#endif

      break;
    } else {
      delayMicroseconds(HERKULEX_PACKET_RESEND_DELAY);
    }
  }

#ifdef HERKULEX_DEBUG_RX_STATS
  if (!success) { m_stats_fails++; }
#endif

  return success;
}


void HerkulexServoBus::update() {
  if (m_serial->available() > 0) {
    // push data to buffer
    while (m_serial->available()) {
      m_rx_buffer.push(m_serial->read());
      m_last_serial = micros();
    }

    // remove characters before header
    while (m_rx_buffer.size() >= 2) {
      if (m_rx_buffer[0] != 0xFF || m_rx_buffer[1] != 0xFF) {
        m_rx_buffer.shift();
      } else {
        break;
      }
    }

    // see if packet is complete
    if (m_rx_buffer.size() >= 9) {  // 7 (min packet length) + 2 (status data)
      uint8_t packet_len = m_rx_buffer[2];
      if (m_rx_buffer.size() >= packet_len) {
        processPacket(false);
        return;
      }
    }
  }

  if (m_rx_buffer.size() > 0) {
    unsigned long now = micros();

    if (now - m_last_serial > HERKULEX_PACKET_RX_TIMEOUT) {
      processPacket(true);
      m_last_serial = now;
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
  m_rx_packet = {};

  if (timeout) {
    m_rx_packet.error |= HerkulexPacketError::Timeout;
  }

  uint8_t bytes_to_process;
  uint8_t checksum1;
  uint8_t checksum2;

  if (m_rx_buffer.size() >= 9) {
    // check length field
    if (m_rx_buffer[2] > m_rx_buffer.size()) {
      bytes_to_process = m_rx_buffer.size();
      m_rx_packet.error |= HerkulexPacketError::Length;
    } else {
      bytes_to_process = m_rx_buffer[2];
    }
  } else {
    bytes_to_process = m_rx_buffer.size();
    m_rx_packet.error |= HerkulexPacketError::Length;
  }

  HerkulexParserState state = HerkulexParserState::Header1;
  uint8_t data_idx = 0;

  while (bytes_to_process > 0) {
    uint8_t b = m_rx_buffer.shift();
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
        m_rx_packet.size = b;
        checksum1 = b;
        state = HerkulexParserState::ID;
        break;

      case HerkulexParserState::ID:
        m_rx_packet.id = b;
        checksum1 ^= b;
        state = HerkulexParserState::Command;
        break;

      case HerkulexParserState::Command:
        if (b > 0x40) {
          m_rx_packet.cmd = static_cast<HerkulexCommand>(b - 0x40);
        } else {
          m_rx_packet.cmd = static_cast<HerkulexCommand>(b);
          m_rx_packet.error |= HerkulexPacketError::Command;
        }
        checksum1 ^= b;

        state = HerkulexParserState::Checksum1;
        break;

      case HerkulexParserState::Checksum1:
        m_rx_packet.checksum1 = b;
        state = HerkulexParserState::Checksum2;
        break;

      case HerkulexParserState::Checksum2:
        m_rx_packet.checksum2 = b;
        state = HerkulexParserState::Data;
        break;

      case HerkulexParserState::Data:
        m_rx_packet.data[data_idx] = b;
        checksum1 ^= b;
        data_idx++;

        // clamp idx to size of data array
        if (data_idx >= HERKULEX_PACKET_RX_MAX_DATA) {
          data_idx = HERKULEX_PACKET_RX_MAX_DATA - 1;
        }
        break;
    }
  }

  if (data_idx >= 2) {
    m_rx_packet.status_error = m_rx_packet.data[data_idx - 2];
    m_rx_packet.status_detail = m_rx_packet.data[data_idx - 1];
  }

  checksum1 = checksum1 & 0xFE;
  checksum2 = (~checksum1) & 0xFE;

  if (m_rx_packet.checksum1 != checksum1 || m_rx_packet.checksum2 != checksum2) {
    m_rx_packet.error |= HerkulexPacketError::Checksum;
  }

  m_rx_packet_ready = true;
}


bool HerkulexServoBus::getPacket(HerkulexPacket &packet) {
  if (!m_rx_packet_ready) {
    return false;
  }

  packet = m_rx_packet;
  m_rx_packet_ready = false;

  return true;
}

#ifdef HERKULEX_DEBUG_RX_STATS
void HerkulexServoBus::resetRxStatistics() {
  m_stats_fails = 0;
  m_stats_totals = 0;

  for (uint8_t i = 0; i < HERKULEX_PACKET_RETRIES; i++) {
    m_stats_attempts[i] = 0;
  }
}


void HerkulexServoBus::printRxStatistics(Stream &output) {
  output.print(m_stats_fails);
  output.print(" fails out of ");
  output.print(m_stats_totals);
  output.println(" attempts.");

  for (uint8_t i = 0; i < HERKULEX_PACKET_RETRIES; i++) {
    output.print("-> ");
    output.print(i);
    output.print(" retries: ");
    output.println(m_stats_attempts[i]);
  }
}
#endif


void HerkulexServoBus::prepareIndividualMove() {
  m_schedule_state = HerkulexScheduleState::IndividualMove;
  m_move_tags = 0;
}


void HerkulexServoBus::prepareSynchronizedMove(uint8_t playtime) {
  m_schedule_state = HerkulexScheduleState::SynchronizedMove;
  m_tx_buffer[0] = playtime;
  m_move_tags = 0;
}


void HerkulexServoBus::executeMove() {
  uint8_t data_len;

  switch (m_schedule_state) {
    case HerkulexScheduleState::IndividualMove:
      data_len = m_move_tags * 5;
      sendPacket(HERKULEX_BROADCAST_ID, HerkulexCommand::IJog, m_tx_buffer, data_len);
      break;

    case HerkulexScheduleState::SynchronizedMove:
      data_len = 1 + m_move_tags * 4;
      sendPacket(HERKULEX_BROADCAST_ID, HerkulexCommand::SJog, m_tx_buffer, data_len);
      break;

    case HerkulexScheduleState::None:
      break;
  }

  m_schedule_state = HerkulexScheduleState::None;
  m_move_tags = 0;
}


HerkulexPacket HerkulexServo::m_response = {};
uint8_t HerkulexServo::m_tx_buffer[] = {};


HerkulexServo::HerkulexServo(HerkulexServoBus &bus, uint8_t id) : m_bus(&bus), m_id(id) {}


void HerkulexServo::writeRam(HerkulexRamRegister reg, uint8_t val) {
  m_tx_buffer[0] = static_cast<uint8_t>(reg);
  m_tx_buffer[1] = 1;
  m_tx_buffer[2] = val;

  m_bus->sendPacket(m_id, HerkulexCommand::RamWrite, m_tx_buffer, 3);
}


void HerkulexServo::writeRam2(HerkulexRamRegister reg, uint16_t val) {
  m_tx_buffer[0] = static_cast<uint8_t>(reg);
  m_tx_buffer[1] = 2;
  m_tx_buffer[2] = static_cast<uint8_t>(val);
  m_tx_buffer[3] = static_cast<uint8_t>(val >> 8);

  m_bus->sendPacket(m_id, HerkulexCommand::RamWrite, m_tx_buffer, 4);
}


void HerkulexServo::writeEep(HerkulexEepRegister reg, uint8_t val) {
  m_tx_buffer[0] = static_cast<uint8_t>(reg);
  m_tx_buffer[1] = 1;
  m_tx_buffer[2] = val;

  m_bus->sendPacket(m_id, HerkulexCommand::EepWrite, m_tx_buffer, 3);
}


void HerkulexServo::writeEep2(HerkulexEepRegister reg, uint16_t val) {
  m_tx_buffer[0] = static_cast<uint8_t>(reg);
  m_tx_buffer[1] = 2;
  m_tx_buffer[2] = static_cast<uint8_t>(val);
  m_tx_buffer[3] = static_cast<uint8_t>(val >> 8);

  m_bus->sendPacket(m_id, HerkulexCommand::EepWrite, m_tx_buffer, 4);
}


uint8_t HerkulexServo::readRam(HerkulexRamRegister reg) {
  m_tx_buffer[0] = static_cast<uint8_t>(reg);
  m_tx_buffer[1] = 1;

  m_bus->sendPacketAndReadResponse(m_response, m_id, HerkulexCommand::RamRead, m_tx_buffer, 2);

  return m_response.data[2];
}


uint16_t HerkulexServo::readRam2(HerkulexRamRegister reg) {
  m_tx_buffer[0] = static_cast<uint8_t>(reg);
  m_tx_buffer[1] = 2;

  m_bus->sendPacketAndReadResponse(m_response, m_id, HerkulexCommand::RamRead, m_tx_buffer, 2);

  uint16_t ret = m_response.data[3];
  ret = ret << 8;
  ret = ret | m_response.data[2];

  return ret;
}


uint8_t HerkulexServo::readEep(HerkulexEepRegister reg) {
  m_tx_buffer[0] = static_cast<uint8_t>(reg);
  m_tx_buffer[1] = 1;

  m_bus->sendPacketAndReadResponse(m_response, m_id, HerkulexCommand::EepRead, m_tx_buffer, 2);

  return m_response.data[2];
}

uint16_t HerkulexServo::readEep2(HerkulexEepRegister reg) {
  m_tx_buffer[0] = static_cast<uint8_t>(reg);
  m_tx_buffer[1] = 2;

  m_bus->sendPacketAndReadResponse(m_response, m_id, HerkulexCommand::EepRead, m_tx_buffer, 2);

  uint16_t ret = m_response.data[3];
  ret = ret << 8;
  ret = ret | m_response.data[2];

  return ret;
}


void HerkulexServo::reboot() {
  m_bus->sendPacket(m_id, HerkulexCommand::Reboot);
}


void HerkulexServo::rollbackToFactoryDefaults(bool skipID, bool skipBaud) {
  m_tx_buffer[0] = skipID ? 1 : 0;
  m_tx_buffer[1] = skipBaud ? 1 : 0;

  m_bus->sendPacket(m_id, HerkulexCommand::Rollback, m_tx_buffer, 2);
}


uint8_t HerkulexServo::getID() {
  return m_id;
}


void HerkulexServo::getStatus(HerkulexStatusError &error, HerkulexStatusDetail &detail) {
  m_bus->sendPacketAndReadResponse(m_response, m_id, HerkulexCommand::Stat);
  error = static_cast<HerkulexStatusError>(m_response.data[0]);
  detail = static_cast<HerkulexStatusDetail>(m_response.data[1]);
}

uint16_t HerkulexServo::getPosition() {
  return readRam2(HerkulexRamRegister::CalibratedPosition) & 0x03FF;
}


uint16_t HerkulexServo::getRawPosition() {
  return readRam2(HerkulexRamRegister::AbsolutePosition) & 0x03FF;
}


void HerkulexServo::setLedColor(HerkulexLed color) {
  if (color == HerkulexLed::Ignore) {
    return;
  }

  m_led = color;
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


void HerkulexServo::enablePositionControlMode() {
  if (m_position_control_mode) {
    return;
  }
  m_position_control_mode = true;

  uint8_t set;

  set = static_cast<uint8_t>(m_led) << 2;
  set |= 0x02;  // continuous rotation;
  set |= 0x20;  // jog invalid

  m_tx_buffer[0] = 0x00;  // jog LSB
  m_tx_buffer[1] = 0x00;  // jog MSB
  m_tx_buffer[2] = set;
  m_tx_buffer[3] = m_id;
  m_tx_buffer[4] = 0x00;  // playtime
  m_bus->sendPacket(HERKULEX_BROADCAST_ID, HerkulexCommand::IJog, m_tx_buffer, 5);
}


void HerkulexServo::enableSpeedControlMode() {
  if (!m_position_control_mode) {
    return;
  }
  m_position_control_mode = false;

  uint8_t set;

  set = static_cast<uint8_t>(m_led) << 2;
  set |= 0x20;  // jog invalid
  // continuous rotation bit (0x02) is implicitly set to 0;

  m_tx_buffer[0] = 0x00;  // jog LSB
  m_tx_buffer[1] = 0x00;  // jog MSB
  m_tx_buffer[2] = set;
  m_tx_buffer[3] = m_id;
  m_tx_buffer[4] = 0x00;  // playtime
  m_bus->sendPacket(HERKULEX_BROADCAST_ID, HerkulexCommand::IJog, m_tx_buffer, 5);
}


void HerkulexServo::setPosition(uint16_t pos, uint8_t playtime, HerkulexLed led) {
  if (!m_position_control_mode) {
    return;
  }

  uint8_t jog_lsb;
  uint8_t jog_msb;
  uint8_t set;

  jog_lsb = static_cast<uint8_t>(pos);
  jog_msb = static_cast<uint8_t>(pos >> 8);

  if (led != HerkulexLed::Ignore) {
    m_led = led;
    set = static_cast<uint8_t>(led) << 2;
  } else {
    set = static_cast<uint8_t>(m_led) << 2;
  }

  jog(jog_lsb, jog_msb, set, playtime);
}

void HerkulexServo::setSpeed(int16_t speed, uint8_t playtime, HerkulexLed led) {
  if (m_position_control_mode) {
    return;
  }

  uint8_t jog_lsb;
  uint8_t jog_msb;
  uint8_t set;
  uint16_t speed_raw;

  if (speed >= 0) {
    speed_raw = speed;
  } else {
    speed_raw = -speed;
  }
  speed_raw &= 0x03FF;  // clamp to max 1023

  jog_lsb = static_cast<uint8_t>(speed_raw);
  jog_msb = static_cast<uint8_t>(speed_raw >> 8);

  if (speed < 0) {
    jog_msb |= 0x40;
  }

  if (led != HerkulexLed::Ignore) {
    m_led = led;
    set = static_cast<uint8_t>(led) << 2;
  } else {
    set = static_cast<uint8_t>(m_led) << 2;
  }
  set |= 0x02;  // continuous rotation flag;

  jog(jog_lsb, jog_msb, set, playtime);
}

void HerkulexServo::jog(uint8_t jog_lsb, uint8_t jog_msb, uint8_t set, uint8_t playtime) {
  uint8_t idx_offset;

  switch (m_bus->m_schedule_state) {
    case HerkulexScheduleState::None:
      m_tx_buffer[0] = jog_lsb;
      m_tx_buffer[1] = jog_msb;
      m_tx_buffer[2] = set;
      m_tx_buffer[3] = m_id;
      m_tx_buffer[4] = playtime;
      m_bus->sendPacket(HERKULEX_BROADCAST_ID, HerkulexCommand::IJog, m_tx_buffer, 5);
      break;

    case HerkulexScheduleState::IndividualMove:
      if ( ((m_bus->m_move_tags + 1) * 5) > HERKULEX_SERIAL_TX_BUFFER) {
        // no room for another move tag, exit
        return;
      }

      idx_offset = m_bus->m_move_tags * 5;  // 5 bytes per tag

      // append data to bus tx buffer
      m_bus->m_tx_buffer[idx_offset    ] = jog_lsb;
      m_bus->m_tx_buffer[idx_offset + 1] = jog_msb;
      m_bus->m_tx_buffer[idx_offset + 2] = set;
      m_bus->m_tx_buffer[idx_offset + 3] = m_id;
      m_bus->m_tx_buffer[idx_offset + 4] = playtime;

      m_bus->m_move_tags++;
      break;

    case HerkulexScheduleState::SynchronizedMove:
      if ( (1 + (m_bus->m_move_tags + 1) * 4) > HERKULEX_SERIAL_TX_BUFFER) {
        // no room for another move tag, exit
        return;
      }

      idx_offset = 1 + m_bus->m_move_tags * 4;  // 4 bytes per tag, 1 byte offset for time

      // append data to bus tx buffer
      m_bus->m_tx_buffer[idx_offset    ] = jog_lsb;
      m_bus->m_tx_buffer[idx_offset + 1] = jog_msb;
      m_bus->m_tx_buffer[idx_offset + 2] = set;
      m_bus->m_tx_buffer[idx_offset + 3] = m_id;

      m_bus->m_move_tags++;
      break;
  }

}
