#include <iostream>
#include <string>
#include <thread>

#include <glog/logging.h>

#include "speak.h"

#include "matrix_leds.h"

namespace sarah_matrix
{

	speak::speak(const std::string& alsaout, leds* l)
		: _leds(l), _alsaout(alsaout), _isSpeaking(false)
	{
		if (_alsaout.empty())
	   		LOG(INFO) << "Speak initialised";
		else
	   		LOG(INFO) << "Speak initialised using option " << _alsaout;
	}

	void speak::tts(const std::string& in)
	{
		std::thread t(&speak::thread_tts, this, in);
		t.detach();
	}

	void speak::sound(const std::string& w)
	{
		std::thread t(&speak::thread_sound, this, w);
		t.detach();
	}

	void speak::thread_tts(std::string text)
	{
	   	LOG(INFO) << "== tts - " << text;

		_isSpeaking = true;

		bool stoploop = false;

		std::thread t(&speak::thread_leds, this, &stoploop);

		std::string command("pico2wave -l fr-FR -w /tmp/test.wav ");
		command += "\"" + text + "\"";
		system(command.c_str());
		
		std::string aplay = "aplay -q " + _alsaout + " /tmp/test.wav";
		system(aplay.c_str());

		system("rm /tmp/test.wav");

		stoploop = true;
		t.join();

		_isSpeaking = false;
	}

	void speak::thread_sound(std::string file)
	{
	   	LOG(INFO) << "== sound - " << file;

		std::string aplay = "aplay -q " + _alsaout + " ./resources/" + file;
		system(aplay.c_str());
	}

	void speak::thread_leds(bool* stoploop)
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


}
