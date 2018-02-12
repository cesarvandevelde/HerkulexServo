#ifndef __HERKULEXSERVO_H__
#define __HERKULEXSERVO_H__

#include <Arduino.h>

#define DRS_BROADCAST_ID 0xFE

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

class HerkulexServoBus {
  public:
    HerkulexServoBus();
    void begin(const Stream &serial_connection);
    void sendPacket(uint8_t id, uint8_t cmd, const uint8_t* data, uint8_t data_len);

  protected:
    Stream& m_serial;
};

class HerkulexServo {
  public:
    HerkulexServo(HerkulexServoBus &bus, uint8_t id);

  protected:
    HerkulexServoBus& m_bus;
    uint8_t m_id;
};

#endif
