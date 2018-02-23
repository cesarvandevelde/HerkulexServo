# Installation
The HerkulexServo library imposes no special hardware requirements and should
work on any board supported by the Arduino framework. This library has one
external dependency, the [CircularBuffer][circularbuffer] library by Roberto Lo
Giacco. Make sure this library is installed as well.

## Through the Arduino IDE
TODO

## Through PlatformIO
{% method %}
Simply add `HerkulexServo` as a dependency to your `platformio.ini` file. The
library and its dependencies will be installed automatically

{% sample lang="C++" %}
Example config for `platformio.ini`:

```ini
[env:nanoatmega328]
platform = atmelavr
board = nanoatmega328
framework = arduino
lib_deps =
  HerkulexServo
```

{% endmethod %}

[circularbuffer]: https://github.com/rlogiacco/CircularBuffer
