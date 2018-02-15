#pragma once

#include <matrix_hal/wishbone_bus.h>
#include <matrix_hal/everloop.h>

namespace sarah_matrix
{

	class leds
	{
	public:
		enum color {
			red,
			green,
			blue,
			white
		};
		
	public:
		leds(matrix_hal::WishboneBus&);

		void On(const color, const uint16_t);
		void On(const color, const uint16_t, const uint16_t);
		void Off();

		void Set(matrix_hal::LedValue);
		void Set(const uint, const matrix_hal::LedValue&);

	private:
		int NumberLeds() const { return 18; }

		void OffWithDelay(uint16_t);

	private:
		matrix_hal::Everloop		everloop;
		matrix_hal::EverloopImage	image1d;
	};

}
