#pragma once

#include <string>

#include "matrix_leds.h"

namespace sarah_matrix
{

	class speak
	{
	public:
		speak(leds*);

		void run(std::string in);

	private:
		leds* _leds;
	};

}
