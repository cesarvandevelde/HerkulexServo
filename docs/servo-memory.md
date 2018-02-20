# Servo memory
Herkulex smart servos have a large amount of control registers. These registers
are used for storing things like control algorithm parameters (e.g. PID values),
configurable options (e.g. servo ID), and sensor readings (e.g. temperature).

To make things even more complicated, there are two types of memory:
non-volatile EEPROM memory and volatile RAM memory. Data stored in RAM is lost
every time the servo is powered off, data stored in EEPROM is not. On startup,
the servo will read data from the EEPROM and store it in RAM. The RAM registers
determine the actual behavior of the servo. Refer to the [manual (PDF)][manual]
for a detailed explanation of what each register does. The HerkulexServo library
does not encapsulate all functionality of the servos. For advanced functions,
you will have to manipulate registries manually.

[manual]: ./assets/manual-drs-0101.pdf

## Reading from registers
{% method %}
Use the `readRam()` and `readRam2()` methods to read data from RAM. These
methods return one and two bytes respectively. Most data is stored in a single
byte of memory, though some registries take up more space. Use the `readEep()`
and `readEep2()` methods to read data from the EEPROM. These methods work in the
same way.

**Important:** Notice the difference between the enum classes
`HerkulexRamRegister` and `HerkulexEepRegister`? This is necessary because the
RAM and EEPROM use different addresses for the same register.

{% sample lang="C++" %}
Reading from RAM:
```C++
uint8_t  one_byte = my_servo.readRam(HerkulexRamRegister::Temperature);
uint16_t two_byte = my_servo.readRam2(HerkulexRamRegister::CalibratedPosition);
```

Reading from EEPROM:
```C++
uint8_t  one_byte = my_servo.readEep(HerkulexEepRegister::ID);
uint16_t two_byte = my_servo.readEep(HerkulexEepRegister::PositionKp);
```
{% endmethod %}


## Writing to registers
{% method %}
Likewise, the `writeRam()`, `writeRam2()`, `writeEep()`, and `writeEep2` methods
can be used to write data to a registry.

**Tip:** Some registers are read-only and cannot be written to. Please refer to
the manual to find out more.

**Important:** Only the RAM registers have an immediate effect on the behavior
of the servo. EEPROM settings only take effect once the servo is rebooted.

{% sample lang="C++" %}
Writing to RAM:
```C++
my_servo.writeRam(HerkulexRamRegister::ID, 10);
my_servo.writeRam2(HerkulexRamRegister::PositionKp, 800);
```

Writing to EEPROM:
```C++
my_servo.writeEep(HerkulexEepRegister::ID, 10);
my_servo.writeEep2(HerkulexEepRegister::MaxPwm, 400);
```
{% endmethod %}

## Reboot
{% method %}
At startup, the servo copies data from the EEPROM into RAM. As mentioned before,
only RAM registers affect the behavior of the servo. You can choose to restart
the servo using the `reboot()` method.


{% sample lang="C++" %}
Request a reboot:
```C++
my_servo.reboot();
```
{% endmethod %}

## Factory defaults
{% method %}
To restore the factory defaults, use the `rollbackToFactoryDefaults()` method.
You can choose to skip the ID register and/or the Baud rate register, saving you
the hassle of removing the servo from the bus to reprogram its ID.

{% sample lang="C++" %}
Reset all registers except ID and baud rate:
```C++
my_servo.rollbackToFactoryDefaults(true, true);
```
{% endmethod %}
