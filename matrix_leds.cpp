
#include "matrix_leds.h"

namespace sarah_matrix
{

	leds::leds(matrix_hal::WishboneBus& bus)
	{
		_everloop.Setup(&bus);
		_image1d.leds.resize(NumberLeds());
	}

	void leds::On(const color c, const uint16_t intensity)
	{
		matrix_hal::LedValue x;

		switch (c)
		{
			case red:
				x.red = intensity;
				break;
			case green:
				x.green = intensity;
				break;
			case blue:
				x.blue = intensity;
				break;
			case white:
				x.white = intensity;
				break;
		}

		Set(x);
	}

    void leds::Off()
    {
		for (matrix_hal::LedValue& led : _image1d.leds) {
			led.red = 0;
			led.green = 0;
			led.blue = 0;
			led.white = 0;
		}
		_everloop.Write(&_image1d);
    }

	void leds::Set(matrix_hal::LedValue val)
	{
		for (matrix_hal::LedValue& led : _image1d.leds) {
			led.red = val.red;
			led.green = val.green;
			led.blue = val.blue;
			led.white = val.white;
		}
		_everloop.Write(&_image1d);
	}
	
    void leds::Set(const uint index, const matrix_hal::LedValue& val)
	{
		_image1d.leds[index % NumberLeds()].red = val.red;
		_image1d.leds[index % NumberLeds()].green = val.green;
		_image1d.leds[index % NumberLeds()].blue = val.blue;
		_image1d.leds[index % NumberLeds()].white = val.white;
		_everloop.Write(&_image1d);
	}	
}