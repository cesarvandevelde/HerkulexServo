# Sweep

This example sweeps a servo back and forth from -90° to +90°. Herkulex servos
use a number between `0` and `1023` to indicate position. To go from degrees to
Herkulex units, use the following formula:

```C++
uint16_t pos = 512 + uint16_t(pos_degree / 0.325f);
```

**Note:** Be sure to change `SERVO_ID` to the ID of the servo you want to sweep.

[include, lang:"C++"](../examples/Sweep/Sweep.ino)
