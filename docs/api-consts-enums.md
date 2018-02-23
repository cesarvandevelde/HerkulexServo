# Constants and Enums
This document lists all the constants and enumerations used throughout the
library.

## `HerkulexCommand`

| Members    |
|------------|
| `EepWrite` |
| `EepRead`  |
| `RamWrite` |
| `RamRead`  |
| `IJog`     |
| `SJog`     |
| `Stat`     |
| `Rollback` |
| `Reboot`   |


## `HerkulexEepRegister`

| Members          |                        |                            |
|------------------|------------------------|----------------------------|
| `ModelNo1`       | `AccelerationRatio`    | `PositionKd`               |
| `ModelNo2`       | `MaxAccelerationTime`  | `PositionKi`               |
| `Version1`       | `DeadZone`             | `PositionFF1stGain`        |
| `Version2`       | `SaturatorOffset`      | `PositionFF2ndGain`        |
| `BaudRate`       | `SaturatorSlope`       | `LedBlinkPeriod`           |
| `ID`             | `PwmOffset`            | `AdcFaultCheckPeriod`      |
| `AckPolicy`      | `MinPwm`               | `PacketGarbageCheckPeriod` |
| `AlarmLedPolicy` | `MaxPwm`               | `StopDetectionPeriod`      |
| `TorquePolicy`   | `OverloadPwmThreshold` | `OverloadProtectionPeriod` |
| `MaxTemperature` | `MinPosition`          | `StopThreshold`            |
| `MinVoltage`     | `MaxPosition`          | `InPositionMargin`         |
| `MaxVoltage`     | `PositionKp`           | `CalibrationDifference`    |


## `HerkulexRamRegister`

| Members                |                            |                          |
|------------------------|----------------------------|--------------------------|
| `ID`                   | `MinPosition`              | `StatusDetail`           |
| `AckPolicy`            | `MaxPosition`              | `TorqueControl`          |
| `AlarmLedPolicy`       | `PositionKp`               | `LedControl`             |
| `TorquePolicy`         | `PositionKd`               | `Voltage`                |
| `MaxTemperature`       | `PositionKi`               | `Temperature`            |
| `MinVoltage`           | `PositionFF1stGain`        | `CurrentControlMode`     |
| `MaxVoltage`           | `PositionFF2ndGain`        | `Tick`                   |
| `AccelerationRatio`    | `LedBlinkPeriod`           | `CalibratedPosition`     |
| `MaxAccelerationTime`  | `AdcFaultCheckPeriod`      | `AbsolutePosition`       |
| `DeadZone`             | `PacketGarbageCheckPeriod` | `DifferentialPosition`   |
| `SaturatorOffset`      | `StopDetectionPeriod`      | `Pwm`                    |
| `SaturatorSlope`       | `OverloadProtectionPeriod` | `AbsoluteGoalPosition`   |
| `PwmOffset`            | `StopThreshold`            | `AbsoluteDesiredTrajPos` |
| `MinPwm`               | `InPositionMargin`         | `DesiredVelocity`        |
| `MaxPwm`               | `CalibrationDifference`    |                          |
| `OverloadPwmThreshold` | `StatusError`              |                          |


## `HerkulexLed`

| Members  |
|----------|
| `Off`    |
| `Red`    |
| `Green`  |
| `Blue`   |
| `Yellow` |
| `Cyan`   |
| `Purple` |
| `White`  |
| `Ignore` |


## `HerkulexPacketError`

| Members    |
|------------|
| `None`     |
| `Timeout`  |
| `Length`   |
| `Command`  |
| `Checksum` |


## `HerkulexStatusError`

| Members            |
|--------------------|
| `None`             |
| `InputVoltage`     |
| `PotLimit`         |
| `TemperatureLimit` |
| `InvalidPacket`    |
| `Overload`         |
| `DriverFault`      |
| `EEPDistorted`     |
| `Reserved`         |


## `HerkulexStatusDetail`

| Members         |
|-----------------|
| None            |
| Moving          |
| InPosition      |
| ChecksumError   |
| UnknownCommand  |
| ExceedRegRange  |
| GarbageDetected |
| MotorOn         |
| Reserved        |
