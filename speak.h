#pragma once

#include <string>

#include "matrix_leds.h"

namespace sarah_matrix
{

	class listen;

	class speak
	{
	public:
		speak(leds*, listen*);

		void run(std::string in);

	private:
		void loop_leds(bool*);

	private:
		leds* 	_leds;
		listen*	_listen;
	};

}
