#pragma once

#include <thread>

#include <matrix_hal/wishbone_bus.h>
#include <matrix_hal/everloop.h>

class ListenLeds
{
	public:
	enum color {
		red,
		green,
		blue,
		white
	};

	public:
	ListenLeds(matrix_hal::WishboneBus& bus)
	{
		image1d.leds.resize(NumberLeds());
		everloop.Setup(&bus);

		Off();
	}

	void On(const color c, const uint16_t delay = 250)
	{
		matrix_hal::LedValue x;

		switch (c)
		{
			case red:
				x.red = 50;
				break;
			case green:
				x.green = 50;
				break;
			case blue:
				x.blue = 50;
				break;
			case white:
				x.white = 50;
				break;
		}

		Set(x);

		if (delay > 0) {
			std::thread t(&ListenLeds::OffWithDelay, this, delay);
			t.detach();
		}
	}

	void Off()
	{
		for (matrix_hal::LedValue& led : image1d.leds) {
			led.red = 0;
			led.green = 0;
			led.blue = 0;
			led.white = 0;
		}
		everloop.Write(&image1d);
	}

	void Set(matrix_hal::LedValue val)
	{
		for (matrix_hal::LedValue& led : image1d.leds) {
			led.red = val.red;
			led.green = val.green;
			led.blue = val.blue;
			led.white = val.white;
		}
		everloop.Write(&image1d);
	}
	
	void Set(const uint index, const matrix_hal::LedValue& val)
	{
		image1d.leds[index % NumberLeds()].red = val.red;
		image1d.leds[index % NumberLeds()].green = val.green;
		image1d.leds[index % NumberLeds()].blue = val.blue;
		image1d.leds[index % NumberLeds()].white = val.white;
		everloop.Write(&image1d);
	}

	private:
	int NumberLeds() const { return 18; }

	void OffWithDelay(uint16_t delay)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		Off();
	}

	private:
		matrix_hal::Everloop		everloop;
		matrix_hal::EverloopImage	image1d;
};
