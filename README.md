Cederberg Senior Scout Adventure 2014
=====================================
Electronics Base Thermometer
----------------------------

The project for [eBase 2014](http://www.sasebase.net) is an indoor/outdoor thermometer.  It is run of an ATmega88PA, with three 7seg LED displays, two pushbuttons, and a pair of indoor and outdoor thermistors, driven by 2 series AA battries.

Operation
---------
The human interaction is through three seven segment displays and two push buttons.  The value of `DISP_TIME` is currently undecided, but will probably be about 3 seconds.

Pressing a pushbutton and releasing will result in the relevant temperature for `DISP_TIME` seconds.  If the button is held for `DISP_TIME` seconds, the minimum and then maximum for that thermometer will be displayed.

Desgin Considerations
---------------------

The device is driven by 2 AA batteries.  As such, battery life is paramount.  The following steps will improve battery life.

 * The thermistors will only be driven whilst a reading is taken.  Due to parallel capacitors C4 and C5, a short time must elapse between the driving of the thermistors and the reading of the voltages.

 * The displays will be strobed.  That is, only one display is ever lit at a time.  However, they flash fast enough (83.3Hz) such that all three appear on at all times.

 * When the displays are not on and no reading is being taken, the microcontroller will be in "sleep mode".  This is an extremely low current draw mode.  Measurements were not accurate enough to determine actual current draw in sleep mode (noise was overriding).

In order to have accurate minimum and maximum temperatures, the temperature must be taken every minute or so.  The watchdog timer is used to wake the microcontroller from sleep and take a reading every 64 seconds.