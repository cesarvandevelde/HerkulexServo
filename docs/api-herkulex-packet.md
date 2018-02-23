# HerkulexPacket struct
This struct is used to represent incoming data packets from the Herkulex bus.
The `HerkulexBus` class does some basic processing on incoming data packets and
uses this information to fill in the `error` field. This field is not part of
the actual serial data transfer. Likewise, The packet header `0xFF 0xFF` is
also stripped automatically. The struct has the following declaration:

```C++
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
```
