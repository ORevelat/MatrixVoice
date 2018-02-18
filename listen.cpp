#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

#ifdef RECORD_TOFILE
#include <chrono>
#include <ctime>
#endif

#include <glog/logging.h>

#include "listen.h"

#include "matrix_mics.h"
#include "matrix_leds.h"

#include "http_client.h"
#include "speak.h"

#include "snowboy_wrapper.h"

#define SILENCE_COUNT	3 * WINDOW_SIZE / 256
// 10 sec maximum
#define MAX_COUNT		10000 / (WINDOW_SIZE / 256)

#define SNOWBOY_FRONTEND_ALGO	false

namespace sarah_matrix
{

	// singleton initialization
	listen* listen::_instance = 0;

	listen* listen::getInstance()
	{
		if (_instance == 0)
			_instance = new listen();
		return _instance;
	}
	
	void listen::delInstance()
	{
		if (_instance)
		{
			delete _instance;
			_instance = 0;
		}
	}

	listen::listen()
		: _exit(false), _thread(0), _mics(0), _leds(0), _http(0)
	{
	}

	listen::~listen()
	{
		stop();
	}

	void listen::start(mics* m, leds* l, http_client* h, speak* sp)
	{
		_mics= m;
		_leds = l;
		_http = h;
		_speak = sp;

		_exit = false;
		_thread = new std::thread(&listen::run, this);
	}

	void listen::stop()
	{
		_exit = true;
	}

	void listen::wait()
	{
		if(_thread)
		{
			_thread->join();
			delete _thread;
			_thread = 0;
		}
	}

	void listen::run()
	{
		LOG(INFO) << "Initializing listening ...";

		// Initializes Snowboy detector.
		Snowboy detector("resources/common.res", "resources/sarah.pmdl");
		detector.SetSensitivity("0.30");
		detector.SetAudioGain(1.0);
		detector.ApplyFrontend(SNOWBOY_FRONTEND_ALGO);

		LOG(INFO) << "Starting voice listening ...";

		int64_t avg_for_hotword = 0;
		uint16_t tick_after_hotword = 0;
		uint16_t total_tick_after_hotword = 0;

		// up to 10 seconds of sound
		int16_t	record_buffer[10000 * (WINDOW_SIZE / 256)];
		size_t	record_len = 0;

		while (!_exit)
		{
			int64_t wnd_avg = _mics->read();

			// detect hotword only if we are not recording nor speaking
			if (!_speak->is_speaking() && (total_tick_after_hotword == 0))
			{
				int result = detector.RunDetection(_mics->last(), NUMBER_SAMPLE);
				if (result > 0) {
					avg_for_hotword = wnd_avg;
					tick_after_hotword = 0;
					total_tick_after_hotword = 0;
					record_len = 0;

					_leds->On(leds::red, 500);
					_speak->sound();
				}
			}
		
			if (avg_for_hotword > 0) {
				// copy to keep raw buffer
				std::memcpy(record_buffer + record_len, _mics->last(), NUMBER_SAMPLE * sizeof(int16_t));
				record_len += NUMBER_SAMPLE;

				total_tick_after_hotword++;
				if (wnd_avg < avg_for_hotword)
					tick_after_hotword ++;
				else
					tick_after_hotword = 0;
			}

			if ((tick_after_hotword > SILENCE_COUNT) || (total_tick_after_hotword > MAX_COUNT))
			{
				avg_for_hotword = 0;
				tick_after_hotword = 0;
				total_tick_after_hotword = 0;

				_leds->On(leds::green, 250);

				_http->Send(record_buffer, record_len);

#ifdef RECORD_TOFILE
				std::ofstream os;
				std::string filename = "recorded.raw";
    			os.open(filename, std::ofstream::binary);
				os.write((const char *)record_buffer, record_len * sizeof(int16_t));
				os.close();
#endif
			}
		}
		
		LOG(INFO) << "Voice listening stopped";
	}

}
