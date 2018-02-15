#include <iostream>
#include <string>

#include <glog/logging.h>

#include "speak.h"
#include "matrix_leds.h"

namespace sarah_matrix
{

	speak::speak(leds* l)
		: _leds(l)
	{}

	void speak::run(std::string in)
	{
	   	LOG(INFO) << "Speak - " << in;

		if (_leds)
			_leds->On(leds::white, 20, 500);

		/*
		pico2wave -l fr-FR -w /tmp/test.wav "$1"
		aplay -q /tmp/test.wav
		rm /tmp/test.wav
		*/
	}

}
