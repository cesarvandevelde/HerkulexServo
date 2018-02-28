<div style="width: 60%; margin-left: 20%;">
  <div style='position: relative; padding-bottom: 56.25%;'>
    <iframe src='https://gfycat.com/ifr/DismalLoathsomeAmericancurl'
            frameborder='0'
            scrolling='no'
            width='100%'
            height='100%'
            style='position: absolute; top: 0; left: 0'
            allowfullscreen>
    </iframe>
  </div>
</div>

# Introduction to the HerkulexServo library
This library allows you to control [Herkulex][herkulex] smart servos directly
from an Arduino. These servos are more expensive than the well-known RC-hobby
servos and they are also a bit more complex to use, but they offer a number of
important advantages:

- They can be controlled over a UART bus protocol. In theory, over 200 servos
  can be controlled using only two pins.
- Communication is bi-directional. You can read back information such as the
  servo's current position.
- They can be used in position control mode as well as speed control
  (continuous rotation) mode. No need to modify the internals... :)
- Nearly all parameters of the position control algorithm (e.g. PID parameters)
  can be changed.
- They have a number of protection features, such as over-voltage and
  over-temperature protection.
- They come with a bunch of different hardware accessories for mounting.

In short, these features make Herkulex servos ideal for small-scale robotics and
other mechatronics projects. Unfortunately, the software-side of the Herkulex
ecosystem is not as rosy. I only found one [existing library][other-lib]
for Arduino, which was last updated 2 years ago. In my testing, I found this
library to be unreliable and too limited for my application. So I went down the
rabbit hole and wrote my own Arduino library. This library offers a number of
benefits:

- Supports Herkulex DRS-0101 and DRS-0201 smart servos.
- Modern, clean API exposes a broad feature set.
- Robust communication layer that checks and resends garbled data packets.
- Works reliably on an Arduino Uno with `SoftwareSerial` at 115200 Baud.

## Important notice
I have only tested this library on my hardware setup, which consists of an
ATMega328 microcontroller communicating with three DRS-0101 servos at 115200
Baud over a SoftwareSerial instance. If your hardware is significantly different
from this, please let me know whether or not this library works for you. You can
do so by opening an issue on [GitHub][github].

[herkulex]: http://www.dongburobot.com/jsp/cms/view.jsp?code=100788
[other-lib]: https://github.com/rambo/Arduino-Servo-Herkulex
[github]: https://github.com/cesarvandevelde/HerkulexServo/issues
