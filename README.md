# EcoSmartLib
An Arduino library to interface with EcoSmart tankless water heaters

I have written a library which allows commands to be sent to the EcoSmart heaters from an Arduino. 

I have not completed the 'read' portion of the library.  I have recorded all the EcoSmart's commands via oscilloscope and translated them into binary sequences but have not had time or need to implement them yet.

Here are the details of the EcoSmart protocol
See https://electronics.stackexchange.com/questions/233374/reverse-engineering-asynchronous-serial-protocol-for-ecosmart-tankless-water-hea

The data stream encodes both F and C temperature values.

Pinout notes: (may be inaccurate.  Test with multimeter and scope to ensure accuracy before connecting!)
Pin 1 - Black wire (Serial Output 5VDC)?
Pin 2 - Red wire (Serial Input 5VDC)?
Pin 3 - Green wire (+13VDC)
Pin 4 - Yellow wire (GND)

Here are some data captures from the unit when changing temperature from one value to another, etc:
From 89 degrees F to 90 degrees F (turning wheel)
[START]0000 1111 0011 1100 0000 1000 0101 1010 0010 0000
(the unit repeats each command six times)

From 90 to 91 degrees F
[START] 0000 1111 0011 1100 0000 1000 0101 1011 0010 0001
(repeat six times)

From this format, you can see that the data can be parsed as follows:

[START]
[HEADER, 2 bytes: always 0000 1111 0011 1100] (0F3Ch)
[COMMAND, 1 byte: see below 0000 1000]
[F temperature, 1 byte: integer value 0101 1011]
[C temperature 1 byte: integer value 0010 0001]

Commands:
Turn wheel to adjust temperature: 0000 1000 (08h)
Press button to turn On: 0000 0000 (00h)
Press button to turn Off: 0000 1000 (08h)(not sure why this is the same as temp change)
Start (water flow): 0010 1000 (28h)
Stop (water flow): 0000 1000 (08h)

