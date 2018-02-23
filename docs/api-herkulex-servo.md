# HerkulexServo class
The `HerkulexServo` class provides a simple interface to manipulate a single
servo on the bus. The class provides low-level methods for manipulating
registers directly, as well as a number of higher level methods for performing
common tasks.

## `HerkulexServo::HerkulexServo()`
```C++
HerkulexServo::HerkulexServo(HerkulexServoBus &bus, uint8_t id);
```
Constructs a `HerkulexServo` instance representing a servo with the specified
ID.

| Param | Description                           |
|-------|---------------------------------------|
| `bus` | Reference to a `HerkulexBus` instance |
| `id`  | ID of the servo                       |


## `HerkulexServo::writeRam()`
```C++
void HerkulexServo::writeRam(HerkulexRamRegister reg, uint8_t val);
```
Write a single byte to a RAM (volatile memory) registry. Affects the behavior of
the servo immediately.

| Param | Description                                              |
|-------|----------------------------------------------------------|
| `reg` | Address of the registry (see `HerkulexRamRegister` enum) |
| `val` | Value to write                                           |


## `HerkulexServo::writeRam2()`
```C++
void HerkulexServo::writeRam2(HerkulexRamRegister reg, uint16_t val);
```
Write a two-byte to a RAM (volatile memory) registry. This method takes
endianness into account. Affects the behavior of the servo immediately.

| Param | Description                                              |
|-------|----------------------------------------------------------|
| `reg` | Address of the registry (see `HerkulexRamRegister` enum) |
| `val` | Value to write                                           |


## `HerkulexServo::writeEep()`
```C++
void HerkulexServo::writeEep(HerkulexEepRegister reg, uint8_t val);
```
Write a single byte to an EEPROM (non-volatile memory) registry. Only affects
the behavior of the servo after a reboot.

| Param | Description                                              |
|-------|----------------------------------------------------------|
| `reg` | Address of the registry (see `HerkulexEepRegister` enum) |
| `val` | Value to write                                           |


## `HerkulexServo::writeEep2()`
```C++
void HerkulexServo::writeEep2(HerkulexEepRegister reg, uint16_t val);
```
Write a two-byte value to an EEPROM (non-volatile memory) registry. This method
takes endianness into account.  Only affects the behavior of the servo after a
reboot.

| Param | Description                                              |
|-------|----------------------------------------------------------|
| `reg` | Address of the registry (see `HerkulexEepRegister` enum) |
| `val` | Value to write                                           |


## `HerkulexServo::readRam()`
```C++
uint8_t HerkulexServo::readRam(HerkulexRamRegister reg);
```
Reads a single byte from a RAM (volatile memory) registry.

| Param  | Description                                              |
|--------|----------------------------------------------------------|
| `reg`  | Address of the registry (see `HerkulexRamRegister` enum) |
| return | Value of the registry                                    |


## `HerkulexServo::readRam2()`
```C++
uint16_t HerkulexServo::readRam2(HerkulexRamRegister reg);
```
Reads a two-byte value from a RAM (volatile memory) registry. This method
takes endianness into account.

| Param  | Description                                              |
|--------|----------------------------------------------------------|
| `reg`  | Address of the registry (see `HerkulexRamRegister` enum) |
| return | Value of the registry                                    |


## `HerkulexServo::readEep()`
```C++
uint8_t HerkulexServo::readEep(HerkulexEepRegister reg);
```
Reads a single byte from an EEPROM (volatile memory) registry.

| Param  | Description                                              |
|--------|----------------------------------------------------------|
| `reg`  | Address of the registry (see `HerkulexEepRegister` enum) |
| return | Value of the registry                                    |


## `HerkulexServo::readEep2()`
```C++
uint16_t HerkulexServo::readEep2(HerkulexEepRegister reg);
```
Reads a two-byte from an EEPROM (volatile memory) registry. This method
takes endianness into account.

| Param  | Description                                              |
|--------|----------------------------------------------------------|
| `reg`  | Address of the registry (see `HerkulexEepRegister` enum) |
| return | Value of the registry                                    |


## `HerkulexServo::reboot()`
```C++
void HerkulexServo::reboot();
```
Ask the servo to reboot.


## `HerkulexServo::rollbackToFactoryDefaults()`
```C++
void HerkulexServo::rollbackToFactoryDefaults(bool skipID, bool skipBaud);
```
Ask the servo to roll back all registries to their default values.
Optionally, you can choose to keep the current value of the ID and Baud rate
registries.

| Param      | Description                                 |
|------------|---------------------------------------------|
| `skipID`   | If true, the current ID will be kept        |
| `skipBaud` | If true, the current baud rate will be kept |


## `HerkulexServo::getID()`
```C++
uint8_t HerkulexServo::getID();
```
Returns the ID associated with the servo.


## `HerkulexServo::getStatus()`
```C++
void HerkulexServo::getStatus(HerkulexStatusError &status_error, HerkulexStatusDetail &status_detail);
```
Requests the two status registries and puts them into the variables referenced.
See the `HerkulexStatusError` and `HerkulexStatusDetail` enums for more detail.

| Param           | Description                                                              |
|-----------------|--------------------------------------------------------------------------|
| `status_error`  | Reference to `HerkulexStatusError` variable to store error information   |
| `status_detail` | Reference to `HerkulexStatusDetail` variable to store detail information |


## `HerkulexServo::getPosition()`
```C++
uint16_t HerkulexServo::getPosition();
```
Returns the current calibrated position of the servo. This position takes the
`HerkulexRamRegister::CalibrationDifference` into account.


## `HerkulexServo::getRawPosition()`
```C++
uint16_t HerkulexServo::getRawPosition();
```
Returns the current raw, uncalibrated position of the servo.


## `HerkulexServo::setLedColor()`
```C++
void HerkulexServo::setLedColor(HerkulexLed color);
```
Sets the color of the built-in LED. Refer to the `HerkulexLed` enum.

| Param   | Description                                        |
|---------|----------------------------------------------------|
| `color` | `HerkulexLed` enum value, e.g. `HerkulexLed::Cyan` |


## `HerkulexServo::setTorqueOn()`
```C++
void HerkulexServo::setTorqueOn();
```
Turn the motor torque on. Torque needs to be turned on in order for a servo to
move to its goal position.


## `HerkulexServo::setTorqueOff()`
```C++
void HerkulexServo::setTorqueOff();
```
Turn the motor torque off. In this mode, the servo horn can be rotated freely.


## `HerkulexServo::setBrake()`
```C++
void HerkulexServo::setBrake();
```
Puts the motor in brake mode, meaning that it will resist movement but it will
not move on its own.


## `HerkulexServo::enablePositionControlMode()`
```C++
void HerkulexServo::enablePositionControlMode();
```
Puts the servo in position control mode. In this mode, the servo will attempt to
reach the specified target position. This is the default control mode.

**Important:** Torque needs to be turned off (`setTorqueOff()`) in order to
change the control mode.


## `HerkulexServo::enableSpeedControlMode()`
```C++
void HerkulexServo::enableSpeedControlMode();
```
Puts the servo in speed control (i.e. continuous rotation) mode. In this mode,
the servo will attempt to reach the specified target speed, but it will no
longer be possible to set its angle.

**Important:** Torque needs to be turned off (`setTorqueOff()`) in order to
change the control mode.


## `HerkulexServo::setPosition()`
```C++
void HerkulexServo::setPosition(uint16_t pos, uint8_t playtime = 0, HerkulexLed led = HerkulexLed::Ignore);
```
Request the servo to move to the specified position over a specified time
duration. Only works in position control mode. If
`HerkulexBus::prepareIndividualMove()` or
`HerkulexBus::prepareSynchronizedMove()` is called on the bus, the position will
be buffered until `executeMove()` is called. Otherwise, the command will be
issued immediately. Optionally, the LED color can also be specified.

**Note:** The `pos` and `playtime` parameters use units that are native to the
Herkulex servos, however they make little physical sense. Use the formulas shown
below to calculate these values:

```C++
uint16_t pos = 512 + uint16_t(pos_degree / 0.325f);
uint8_t playtime = uint8_t(time_ms / 11.2f);
```

**Note:** In `HerkulexBus::prepareSynchronizedMove()` mode, the playtime
parameter will be ignored. The playtime value specified in the
`prepareSynchronizedMove()` call takes precedence.

| Param      | Description                                                                                                    |
|------------|----------------------------------------------------------------------------------------------------------------|
| `pos`      | Value between `0` and `1023` indicating the goal position. A value of `512` indicates neutral.                 |
| `playtime` | *(optional)* Value between `0` and `255` indicating the move duration. Each tick represents 11.2 milliseconds. |
| `led`      | *(optional)* Color of the servo's LED, will remain the same if unspecified.                                    |


## `HerkulexServo::setSpeed()`
```C++
void HerkulexServo::setSpeed(int16_t speed, uint8_t playtime = 0, HerkulexLed led = HerkulexLed::Ignore);
```
Analogous to the `setPosition()` method, but for use with servos in speed
control mode.

| Param      | Description                                                                                                    |
|------------|----------------------------------------------------------------------------------------------------------------|
| `speed`    | Value between `-1023` and `1023` indicating the goal speed. Negative numbers indicate reversed direction.      |
| `playtime` | *(optional)* Value between `0` and `255` indicating the move duration. Each tick represents 11.2 milliseconds. |
| `led`      | *(optional)* Color of the servo's LED, will remain the same if unspecified.                                    |
