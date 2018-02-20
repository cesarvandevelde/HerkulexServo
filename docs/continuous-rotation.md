# Continuous rotation mode

It is also possible to use Herkulex servos in continuous rotation mode. In this
mode, the servo can move beyond its range of roughly 320°. However, position
control is no longer possible. The motion algorithm will control the *speed*
instead.

## Setup

{% method %}
By default, the servo boots into position control mode. Speed control mode needs
to be explicitly enabled using the `enableSpeedControlMode()` method.

**Important:** The servo's control mode can only be switched when torque is
turned off. This can be done using the `setTorqueOff()` method.

{% sample lang="C++" %}
Turn torque off:
```C++
my_servo.setTorqueOff();
```
Then, switch to speed control mode:
```C++
my_servo.enableSpeedControlMode();
```

{% endmethod %}

## Controlling speed
{% method %}
The rotation speed can be set using the `setSpeed()` method. This method is
analogous to the `setPosition()` method, but only works in speed control mode.
Speed is expressed as a number ranging from `-1023` to `+1023`. The direction of
rotation is determined by the sign of the speed parameter.

**Tip:** `setSpeed()` has an optional playtime and led color parameter, just
like `setPosition()`.

**Tip:** `setSpeed()` can also be used in conjunction with `prepareIndividualMove()`
and `prepareSynchronizedMove()`.

{% sample lang="C++" %}
Set speed:
```C++
my_servo.setSpeed(600);
```

Then, turn torque back on:
```C++
my_servo.setTorqueOn();
```
{% endmethod %}

## Switching back
{% method %}
Switching back to position control mode can be done using the
`enablePositionControlMode()` method. Again, torque needs to be switched off
before changing control modes.


{% sample lang="C++" %}
Turn torque off:
```C++
my_servo.setTorqueOff();
```
Then, switch to position control mode:
```C++
my_servo.enablePositionControlMode();
```
{% endmethod %}
