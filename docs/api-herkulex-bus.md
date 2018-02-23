# HerkulexBus class
The `HerkulexBus` class manages communication with Herkulex servos over a UART
bus. In addition, it provides methods that affect all servos on the bus, such as
performing `IJog` and `SJog` commands.

## `HerkulexBus::HerkulexBus()`
```C++
HerkulexBus::HerkulexServoBus(Stream &serial_connection);
```
The constructor takes exactly one argument: a `Stream`-like instance that
represents the UART port to communicate over. For example, this can be a
hardware serial port (e.g. `Serial`, `Serial1`, `Serial2`, `Serial3`) or an
instance of the `SoftwareSerial` class.

| Param               | Description                                    |
|---------------------|------------------------------------------------|
| `serial_connection` | Reference to a serial port to communicate over |


## `HerkulexBus::sendPacket()`
```C++
void HerkulexBus::sendPacket(uint8_t id, HerkulexCommand cmd, const uint8_t* data = nullptr, uint8_t data_len = 0);
```
Sends a single data packet to the bus.

| Param      | Description                                                       |
|------------|-------------------------------------------------------------------|
| `id`       | The ID of the target servo                                        |
| `cmd`      | Which command to send, e.g. `HerkulexCommand::Stat`               |
| `data`     | *(optional)* Pointer to a byte array that holds extra packet data |
| `data_len` | *(optional)* Number of extra data bytes to transfer               |


## `HerkulexBus::sendPacketAndReadResponse()`
```C++
bool HerkulexBus::sendPacketAndReadResponse(HerkulexPacket &resp, uint8_t id, HerkulexCommand cmd, const uint8_t* data = nullptr, uint8_t data_len = 0);
```
Sends a data packet and attempts to read a response. The method will continue to
send out packets until it is successful or until it reaches
`HERKULEX_PACKET_RETRIES` attempts.

| Param      | Description                                                             |
|------------|-------------------------------------------------------------------------|
| `resp`     | Reference to a `HerkulexPacket` variable where the response will be put |
| `id`       | The ID of the target servo                                              |
| `cmd`      | Which command to send, e.g. `HerkulexCommand::Stat`                     |
| `data`     | *(optional)* Pointer to a byte array that holds extra packet data       |
| `data_len` | *(optional)* Number of extra data bytes to transfer                     |
| return     | True if successful, false if the method times out                       |


## `HerkulexBus::update()`
```C++
void HerkulexBus::update();
```
This method should be called as frequently as possible in your main loop. The
method checks for incoming serial data and attempts to decode the packets.


## `HerkulexBus::getPacket()`
```C++
bool HerkulexBus::getPacket(HerkulexPacket &packet);
```
This method attempts to get a `HerkulexPacket` from the bus and put it in the
`packet` variable. Returns true if successful. If no packets are available, the
`packet` variable remains unchanged and the method returns false.

| Param    | Description                                                                    |
|----------|--------------------------------------------------------------------------------|
| `packet` | Reference to a `HerkulexPacket` variable where the incoming packet will be put |
| return   | True if successful, false if no packets are available                          |


## `HerkulexBus::resetRxStatistics()`
```C++
void HerkulexBus::resetRxStatistics();
```
Resets the internal variables that keep track of data transfer statistics, i.e.
number of successful packets, failed packets, retry attempts.

**Note:** This method is only available when the `HERKULEX_DEBUG` preprocessor
flag is enabled.


## `HerkulexBus::printRxStatistics()`
```C++
void HerkulexBus::printRxStatistics(Stream &output);
```
Prints out a packet transfer statistics report, i.e.
number of successful packets, failed packets, retry attempts.

**Note:** This method is only available when the `HERKULEX_DEBUG` preprocessor
flag is enabled.

| Param  | Description                                                    |
|--------|----------------------------------------------------------------|
| output | Stream to print the statistics debug message to, e.g. `Serial` |


## `HerkulexBus::prepareIndividualMove()`
```C++
void HerkulexBus::prepareIndividualMove();
```
Prepares the bus for sending an `IJog` data packet. This is a single packet that
contains the goal positions and playtimes of multiple servos. All subsequent
calls to `HerkulexServo::setPosition()` and `HerkulexServo::setSpeed` will be
stored in buffer until `executeMove()` is called. There is a limit to the number
of moves that can be scheduled, defined by the value of
`HERKULEX_MAX_SCHEDULED_SERVOS`.


## `HerkulexBus::prepareSynchronizedMove()`
```C++
void HerkulexBus::prepareSynchronizedMove(uint8_t playtime);
```
Prepares the bus for sending an `HerkulexCommand::SJog` data packet. This packet
is similar to the `HerkulexCommand::IJog` packet, but only a one playtime value
is specified for all servos affected by the command. All subsequent calls to
`HerkulexServo::setPosition()` and `HerkulexServo::setSpeed` will be stored in
buffer until `executeMove()` is called. There is a limit to the number of moves
that can be scheduled, defined by the value of `HERKULEX_MAX_SCHEDULED_SERVOS`.

| Param    | Description                                                                         |
|----------|-------------------------------------------------------------------------------------|
| playtime | Duration of the move, multiply by `11.2` to calculate the duration in milliseconds. |


## `HerkulexBus::executeMove()`
```C++
void HerkulexBus::executeMove();
```
Sends out the moves scheduled by `prepareIndividualMove()` or
`prepareSynchronizedMove()`. The class keeps track of which method was called,
and will either send out a `HerkulexCommand::IJog` or a `HerkulexCommand::SJog`
data packet.
