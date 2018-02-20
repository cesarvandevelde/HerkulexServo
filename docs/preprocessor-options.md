# Preprocessor options

This library has a bunch of preprocessor options that can be changed to tweak the
speed, performance and memory usage of the library. The default values should be
good enough in most cases, but can be overwritten if necessary. PlatformIO users
can add the following snippet to the `platformio.ini` file to override the
default options:

```ini
build_flags =
  -D HERKULEX_OPTION_1=123
  -D HERKULEX_OPTION_2=456
```

Unfortunately, Arduino users will have to manually edit the values in the
`HerkulexServo.h` file. I'm open to suggestions if anyone has a better solution
for this issue.


## List of options

### HERKULEX_PACKET_RX_TIMEOUT
Value in microseconds. If no serial data is received during this time, the
packet is assumed to be transferred completely and serial buffer processing is
started.


### HERKULEX_PACKET_RETRIES
The number of times the library will resend a command in order to receive a
valid packet. Software serial is fairly unreliable at 115200 baud, so this value
is quite high. In my testing, about 95% of the command responses are
successfully read on the first try. The other 5% take up to 6 retries.


### HERKULEX_PACKET_RESEND_DELAY
Value in microseconds. The delay between each packet retry.


### HERKULEX_PACKET_RX_MAX_DATA
The maximum number of data bytes in incoming packets.


### HERKULEX_SERIAL_RX_BUFFER
The size of the serial receive data ring buffer, in bytes.


### HERKULEX_MAX_SCHEDULED_SERVOS
The maximum number of moves that can be scheduled in a single data packet.
Affects `prepareIndividualMove()`, `prepareSynchronizedMove()` and
`executeMove()` functionality. Increasing this number will let you schedule more
moves in a single command at the cost of increased memory usage.


### HERKULEX_SERIAL_TX_BUFFER
The size, in bytes, of the serial transmit buffer. By default, this value is
calculated based on `HERKULEX_MAX_SCHEDULED_SERVOS`.


### HERKULEX_DEBUG
If enabled, this preprocessor option adds methods to the `HerkulexBus` class to
track data transfer statistics. The snippet below shows basic usage:

```C++
HerkulexPacket resp;
uint8_t id = 10;
int attempts = 0;

Serial.println("Starting test...");
herkulex_bus.resetRxStatistics();
delay(10);

for (attempts = 0; attempts < 1000; attempts++) {
  herkulex_bus.sendPacketAndReadResponse(resp, id, HerkulexCommand::Stat);

  // send packets to IDs 10, 11, and 12
  id++;
  if (id > 12) {
    id = 10;
  }

  delay(5);
}

herkulex_bus.printRxStatistics(Serial);
```
