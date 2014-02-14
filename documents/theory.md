# VKey
##Theory Of Operation

### Background
The initial product specification for the VKey was short and to the point:

>The idea for this board is to create an array of buttons, say, 10 or 12, that, when pressed, create a unique voltage at the single output header. This is accomplished by designing each button to be a unique voltage divider. This will allow the user to read several buttons using only a single analog pin.

Sound easy, right?  Some voltage dividers feeding the output - a dozen resistors, a dozen buttons, and done.

Well, not so fast actually.  The basic idea is sound, but it's got a significant hangup.  Let's mock it up, and explore where it falls apart.

### The Simple Version

For the sake of keeping the math simple, let's start with a divider with 4 resistors, powered from a 5 Volt power supply.  Each node in the divider will be 1.25 Volts higher the the previous node.  At this point, the exact value of the resistor doesn't matter, as long as they're all about the same.  We'll use 10K Ohms as a starting value.

![Simple Divider](/pictures/pic1.png)

That yields easily discernible voltage steps, and draws negligible current.  

Let's add a set of switches that allow us to connect each of those Voltage levels to an output.

![Divider With Switches](/pictures/pic2.png)

When a switch is closed, the voltage indeed shows up at the output.

![Press A Switch](/pictures/pic3.png)

### Some Problems

Occams Razor reminds us that things should be as simple as possible, and no simpler.  This would appear to fail that test: it's so simple that the results are unusable.  It has a couple of notable problems, both related to closing multiple switches at the same time.

The first problem is that it can cause aliasing.  If we press the second and fourth switches at the same time, we get the same result as if we pressed the third switch, and we don't have a reasonable way to tell the difference.  This isn't solely a problem with the resistive keypad concept.  A plain scanned-matrix keypad can also alias when multiple keys are pressed.

![Aliasing](/pictures/pic4.png)

The second problem is more severe.  If we close keys 1 and 5 at the same time, we create a short from the power supply to ground -- a situation generally best avoided.

![Short Circuit!](/pictures/pic5.png)

### A Slight Improvement

We could add another resistor at the top or bottom of the divider, and avoid the supply-shorting problem altogether.  For argument's sake, let's put it at the bottom.

![Better Divider](/pictures/pic6.png)

Now we have 5V divided into 5 steps - 1 V per step.  We can limit the max current when too many switches are pressed all at once.  But it still demonstrates aliasing, and with a little cleverness, we can avoid that too.

Let's take a moment to consider the load the circuit places on the power supply.  When no buttons are pressed, the overall resistance of the string is 50K.  Likewise, when only a single button is pressed, no parallel paths are formed, so the overall resistance is still 50K.  If we were to mash down all of the buttons, we create a short path around all but the bottom resistor, and the resistance falls to a worst-case of 10K.   

The current drawn by the circuit will also vary with the number of buttons pressed.  With zero or one button held, 5V across the 50K resistance will draw .1 mA.  In the all buttons case, 5V through 10K Ohms results in a maximum current consumption of .5 mA.

# A Bigger Improvement

Let's reconsider how the circuit is connected to the power supply: the voltage is held constant, while the current through the resistors varies as different key combinations are pressed.  But what if we could change that?  While constant voltage supplies are common, it's also possible to make a constant current supply.  In fact, it turns out to be very useful in this case.

The constant current source is a circuit block commonly used inside analog ICs.  It's frequently represented using symbols such as a circle and a line, an overlapping pair of circles, or even just an arrow with a current value notation nearby. 

![Current Sources](/pictures/pic7.png) 

Since it's not a component that you can just pull of the shelf, you don't always see current sources.  Sometimes they turn up as a simplification when discussing circuit design, and they frequently pop up on datasheets, when the internal details of an analog IC are described.

![Opamp Internals](/pictures/pic8.png)
-> *Internal diagram of the Linear Tech LT1112 Operational Amplifier* <-

Page 181 of the 2nd edition of the Art of Electronics shows a commmon circuit used for current sources, consisting of an opamp and a transistor.  The opamp is used to bias the base of the transistor with a specific voltage, drawn across a fixed resistance, causing it to conduct a specific current.  

	Iout ~= (Vcc - (V at opamp inverting input))/fixed resistance

With 1 mA, lets reevaluate our resistor string, and how the circuit behavior changes.  In order to put a constant current into a variable resistance, the source acts as a variable voltage supply.  To put 1 mA into 10K, it will rise to 10V.  To put 1 mA into 50k, it rises to 50V.

These voltages are problematic in the modern world of 3.3V power supplies, where we don't have 5V to work with, much less 10V or 50V.  So  instead of raising the supply voltage, let's decrease the resistor values.  1 mA into 100 Ohms only requires .1V, and into 500 Ohms requires .5V.

So we can rebuild the circuit with the resistor string and the switches, but powering the string from the current source.  As long as only one button is pressed at a time, the whole resistor string is biased with the prescribed current.  If more than one switch is closed, a shorter path is formed and the equivalent resistance changes, but the current source compensates by lowering the voltage.  Effectively, the voltage from the current source indicates the resistance between the lowest closed switch and ground.

## The Pieces Come Together.

Let's examine the VKey schematic.

![VKey Schematic](/pictures/schem.png)

For the VKey, we've modified the H&H current source a little, using a silicon diode to form a constant voltage below Vcc.  Silicon diodes have a forward drop voltage of around .6 V.  If we use a 600 Ohm resistor, the source provides 1 mA.  This keeps the current more constant across different power supply voltages, though it  does vary a tiny bit between 3.3V and 5V, as the diode conducts a bit more when biased from 5V.

In practical realization, the VKey current source wound up with a 1K Ohm resistor, rather than 600 (a rather uncommon value), resulting in roughly 0.6mA.  The resistors in the string are 330 Ohms each.  0.6mA through 330 Ohms results in a voltage drop of about 200 mV. 

Using a dual opamp, the second section is used as a voltage buffer, so the circuit can consistently drive external loads, regardless of key combinations (such as the capacitor in an A-to-D converter).

### Digging Deeper

This circuit is reasonably flexible.  By adjusting the current from the source and the value of the resistors in the ladder, it can be configured for different power supply voltages, and different numbers of switches.  The specific values for the VKey were calculated using the `param-tuning.ods` spreadsheet.  You can input various circuit characteristics, and use the sheet to configure the current source, then determine a step resistor value that yields discernible steps, and doesn't exceed the circuit capabilities.  Instructions for using the spreadsheet can be found in the sheet itself.

A second spreadsheet,`ADC.ods`, is useful for interpreting the output from the circuit with an ADC.  With equal value resistors in the ladder, the switch at each step will yield an incrementally higher voltage.  The VKey Arduino library uses a calibration table to decipher the input voltage.  Those tables were generated for 3.3V and 5V reference voltages, and 10-bit ADC converters.  This spreadsheet will be useful in adapting the VKey to other reference voltages or data widths - take a series of ADC measurements while pressing each key, and enter the resulting readings into the sheet.  The sheet will calculate the min, max and step-size parameters as needed by the VKey library.

### References

1. Paul Horowitz and Winfield Hill, __The Art of Electronics__, second edition, 1989.