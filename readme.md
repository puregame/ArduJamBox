# Arduino Radio

This is the code portion for an arduino-based radio, complete with a bluetoth or any similar desired modules.

### User input:
The project uses a simple rotary encoder for frequency and input adjustments, the ClickEncoder library was used to greatly simplify the implementation, see: [0xPIT/encoder](https://github.com/0xPIT/encoder/tree/arduino) for more details.

### Sound Input:
For FM connectivity the project uses the incredibly cheap and ever useful TEA5767 radio chip, available at just a google search away.

For bluetooth connectivity the project uses a simple bluetooth module, I got mine from ebay for about $5, it has a USB on one end and a 3.5mm jack on the other. Any module with matching description will do.

An auxilary jack is also very easy to integrate, since 

### Other Parts:
1. DIY arduino (any arduino will do):
	- Atmega328P
	- 16mhz oscillator
	- Button and light for basic I/O
2. Rotary Encoder
3. PAM8403 audio amplifier
4. TEA5767 radio chip
5. wire, solder, resistors, header pins, ect.

Schematics and build log to come.