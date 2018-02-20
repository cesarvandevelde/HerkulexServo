#ifndef __HERKULEXSERVO_H__
#define __HERKULEXSERVO_H__

#include <Arduino.h>
#include <CircularBuffer.h>

#define HERKULEX_BROADCAST_ID 0xFE

#ifndef HERKULEX_PACKET_RX_TIMEOUT
#define HERKULEX_PACKET_RX_TIMEOUT 100  // microseconds
#endif

#ifndef HERKULEX_PACKET_RETRIES
#define HERKULEX_PACKET_RETRIES 6
#endif

#ifndef HERKULEX_PACKET_RESEND_DELAY
#define HERKULEX_PACKET_RESEND_DELAY 70  // microseconds
#endif

#ifndef HERKULEX_PACKET_RX_MAX_DATA
#define HERKULEX_PACKET_RX_MAX_DATA 10  // bytes
#endif

#ifndef HERKULEX_SERIAL_RX_BUFFER
#define HERKULEX_SERIAL_RX_BUFFER 30  // bytes
#endif

#ifndef HERKULEX_MAX_SCHEDULED_SERVOS
#define HERKULEX_MAX_SCHEDULED_SERVOS 10
#endif

#ifndef HERKULEX_SERIAL_TX_BUFFER
#define HERKULEX_SERIAL_TX_BUFFER (1 + HERKULEX_MAX_SCHEDULED_SERVOS * 5)
#endif

#ifdef HERKULEX_DEBUG
#define HERKULEX_DEBUG_RX_STATS
#endif


enum class HerkulexCommand {
  EepWrite  = 0x01,
  EepRead   = 0x02,
  RamWrite  = 0x03,
  RamRead   = 0x04,
  IJog      = 0x05,
  SJog      = 0x06,
  Stat      = 0x07,
  Rollback  = 0x08,
  Reboot    = 0x09
};

enum class HerkulexEepRegister {
  ModelNo1                 = 0,
  ModelNo2                 = 1,
  Version1                 = 2,
  Version2                 = 3,
  BaudRate                 = 4,
  ID                       = 6,
  AckPolicy                = 7,
  AlarmLedPolicy           = 8,
  TorquePolicy             = 9,
  MaxTemperature           = 11,
  MinVoltage               = 12,
  MaxVoltage               = 13,
  AccelerationRatio        = 14,
  MaxAccelerationTime      = 15,
  DeadZone                 = 16,
  SaturatorOffset          = 17,
  SaturatorSlope           = 18,
  PwmOffset                = 20,
  MinPwm                   = 21,
  MaxPwm                   = 22,
  OverloadPwmThreshold     = 24,
  MinPosition              = 26,
  MaxPosition              = 28,
  PositionKp               = 30,
  PositionKd               = 32,
  PositionKi               = 34,
  PositionFF1stGain        = 36,
  PositionFF2ndGain        = 38,
  LedBlinkPeriod           = 44,
  AdcFaultCheckPeriod      = 45,
  PacketGarbageCheckPeriod = 46,
  StopDetectionPeriod      = 47,
  OverloadProtectionPeriod = 48,
  StopThreshold            = 49,
  InPositionMargin         = 50,
  CalibrationDifference    = 53
};

enum class HerkulexRamRegister {
  ID                       = 0,
  AckPolicy                = 1,
  AlarmLedPolicy           = 2,
  TorquePolicy             = 3,
  MaxTemperature           = 5,
  MinVoltage               = 6,
  MaxVoltage               = 7,
  AccelerationRatio        = 8,
  MaxAccelerationTime      = 9,
  DeadZone                 = 10,
  SaturatorOffset          = 11,
  SaturatorSlope           = 12,
  PwmOffset                = 14,
  MinPwm                   = 15,
  MaxPwm                   = 16,
  OverloadPwmThreshold     = 18,
  MinPosition              = 20,
  MaxPosition              = 22,
  PositionKp               = 24,
  PositionKd               = 26,
  PositionKi               = 28,
  PositionFF1stGain        = 30,
  PositionFF2ndGain        = 32,
  LedBlinkPeriod           = 38,
  AdcFaultCheckPeriod      = 39,
  PacketGarbageCheckPeriod = 40,
  StopDetectionPeriod      = 41,
  OverloadProtectionPeriod = 42,
  StopThreshold            = 43,
  InPositionMargin         = 44,
  CalibrationDifference    = 47,
  StatusError              = 48,
  StatusDetail             = 49,
  TorqueControl            = 52,
  LedControl               = 53,
  Voltage                  = 54,
  Temperature              = 55,
  CurrentControlMode       = 56,
  Tick                     = 57,
  CalibratedPosition       = 58,
  AbsolutePosition         = 60,
  DifferentialPosition     = 62,
  Pwm                      = 64,
  AbsoluteGoalPosition     = 68,
  AbsoluteDesiredTrajPos   = 70,
  DesiredVelocity          = 72
};

enum class HerkulexLed : uint8_t {
  Off    = 0x00,
  Red    = 0x04,
  Green  = 0x01,
  Blue   = 0x02,
  Yellow = 0x05,
  Cyan   = 0x03,
  Purple = 0x06,
  White  = 0x07,
  Ignore = 0xFF
};


#define ENUM_FLAG_OPERATOR(T,X) inline T operator X (T lhs, T rhs) { return (T) (static_cast<uint8_t>(lhs) X static_cast<uint8_t>(rhs)); }
#define ENUM_FLAG_ASSIGNMENT_OPERATOR(T,X,Y) inline T& operator X (T& lhs, const T& rhs) { return lhs = (lhs Y rhs); }


enum class HerkulexPacketError : uint8_t {
  None     = 0,
  Timeout  = 0b00000001,
  Length   = 0b00000010,
  Command  = 0b00000100,
  Checksum = 0b00001000
};

ENUM_FLAG_OPERATOR(HerkulexPacketError, |)
ENUM_FLAG_OPERATOR(HerkulexPacketError, &)
ENUM_FLAG_ASSIGNMENT_OPERATOR(HerkulexPacketError, |=, |)


enum class HerkulexStatusError : uint8_t {
  None             = 0,
  InputVoltage     = 0b00000001,
  PotLimit         = 0b00000010,
  TemperatureLimit = 0b00000100,
  InvalidPacket    = 0b00001000,
  Overload         = 0b00010000,
  DriverFault      = 0b00100000,
  EEPDistorted     = 0b01000000,
  Reserved         = 0b10000000
};

ENUM_FLAG_OPERATOR(HerkulexStatusError, |)
ENUM_FLAG_OPERATOR(HerkulexStatusError, &)
ENUM_FLAG_ASSIGNMENT_OPERATOR(HerkulexStatusError, |=, |)


enum class HerkulexStatusDetail : uint8_t {
  None            = 0,
  Moving          = 0b00000001,
  InPosition      = 0b00000010,
  ChecksumError   = 0b00000100,
  UnknownCommand  = 0b00001000,
  ExceedRegRange  = 0b00010000,
  GarbageDetected = 0b00100000,
  MotorOn         = 0b01000000,
  Reserved        = 0b10000000
};

ENUM_FLAG_OPERATOR(HerkulexStatusDetail, |)
ENUM_FLAG_OPERATOR(HerkulexStatusDetail, &)
ENUM_FLAG_ASSIGNMENT_OPERATOR(HerkulexStatusDetail, |=, |)


enum class HerkulexScheduleState : uint8_t {
  None = 0,
  IndividualMove,
  SynchronizedMove
};


struct HerkulexPacket {
  uint8_t size;
  uint8_t id;
  HerkulexCommand cmd;
  uint8_t checksum1;
  uint8_t checksum2;
  uint8_t data[HERKULEX_PACKET_RX_MAX_DATA];
  uint8_t status_error;
  uint8_t status_detail;
  HerkulexPacketError error;
};


class HerkulexServo;


class HerkulexServoBus {
  public:
    HerkulexServoBus(Stream &serial_connection);
    void sendPacket(uint8_t id, HerkulexCommand cmd, const uint8_t* data = nullptr, uint8_t data_len = 0);
    bool sendPacketAndReadResponse(HerkulexPacket &resp, uint8_t id, HerkulexCommand cmd, const uint8_t* data = nullptr, uint8_t data_len = 0);

    void update();
    bool getPacket(HerkulexPacket &packet);

#ifdef HERKULEX_DEBUG_RX_STATS
    void resetRxStatistics();
    void printRxStatistics(Stream &output);
#endif

    void prepareIndividualMove();
    void prepareSynchronizedMove(uint8_t playtime);
    void executeMove();

  protected:
    Stream* m_serial;
    CircularBuffer<uint8_t, HERKULEX_SERIAL_RX_BUFFER> m_rx_buffer;
    unsigned long m_last_serial = 0;

    HerkulexPacket m_rx_packet = {};
    bool m_rx_packet_ready = false;

    uint8_t m_tx_buffer[HERKULEX_SERIAL_TX_BUFFER];
    uint8_t m_move_tags = 0;
    HerkulexScheduleState m_schedule_state = HerkulexScheduleState::None;

#ifdef HERKULEX_DEBUG_RX_STATS
    uint16_t m_stats_attempts[HERKULEX_PACKET_RETRIES] = {};
    uint16_t m_stats_fails = 0;
    uint16_t m_stats_totals = 0;
#endif

    void processPacket(bool timeout);

    friend class HerkulexServo;
};

class HerkulexServo {
  public:
    HerkulexServo(HerkulexServoBus &bus, uint8_t id);

    void writeRam(HerkulexRamRegister reg, uint8_t val);
    void writeRam2(HerkulexRamRegister reg, uint16_t val);
    void writeEep(HerkulexEepRegister reg, uint8_t val);
    void writeEep2(HerkulexEepRegister reg, uint16_t val);

    uint8_t  readRam(HerkulexRamRegister reg);
    uint16_t readRam2(HerkulexRamRegister reg);
    uint8_t  readEep(HerkulexEepRegister reg);
    uint16_t readEep2(HerkulexEepRegister reg);

    void reboot();
    void rollbackToFactoryDefaults(bool skipID, bool skipBaud);

    uint8_t  getID();
    void     getStatus(HerkulexStatusError &status_error, HerkulexStatusDetail &status_detail);
    uint16_t getPosition();
    uint16_t getRawPosition();

    void setLedColor(HerkulexLed color);

    void setTorqueOn();
    void setTorqueOff();
    void setBrake();

    void enablePositionControlMode();
    void enableSpeedControlMode();
    void setPosition(uint16_t pos, uint8_t playtime = 0, HerkulexLed led = HerkulexLed::Ignore);
    void setSpeed(int16_t speed, uint8_t playtime = 0, HerkulexLed led = HerkulexLed::Ignore);

  protected:
    HerkulexServoBus* m_bus;
    uint8_t m_id;
    HerkulexLed m_led = HerkulexLed::Off;
    bool m_position_control_mode = true;

    static HerkulexPacket m_response;
    static uint8_t m_tx_buffer[5];

    void jog(uint8_t jog_lsb, uint8_t jog_msb, uint8_t set, uint8_t playtime);
};

#endif
