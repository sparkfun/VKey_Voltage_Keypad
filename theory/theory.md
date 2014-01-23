# VKey
##Theory Of Operation

### Background
The product specification for the VKey was short and to the point:

>The idea for this board is to create an array of buttons, say, 10 or 12, that, when pressed, create a unique voltage at the single output header. This is accomplished by designing each button to be a unique voltage divider. This will allow the user to read several buttons using only a single analog pin.

Sound easy, right?  Some voltage dividers feeding the output - a dozen resistors, a dozen buttons, and done.

### 
Well, not so fast actually.  The basic idea is sound, but it's got a significant hangup.  Let's mock it up, and explore where it falls apart.

For the sake of keeping the math simple, let's use a divider with 5 steps, powered from a 5 Volt power supply.  Each step in the divider will be 1 Volt higher the the previous step.

#pic1

That yields easily discernible voltage steps.  Let's add a set of switches that allow us to connect each of those Voltage levels to an output.

#pic2

When a switch is closed, the voltage indeed shows up at the output.

#pic3

Occams Razor reminds us that things should be as simple as possible, and no simpler.  This would appear to fail that test: it's so simple that the results are rather unusable.