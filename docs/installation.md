# Installation
The HerkulexServo library imposes no special hardware requirements and should
work on any board supported by the Arduino framework. This library has one
external dependency, the [CircularBuffer][circularbuffer] library by Roberto Lo
Giacco. Make sure this library is installed as well.

## Through the Arduino IDE
{% method %}
In the Arduino IDE, go to `Sketch` > `Include Library` > `Manage Libraries...`.
From there, search for and install the following libraries:
- **CircularBuffer** by *AgileWare*
- **HerkulexServo** by *Cesar Vandevelde*

{% sample lang="C++" %}

<div>
  <div style='position: relative; padding-bottom: 57.75%;'>
    <iframe src='https://gfycat.com/ifr/ColorlessDirtyElectriceel'
            frameborder='0'
            scrolling='no'
            width='100%'
            height='100%'
            style='position: absolute; top: 0; left: 0'
            allowfullscreen>
    </iframe>
  </div>
</div>

{% endmethod %}

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
