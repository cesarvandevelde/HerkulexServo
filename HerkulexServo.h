#ifndef __HERKULEXSERVO_H__
#define __HERKULEXSERVO_H__

#include <Arduino.h>
#include <CircularBuffer.h>

#define DRS_BROADCAST_ID 0xFE

#ifndef DRS_PACKET_RX_TIMEOUT
#define DRS_PACKET_RX_TIMEOUT 500  // microseconds
#endif

#ifndef DRS_PACKET_RETRIES
#define DRS_PACKET_RETRIES 5
#endif

#ifndef DRS_PACKET_RESEND_DELAY
#define DRS_PACKET_RESEND_DELAY 70  // microseconds
#endif

#ifndef DRS_PACKET_RX_MAX_DATA
#define DRS_PACKET_RX_MAX_DATA 10  // bytes
#endif

#ifndef DRS_SERIAL_RX_BUFFER
#define DRS_SERIAL_RX_BUFFER 30  // bytes
#endif

#ifndef DRS_PACKET_RX_BUFFER
#define DRS_PACKET_RX_BUFFER 3  // HerkulexPacket structs
#endif


enum DRSCommand {
  DRS_CMD_EEP_WRITE = 0x01,
  DRS_CMD_EEP_READ  = 0x02,
  DRS_CMD_RAM_WRITE = 0x03,
  DRS_CMD_RAM_READ  = 0x04,
  DRS_CMD_I_JOG     = 0x05,
  DRS_CMD_S_JOG     = 0x06,
  DRS_CMD_STAT      = 0x07,
  DRS_CMD_ROLLBACK  = 0x08,
  DRS_CMD_REBOOT    = 0x09
};

enum DRSEEPRegister {
  DRS_EEP_REG_MODEL_NO1                   = 0,
  DRS_EEP_REG_MODEL_NO2                   = 1,
  DRS_EEP_REG_VERSION1                    = 2,
  DRS_EEP_REG_VERSION2                    = 3,
  DRS_EEP_REG_BAUD_RATE                   = 4,
  DRS_EEP_REG_ID                          = 6,
  DRS_EEP_REG_ACK_POLICY                  = 7,
  DRS_EEP_REG_ALARM_LED_POLICY            = 8,
  DRS_EEP_REG_TORQUE_POLICY               = 9,
  DRS_EEP_REG_MAX_TEMPERATURE             = 11,
  DRS_EEP_REG_MIN_VOLTAGE                 = 12,
  DRS_EEP_REG_MAX_VOLTAGE                 = 13,
  DRS_EEP_REG_ACCELARATION_RATIO          = 14,
  DRS_EEP_REG_MAX_ACCELERATION_TIME       = 15,
  DRS_EEP_REG_DEAD_ZONE                   = 16,
  DRS_EEP_REG_SATURATOR_OFFSET            = 17,
  DRS_EEP_REG_SATURATOR_SLOPE             = 18,
  DRS_EEP_REG_PWM_OFFSET                  = 20,
  DRS_EEP_REG_MIN_PWM                     = 21,
  DRS_EEP_REG_MAX_PWM                     = 22,
  DRS_EEP_REG_OVERLOAD_PWM_THRESHOLD      = 24,
  DRS_EEP_REG_MIN_POSITION                = 26,
  DRS_EEP_REG_MAX_POSITION                = 28,
  DRS_EEP_REG_POSITION_KP                 = 30,
  DRS_EEP_REG_POSITION_KD                 = 32,
  DRS_EEP_REG_POSITION_KI                 = 34,
  DRS_EEP_REG_POSITION_FF_1ST_GAIN        = 36,
  DRS_EEP_REG_POSITION_FF_2ND_GAIN        = 38,
  DRS_EEP_REG_LED_BLINK_PERIOD            = 44,
  DRS_EEP_REG_ADC_FAULT_CHECK_PERIOD      = 45,
  DRS_EEP_REG_PACKET_GARBAGE_CHECK_PERIOD = 46,
  DRS_EEP_REG_STOP_DETECTION_PERIOD       = 47,
  DRS_EEP_REG_OVERLOAD_PROTECTION_PERIOD  = 48,
  DRS_EEP_REG_STOP_THRESHOLD              = 49,
  DRS_EEP_REG_INPOSITION_MARGIN           = 50,
  DRS_EEP_REG_CALIBRATION_DIFFERENCE      = 53
};

enum DRSRAMRegister {
  DRS_RAM_REG_ID                          = 0,
  DRS_RAM_REG_ACK_POLICY                  = 1,
  DRS_RAM_REG_ALARM_LED_POLICY            = 2,
  DRS_RAM_REG_TORQUE_POLICY               = 3,
  DRS_RAM_REG_MAX_TEMPERATURE             = 5,
  DRS_RAM_REG_MIN_VOLTAGE                 = 6,
  DRS_RAM_REG_MAX_VOLTAGE                 = 7,
  DRS_RAM_REG_ACCELARATION_RATIO          = 8,
  DRS_RAM_REG_MAX_ACCELERATION_TIME       = 9,
  DRS_RAM_REG_DEAD_ZONE                   = 10,
  DRS_RAM_REG_SATURATOR_OFFSET            = 11,
  DRS_RAM_REG_SATURATOR_SLOPE             = 12,
  DRS_RAM_REG_PWM_OFFSET                  = 14,
  DRS_RAM_REG_MIN_PWM                     = 15,
  DRS_RAM_REG_MAX_PWM                     = 16,
  DRS_RAM_REG_OVERLOAD_PWM_THRESHOLD      = 18,
  DRS_RAM_REG_MIN_POSITION                = 20,
  DRS_RAM_REG_MAX_POSITION                = 22,
  DRS_RAM_REG_POSITION_KP                 = 24,
  DRS_RAM_REG_POSITION_KD                 = 26,
  DRS_RAM_REG_POSITION_KI                 = 28,
  DRS_RAM_REG_POSITION_FF_1ST_GAIN        = 30,
  DRS_RAM_REG_POSITION_FF_2ND_GAIN        = 32,
  DRS_RAM_REG_LED_BLINK_PERIOD            = 38,
  DRS_RAM_REG_ADC_FAULT_CHECK_PERIOD      = 39,
  DRS_RAM_REG_PACKET_GARBAGE_CHECK_PERIOD = 40,
  DRS_RAM_REG_STOP_DETECTION_PERIOD       = 41,
  DRS_RAM_REG_OVERLOAD_PROTECTION_PERIOD  = 42,
  DRS_RAM_REG_STOP_THRESHOLD              = 43,
  DRS_RAM_REG_INPOSITION_MARGIN           = 44,
  DRS_RAM_REG_CALIBRATION_DIFFERENCE      = 47,
  DRS_RAM_REG_STATUS_ERROR                = 48,
  DRS_RAM_REG_STATUS_DETAIL               = 49,
  DRS_RAM_REG_TORQUE_CONTROL              = 52,
  DRS_RAM_REG_LED_CONTROL                 = 53,
  DRS_RAM_REG_VOLTAGE                     = 54,
  DRS_RAM_REG_TEMPERATURE                 = 55,
  DRS_RAM_REG_CURRENT_CONTROL_MODE        = 56,
  DRS_RAM_REG_TICK                        = 57,
  DRS_RAM_REG_CALIBRATED_POSITION         = 58,
  DRS_RAM_REG_ABSOLUTE_POSITION           = 60,
  DRS_RAM_REG_DIFFERENTIAL_POSITION       = 62,
  DRS_RAM_REG_PWM                         = 64,
  DRS_RAM_REG_ABSOLUTE_GOAL_POSITION      = 68,
  DRS_RAM_REG_ABSOLUTE_DESIRED_TRAJ_POS   = 70,
  DRS_RAM_REG_DESIRED_VELOCITY            = 72
};

enum class HerkulexLed : uint8_t {
  Off = 0x00,
  Red = 0x04,
  Green = 0x01,
  Blue = 0x02,
  Yellow = 0x05,
  Cyan = 0x03,
  Purple = 0x06,
  White = 0x07
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


struct HerkulexPacket {
  uint8_t size;
  uint8_t id;
  uint8_t cmd;
  uint8_t checksum1;
  uint8_t checksum2;
  uint8_t data[DRS_PACKET_RX_MAX_DATA];
  uint8_t status_error;
  uint8_t status_detail;
  HerkulexPacketError error;
};


class HerkulexServoBus {
  public:
    HerkulexServoBus();
    void begin(Stream &serial_connection);
    void sendPacket(uint8_t id, uint8_t cmd, const uint8_t* data = nullptr, uint8_t data_len = 0);
    bool sendPacketAndReadResponse(HerkulexPacket &resp, uint8_t id, uint8_t cmd, const uint8_t* data = nullptr, uint8_t data_len = 0);

    void update();
    bool getPacket(HerkulexPacket &packet);

  protected:
    Stream* m_serial;
    CircularBuffer<HerkulexPacket, DRS_PACKET_RX_BUFFER> m_packets;
    CircularBuffer<uint8_t, DRS_SERIAL_RX_BUFFER> m_buffer;
    unsigned long last_serial;

    void processPacket(bool timeout);

};

class HerkulexServo {
  public:
    HerkulexServo(HerkulexServoBus &bus, uint8_t id);

    void writeRam(uint8_t reg, uint8_t val);
    void writeRam(uint8_t reg, uint8_t val1, uint8_t val2);
    void writeEep(uint8_t reg, uint8_t val);
    void writeEep(uint8_t reg, uint8_t val1, uint8_t val2);

    uint8_t  readRam(uint8_t reg);
    uint16_t readRam2(uint8_t reg);

    void getStatus(HerkulexStatusError &status_error, HerkulexStatusDetail &status_detail);
    void reboot();
    void rollbackToFactoryDefaults(bool skipID, bool skipBaud);

    void setLedColor(HerkulexLed color);

  protected:
    HerkulexServoBus* m_bus;
    uint8_t m_id;
    HerkulexPacket m_response = {};
};

#endif
