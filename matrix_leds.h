#pragma once

#include <matrix_hal/matrixio_bus.h>
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
		leds(matrix_hal::MatrixIOBus&);

		void On(const color, const uint16_t intensity = 50);
		void Off();

		void Set(matrix_hal::LedValue);
		void Set(const uint, const matrix_hal::LedValue&);

		int NumberLeds() const { return 18; }
        
	private:
		matrix_hal::Everloop		_everloop;
		matrix_hal::EverloopImage	_image1d;
   };

}