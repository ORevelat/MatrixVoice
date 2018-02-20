#include <thread>

#include "main.h"
#include "display.h"

namespace sarah_matrix
{

	display::display(event_notifier& n, leds& l)
		: _notif(n), _leds(l)
	{
		_notif.function_register(event_notifier::INITIALISE, std::bind(&display::initialise, this));
		_notif.function_register(event_notifier::DEINITIALISE, std::bind(&display::deinitialise, this));

		_notif.function_register(event_notifier::HOTWORD_DETECTED, [&] (void*) { std::thread t(&display::hotword_detected, this); t.detach(); });
		_notif.function_register(event_notifier::RECORD_START, [&] (void*) { /* nothing */ });
		_notif.function_register(event_notifier::RECORD_END, [&] (void*) { std::thread t(&display::speech_ended, this); t.detach(); });

		_notif.function_register(event_notifier::SPEAK_START, [&] (void*) { _stop_speak = false; std::thread t(&display::speak_started, this); t.detach(); });
		_notif.function_register(event_notifier::SPEAK_END, [&] (void*) { _stop_speak = true; });
	}

	void display::initialise()
	{
		LOG(INFO) << "display initialising";

		LOG(INFO) << "display initialised";
	}

	void display::deinitialise()
	{
		LOG(INFO) << "display deinitialising";
		
		LOG(INFO) << "display deinitialised";
	}

	void display::hotword_detected()
	{
		_leds.On(leds::red);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		_leds.Off();
	}

	void display::speech_ended()
	{
		_leds.On(leds::green);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		_leds.Off();
	}

	void display::speak_started()
	{
		matrix_hal::LedValue c;

		c.blue = 50;
		_leds.Set(c);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		c.blue = 0;
		c.white = 20;

		for (int i=0;; i++) {
			_leds.Set(i, c);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			_leds.Off();
			if (_stop_speak)
				break;
		}

		c.white = 0;
		c.blue = 50;
		_leds.Set(c);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		_leds.Off();
	}

}