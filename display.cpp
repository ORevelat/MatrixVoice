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

		_notif.function_register(event_notifier::HOTWORD_DETECTED, [&] (void*) { /* nothing */});
		_notif.function_register(event_notifier::RECORD_START, 
			[&] (void*) { 
				_stop_speech = false;
				std::thread t(&display::speech_started, this);
				t.detach();
		});
		_notif.function_register(event_notifier::RECORD_END, [&] (void*) { _stop_speech = true; });

		_notif.function_register(event_notifier::SPEAK_START, 
			[&] (void*) { 
				_stop_speak = false;
				std::thread t(&display::speak_started, this);
				t.detach();
		});
		_notif.function_register(event_notifier::SPEAK_END, [&] (void*) { _stop_speak = true; });
	}

	void display::initialise()
	{
		LOG(INFO) << "display initialising";

		_leds.Off();

		LOG(INFO) << "display initialised";
	}

	void display::deinitialise()
	{
		LOG(INFO) << "display deinitialising";
		
		_leds.Off();
		
		LOG(INFO) << "display deinitialised";
	}

	void display::speech_started()
	{
		_leds.On(leds::red);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		matrix_hal::LedValue c;
		
		for (int i=0;; i++) {
			for (int j=0; j<_leds.NumberLeds(); j++)
			{
				c.green = 50 * (((i + j) % 6 == 0) ? 1 : 0);

				_leds.Set(j, c);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			if (_stop_speech)
				break;
		}

		_leds.On(leds::green);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		_leds.Off();
	}

	void display::speak_started()
	{
		_leds.On(leds::blue);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		matrix_hal::LedValue c;
		
		for (int i=0;; i++) {
			for (int j=0; j<_leds.NumberLeds(); j++)
			{
				c.blue = 50 * (((i + j) % 3 == 0) ? 1 : 0);

				_leds.Set(j, c);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(150));
			if (_stop_speak)
				break;
		}

		_leds.On(leds::blue);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		_leds.Off();
	}

}