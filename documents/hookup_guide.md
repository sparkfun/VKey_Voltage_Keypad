#VKey Voltage Keypad Hookup Guide

##Introduction

If you need to add a keypad to your microcontroller project, but don't want to use up a lot of I/O  pins to interface with it, the VKey might be the solution.  It outputs an analog voltage to encode which key has been pressed, which can in turn be read by an analog to digital converter on a microcontroller.

`![alt](url)Ortho pic`

-> *VKey Voltage Keypad* <-

Traditional digital keypad interfacing techniques can require a large number of digital I/O lines.  One common arrangement uses N digital outputs and M digital inputs to interface with an N*M array of keys, [as described here](http://www.openmusiclabs.com/learning/digital/input-matrix-scanning/single-mux/).  On a small controller, there may not be many I/O pins left for other tasks.

The VKey board has 12 pushbutton switches and some supporting circuitry.  It outputs an analog voltage that represents which of the keys is being pressed.  The output can be sampled using a single channel of ADC, allowing 12 keys to be scanned using a single analog input pin.

### Covered In This Tutorial

This tutorial will guide you through connecting the VKey to an Arduino, and introduces a library that tells the application which key is currently pressed.

### Required Materials

* [VKey Voltage Keypad](https://www.sparkfun.com/products/...) 
* [Arduino](https://www.sparkfun.com/products/11021), [RedBoard](https://www.sparkfun.com/products/11575) or any [Arduino-compatible](https://www.sparkfun.com/categories/242) board. 
* [Male PTH headers](https://www.sparkfun.com/products/116) or [right angle PTH headers](https://www.sparkfun.com/products/553).
* [Jumper Wires](https://www.sparkfun.com/products/9385)

### Suggested Reading

* [What is an Arduino?](tutorials/50)
* [How to Solder](tutorials/5)
* [Working with Wire](tutorials/41)
* [Analog to Digital Conversion](tutorials/35)
* [Voltage Dividers](tutorials/44)
* [Voltage, Current, Resistance, and Ohm's Law](tutorials/27)

---

##Board Overview

Looking at the front of the board, we see an 3 * 4 array of tactile switches, and three connection pads near the bottom edge of the board.

`front pic`
-> *Front of VKey* <-

**GND** should be connected to the ground of the host circuit.

**Vout** is the analog output of the keypad, and should be connected to an analog-to-digital channel (such as A0, A1, etc on an Arduino).

**V+** is the power supply, and should be connected to a voltage between 2.5V - 5.5V.

### How It Works

Take a look at the back of the VKey.

`back pic`
-> *Back of VKey* <-

The chip is a dual opamp, used as a current source and buffer amplifier.  The current source drives a string of resistors that form a voltage divider, and the tact switches select different voltages at the different taps of the divider. 

See the [resources]() section for links to a much more detailed guide to the internals of the VKey.

Each of the buttons on the VKey produces a unique analog voltage


<table border="1" align="center" width="500">
<tr align="center"><td><b>Key Number</b></td><td><b>V<sub>out</sub> (5V supply)</b></td><td><b>V<sub>out</sub> (3.3V supply)</b></td></tr>
<tr align="center"><td>None</td><td>.057</td><td>.053</td></tr>
<tr align="center"><td>12</td><td>.198</td><td>.194</td></tr>
<tr align="center"><td>11</td><td>.396</td><td>.388</td></tr>
<tr align="center"><td>10</td><td>.596</td><td>.583</td></tr>
<tr align="center"><td>9</td><td>.794</td><td>.776</td></tr>
<tr align="center"><td>8</td><td>.992</td><td>.970</td></tr>
<tr align="center"><td>7</td><td>1.190</td><td>1.164</td></tr>
<tr align="center"><td>6</td><td>1.388</td><td>1.358</td></tr>
<tr align="center"><td>5</td><td>1.585</td><td>1.551</td></tr>
<tr align="center"><td>4</td><td>1.781</td><td>1.744</td></tr>
<tr align="center"><td>3</td><td>1.979</td><td>1.938</td></tr>
<tr align="center"><td>2</td><td>2.176</td><td>2.131</td></tr>
<tr align="center"><td>1</td><td>2.372</td><td>2.323</td></tr>
</table><br>

As you can see, with a 5V supply, each successive button adds about 200 mV to the output voltage -- at 3.3V the voltage per step is slightly less.

One situation to consider is when more than one key is pressed at the same time.  The VKey implements high-key number (or low voltage) priority - when more than one of the switches is closed at a time, the output will indicate the higher key number.  For instance, if you hold down 5 and 9 together, the output will indicate key 9 is pressed.

---

##Hookup Example

The VKey comes with the surface mount components assembled, but the PTH tactile switches are loose, and need to be soldered to the board.   

`pic`
-> *Contents of the VKey package* <- 

The first step in hooking up the VKey is to solder in the key switches.

`pic`
-> *Attaching the tact switches* <-

Depending on how the board is to be mounted, different headers can be soldered to the board.

If the overall depth of the assembly is a concern, you can use a right angle header on the same side of the board as the tact switches.

`pic`
-> *Right angle header on top of board* <-

If clearance past the edge of the board is a concern, you can use a straight header soldered to the side opposite the switches.

`pic`
-> *Straight header on back of board* <-

And of course, you could always solder wires directly to the board in place of the header.

###Connecting The VKey To An Arduino

`pic`
-> *Basic hookup to an Arduino* <-

(VKey → Arduino)

* V+ → 5V
* Vout → A0
* GND → GND

---

##Arduino Library

To make interfacing the VKey as simple as possible, we've written an [Arduino library](https://github.com/sparkfun/VKey/), which we demonstrate with an example sketch. If you need a refresher on how to install an Arduino library, please see our [library tutorial](tutorials/15).  

The VKey library provides an object that interfaces with a VKey keypad.  In a typical application, this class is instantiated as a global object before the `setup()` function.  In the VKey object declaration, you need to select the appropriate analog input pin, and set the power indication to the proper voltage, either `THREE` or `FIVE`.

It is possible to attach several VKeys, each on it's own analog input pin, by declaring multiple VKey objects.

The VKey status is queried using the function
 
	bool CheckKeys(eKeynum & k);

The function will always return an indication of the current button in reference parameter k, or `NONE` in the case that no buttons are pressed.  The function also returns a boolean indication of whether the key has changed since the last time it was queried -- a form of edge detection.  Many applications will only care about handling the key number when it updates, and don't need to process the input when it is unchanging.

The next page will show us a quick sample sketch that uses the VKey library.    

---
##Example Code

...obtain the example sketch

#Fill this in after code is reviewed...

* includes
* obj decl
* no special setup...
* call `checkKeys()`

Build and upload the sketch, then open a serial terminal, and observe the output while pressing buttons.  The Arduino will print notifications as buttons are pressed and released. 

`pic`
-> *The Arduino recognizes keys based on the voltage produced by the VKey* <-

If the Arduino isn't properly tracking the key numbers, doublecheck that the VKey object was declared with the proper supply voltage.

---

##Resources and Going Further
 
The VKey is a great way to add up to 12 buttons to a project, while still leaving many I/O pins for other tasks.

###Modifications

There are a number of possible alterations that can be made to the VKey, to tailor it to a specific application.

#### Different Switches
The VKey comes with 12 PTH pushbutton switches, but can support nearly any normally-open SPST switch, such as [arcade switches](https://www.sparkfun.com/products/9336) or [microswitches](https://www.sparkfun.com/products/9414).


`pix`
-> *Connect alternate switches across the left pair of pads* <-

To connect other switches, connect them to the pair of pads on the left of the tact switch footprint, as seen from the front of the board.

#### Voltage Source

While the VKey is primarily intended to interface with a microcontroller, that doesn't mean it can't be used for other applications.  The circuit is similar to those used in analog synthesizers to extract a control voltage from the piano keyboard.  You could use the VKey to drive any device with a control voltage input.

###Digging Deeper

For more details about the internals of the VKey, check the *documents* directory of the [VKey Github repository](https://github.com/sparkfun/VKey).  You'll find a more detailed theory of operations document, with supporting  SPICE simulations and spreadsheets.

* schem
* eagle files zip
* [Github Repository](https://github.com/sparkfun/VKey)