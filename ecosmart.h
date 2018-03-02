// ecosmart.h
// EcoSmart Water Heater control protocol
// By Ryan Griggs/hilltop.net
// MIT licensed - commercial use, attribution required
// Copyright (c) 2017

// This library currently only writes commands to the EcoSmart.	It does not handle input.

#ifndef ECOSMART_LIBRARY
#define ECOSMART_LIBRARY

// START sequence timing:
#define ECO_START1 4000 // 4 mS (LOW) (minimum length)
#define ECO_START2 7000 // 7 mS (HIGH)
#define ECO_START3 4000 // 4 mS (LOW)

// Bit timing:
#define ECO_LOW_BIT 720	 // 720uS	(HIGH)
#define ECO_HIGH_BIT 2400 // 2.4mS	(HIGH)
#define ECO_SPACER 840		// 840uS	(LOW)

#define ECO_TEMP_PREFIX1 0x0f
#define ECO_TEMP_PREFIX2 0x3c
#define ECO_TEMP_PREFIX3 0x08;	// Prefix to send.

class EcoSmart
{
private:
	void outputLow();
	void outputHigh();
	void outputStart();
	byte command[5];	// Command to be sent.

	int output_pin;	// The pin to use for digitalWrites to the ecosmart.

public:


	void init(int);
	void writeCommand(byte*);
	void setTemperature(int temperature);

};


void EcoSmart::init(int pin)
{
	output_pin = pin;
	pinMode(output_pin, OUTPUT);
}

void EcoSmart::setTemperature(int temperature)
// Send command to set the F temperature.
{
	if (temperature < 70) { temperature = 70; }
	if (temperature > 140) { temperature = 140; }

	int c = round((temperature - 32) * 0.5555);	// Convert to Celsius

	// Convert temperature into binary string
	// NOTE: to actually control the temperature, both the F and C temperature values must be specified.
	// if only the F temperature is specified and the C temperature is set to zero, the heater attempts
	// to output 0 degree water (no heat applied), which is evidenced by switching to Celsius mode after sending the setTemperature command.
	byte command[5];
	command[0] = ECO_TEMP_PREFIX1;
	command[1] = ECO_TEMP_PREFIX2;
	command[2] = ECO_TEMP_PREFIX3;
	command[3] = (byte) temperature;	// Fahrenheit
	command[4] = (byte) c;				// Celsius

	writeCommand(command);

}


void EcoSmart::writeCommand(byte *command)
{
	this->outputStart();
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (command[i] & 0x80)	// High bit
			{
				this->outputHigh();
			}
			else
			{
				this->outputLow();
			}
			command[i] = command[i] << 1;	// Shift left.
		}
	}
}	


void EcoSmart::outputLow()
// Output a Logic Low value to the serial stream
{
	digitalWrite(this->output_pin, HIGH);
	delayMicroseconds(ECO_LOW_BIT);
	digitalWrite(this->output_pin, LOW);
	delayMicroseconds(ECO_SPACER);
}

void EcoSmart::outputHigh()
// Output a Logic High value to the serial stream
{
	digitalWrite(this->output_pin, HIGH);
	delayMicroseconds(ECO_HIGH_BIT);
	digitalWrite(this->output_pin, LOW);
	delayMicroseconds(ECO_SPACER);
}

void EcoSmart::outputStart()
// Output START sequence
{
	digitalWrite(this->output_pin, LOW);
	delayMicroseconds(ECO_START1);
	digitalWrite(this->output_pin, HIGH);
	delayMicroseconds(ECO_START2);
	digitalWrite(this->output_pin, LOW);
	delayMicroseconds(ECO_START3);
}

#endif
