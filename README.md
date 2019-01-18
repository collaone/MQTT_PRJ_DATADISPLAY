# MQTT_PRJ_DATADISPLAY

Console application developed to interact with the [probe](https://github.com/collaone/MQTT_PRJ_PROBE).
Use the class [QConsoleListener](https://github.com/juangburgos/QConsoleListener) for asynchronous input.

### Note: 
It works on linus system, with Qt5.12. Mqtt library must be installed.

## Build
```c++
qmake Probe.pro
make
```

## Usage
Start the probe application.
Start the application DataDisplay.

The application wait for a command in input:
```c++
T to start displaying CPU temperature
U to start displaying CPU usage
F to start displaying free space of the root partition
S to stop displaying
a number from 500 to 5000 to set the probing interval
```
and will subscribe the chosen topic, starting to visualize that property.
If a number is typed, the application will publish a MQTT message, received by the the probe (that has a subscription to that topic) which will change the probing interval to the number of milliseconds in input

