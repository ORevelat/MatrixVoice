#include <unistd.h>
#include <iostream>

#include <matrix_hal/wishbone_bus.h>

#include "leds.h"

namespace hal = matrix_hal;

int main(int argc, const char** argv)
{
	std::cout << "Initializing ..." << std::endl;

	hal::WishboneBus bus;
	bus.SpiInit();

	std::cout << "Running ..." << std::endl;

	Leds leds(bus);
	
	hal::LedValue x;
	x.red = 30;

	for(int count = 0; count < 10 * leds.NumberLeds(); ++count) {
		leds.Reset();
		leds.Set(count, x);
		usleep(100000);
	}

	std::cout << "Cleaning ..." << std::endl;

	leds.Reset();
	bus.SpiClose();

	std::cout << "Done" << std::endl;
	return 0;
}

