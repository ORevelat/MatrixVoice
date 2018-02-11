
#include <matrix_hal/wishbone_bus.h>

#include "everloop_image.h"
#include <matrix_hal/everloop.h>

class Leds
{
	public:
	Leds(matrix_hal::WishboneBus& bus)
	{
		everloop.Setup(&bus);
	}

	void Reset() {
		for (matrix_hal::LedValue& led : image1d.leds) {
			led.red = 0;
			led.green = 0;
			led.blue = 0;
			led.white = 0;
		}
		everloop.Write(&image1d);
	}

	void Set(matrix_hal::LedValue val) {
		for (matrix_hal::LedValue& led : image1d.leds) {
			led.red = val.red;
			led.green = val.green;
			led.blue = val.blue;
			led.white = val.white;
		}
		everloop.Write(&image1d);
	}

	void Set(const uint index, matrix_hal::LedValue val) {
		image1d.leds[index % NumberLeds()].red = val.red;
		image1d.leds[index % NumberLeds()].green = val.green;
		image1d.leds[index % NumberLeds()].blue = val.blue;
		image1d.leds[index % NumberLeds()].white = val.white;
		everloop.Write(&image1d);
	}


	int NumberLeds() const { return matrix_hal::kMatrixVoiceNLeds; }

	private:
		matrix_hal::Everloop		everloop;
		matrix_hal::EverloopImage	image1d;
};
