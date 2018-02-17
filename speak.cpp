#include <iostream>
#include <string>
#include <thread>

#include <glog/logging.h>

#include "speak.h"
#include "listen.h"

#include "matrix_leds.h"

namespace sarah_matrix
{

	speak::speak(leds* l, listen* ls)
		: _leds(l), _listen(ls)
	{}

	void speak::loop_leds(bool* stoploop)
	{
		matrix_hal::LedValue c;

		c.blue = 50;
		_leds->Set(c);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		c.blue = 0;
		c.white = 20;

		for (int i=0;; i++) {
			_leds->Set(i, c);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			_leds->Off();
			if (*stoploop)
				break;
		}

		c.white = 0;
		c.blue = 50;
		_leds->Set(c);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		_leds->Off();
	}

	void speak::run(std::string in)
	{
	   	LOG(INFO) << "== speak - " << in;

		_listen->speaking(true);

		bool stoploop = false;

		std::thread t(&speak::loop_leds, this, &stoploop);

		std::string command("pico2wave -l fr-FR -w /tmp/test.wav ");

		command += "\"" + in + "\"";
		system(command.c_str());
		system("aplay -q -q /tmp/test.wav");
		system("rm /tmp/test.wav");

		stoploop = true;
		t.join();

		_listen->speaking(false);
	}

}
