# LED Capacitive Lamp #

12v RGBW LED Strip controlled via Arduino and Capacitive Touch Sensor Library

### How do I get set up? ###

* Arduino Uno or Equivalent
* [Arduino Software](http://arduino.cc/en/Main/Software)
* [CapacitiveSensor Library](https://github.com/PaulStoffregen/CapacitiveSensor)
* 1 x 12v RGBW LED Strip
* 1 x 12v DC Wall Adaptor
* 4 x NPN TIP120 Transistor
* 1 x 1K Resistor

### How it works ###

* Once the hardware is all built and the Arduino is loaded with project, the lamp will start to cycle colors, starting with Blue then slowly transitioning into Green then Red. 
* A single tap to the Capacitive Sensor will make the White LEDs brighter, on a scale of 5 levels of Brightnesses.
* A Double Tap the Cap Sensor will make the color cycle stop rotating and will stop on the current color. The lights will briefly blink showing that you successfully stopped the cycling. Double Tap again to continue the Color Cycling.
* Tap and Hold for 1 second will cause the Lamp to shut all lights ON/OFF.


More info about the Cap Sensor library can also be found [Here](http://playground.arduino.cc/Main/CapacitiveSensor?from=Main.CapSense)

![LED Lamp Cap_bb.png](https://raw.githubusercontent.com/dansl/LED-Capacitive-Lamp/master/LED%20Lamp%20Cap_bb.png)
