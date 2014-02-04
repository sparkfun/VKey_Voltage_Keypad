# VKey
##Theory Of Operation

### Background
The initial product specification for the VKey was short and to the point:

>The idea for this board is to create an array of buttons, say, 10 or 12, that, when pressed, create a unique voltage at the single output header. This is accomplished by designing each button to be a unique voltage divider. This will allow the user to read several buttons using only a single analog pin.

Sound easy, right?  Some voltage dividers feeding the output - a dozen resistors, a dozen buttons, and done.

### 
Well, not so fast actually.  The basic idea is sound, but it's got a significant hangup.  Let's mock it up, and explore where it falls apart.

For the sake of keeping the math simple, let's use a divider with 4 resistors, powered from a 5 Volt power supply.  Each node in the divider will be 1.25 Volts higher the the previous node.  At this point, the exact value of the resistor doesn't matter, as long as they're all about the same.  We'll use 10K Ohms as a starting value.

#pic1

That yields easily discernible voltage steps.  

Let's add a set of switches that allow us to connect each of those Voltage levels to an output.

#pic2

When a switch is closed, the voltage indeed shows up at the output.

#pic3

Occams Razor reminds us that things should be as simple as possible, and no simpler.  This would appear to fail that test: it's so simple that the results are rather unusable.  It has a couple of notable problems, both related to closing multiple switches at the same time.

The first problem is that it can cause aliasing.  If we press the second and fourth switches at the same time, we get the same result as if we pressed the third switch, and we don't have a reasonable way to tell the difference.  This isn't solely a problem with the resistive keypad concept.  A plain scanned-matrix keypad can also alias when multiple keys are pressed.

#pic4

The second problem is more severe.  If we close keys 1 and 5 at the same time, we create a short from the power supply to ground -- a situation generally best avoided.

So we could add another resistor at the top or bottom of the divider, and avoid the supply-shorting problem altogether.  For argument's sake, let's put it at the bottom.

#pic5

Now we have 5V divided into 5 steps - 1 V per step.  

Let's look at the current consumed by the circuit.  When no buttons are pressed, the overall resistance of the string is 50K.  Likewise, when only a single button is pressed, no parallel paths are formed, so the overall resistance is still 50K.  If we were to mash down all of the buttons, we create a short path around all but the bottom resistor, and the resistance falls to a worst-case of 10K.   

The current drawn by the circuit will also vary with the number of buttons pressed.  With zero or one button held, 5V across the 50K resistance will draw .1 mA.  In the all buttons case, 5V through 10K Ohms results in a maximum current consumption of .5 mA.

Let's reconsider how the circuit is connected to the power supply: the voltage is held constant, while the current through the resistors varies as different key combinations are pressed.  But what if we could change that?  While constant voltage supplies are common, it's also possible to make a constant current supply.  In fact, it turns out to be very useful in this case.

The constant current source is a circuit block commonly used inside analog ICs.  It's frequently represented using symbols such as a pair of circles, a circle and a line, or even just an arrow with a current value notation nearby. 

#pic of symbols - 741 or sim datasheet?

Page xxx of the 2nd edition of the Art of Electronics shows a simple commmon circuit used for current sources, consisting mainly of an opamp and a transistor.  The opamp is used to bias the base of the transistor with a specific voltage, causing it to conduct a specific current.

...equation...

Let's start with a constant current source that can supply 1mA, then reconsider the our resistance, and how the circuit behavior changes.  In order to deliver the prescribed current into the resistance, the current source will vary the voltage at it's output.  In order to put a constant current into the resistors, the source acts as a variable voltage supply.  To put 1 mA into 10K, it will rise to 10V.  To put 1 mA into 50k, it rises to 50V.

These voltages are problematic in the modern world of 3.3V power supplies, where we don't have 5V to work with, much less 25V.  So  instead of raising the available supply voltage, let's decrease the resistor values.  1 mA into 100 Ohms only requires .1V, and into 500 Ohms requires .5V.


So we can rebuild the circuit with the resistor string and the switches, but powering the string from the current source.  As long as only one button is pressed at a time, the resistor string is biased with the prescribed current.  If more than one switch is closed, the equivalent resistance changes, but the current source changes to compensate.  Effectively, this circuit is measuring the resistance between the lowest closed switch and ground.  

This circuit is reasonably flexible.  By adjusting the current from the current source and the value of the resistors in the ladder, it can be configured for different power supply voltages, and different numbers of switches.  The specific values for the VKey were calculated using the `param-tuning.ods` spreadsheet.  You can input various circuit characteristics, and use the sheet to configure the current source, then determine a step resistor value that yields discernible steps, and doesn't exceed the circuit capabilities.  Instructions for using the spreadsheet can be found in the sheet itself.

In the end, the VKey wound up using a ~.5 mA source, and 330 Ohm resistors, resulting in about 200 mV per step. 

A second spreadsheet,`ADC.ods`, is useful for interpreting the output from the circuit with an ADC.  With equal value resistors in the ladder, the switch at each step will yield an incrementally higher voltage.  The VKey Arduino library uses a calibration table to decipher the input voltage.  Those tables were generated for 3.3V and 5V reference voltages, and 10-bit ADC converters.  This spreadsheet will be useful in adapting the VKey to other reference voltages or data widths - take a series of ADC measurements while pressing each key, and enter the resulting readings into the sheet.  The sheet will calculate the min, max and step-size parameters as needed by the VKey library.