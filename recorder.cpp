
#include "main.h"
#include "recorder.h"

#define SILENCE_COUNT	4 * WINDOW_SIZE / 256
#define MAX_COUNT		10000 / (WINDOW_SIZE / 256)

namespace sarah_matrix
{
	
	recorder::recorder(event_notifier& n, microphones& mics)
		: _notif(n), _mics(mics)
		, _isplaying(false)
	{
		_notif.function_register(event_notifier::INITIALISE, std::bind(&recorder::initialise, this));
		_notif.function_register(event_notifier::DEINITIALISE, std::bind(&recorder::deinitialise, this));
		_notif.function_register(event_notifier::SPEAK_START, [&](void*) { _isplaying = true; });
		_notif.function_register(event_notifier::SPEAK_END, [&](void*) { _isplaying = false; });
	}

	void recorder::initialise()
	{
		LOG(INFO) << "recorder initialising";

		_detector.reset(new Snowboy("resources/common.res", std::string("resources/" + FLAGS_hotword_model).c_str()));
		_detector.get()->SetSensitivity(FLAGS_hotword_sensitivity.c_str());
		_detector.get()->SetAudioGain(1.0);
		_detector.get()->ApplyFrontend(FLAGS_frontend_algo);

		LOG(INFO) << " == using model=" << FLAGS_hotword_model << " , sensitivity=" << FLAGS_hotword_sensitivity << " , frontend=" << FLAGS_frontend_algo;

		_state.reset(new record_state());

		LOG(INFO) << "recorder initialised";
	}

	void recorder::deinitialise()
	{
		LOG(INFO) << "recorder deinitialising";

		_state.reset();
		_detector.reset();
		
		LOG(INFO) << "recorder deinitialised";
	}

	void recorder::record()
	{
		if (!_detector || !_state)
			return;
	
		_mics.read();

		int64_t avg = _mics.average_energy();
		if (!_isplaying && (_state.get()->total_tick_after_hotword == 0))
		{
			int result = _detector.get()->RunDetection(_mics.last(), NUMBER_SAMPLE);
			if (result > 0) 
			{
				_state.get()->reset(avg, true);

				_notif.notify(event_notifier::HOTWORD_DETECTED);
				_notif.notify(event_notifier::RECORD_START);
			}
		}

		if (_state.get()->average_energy > 0)
		{
			// copy to keep raw buffer
			std::memcpy(_state.get()->record_buffer + _state.get()->record_len, _mics.last(), NUMBER_SAMPLE * sizeof(int16_t));
			_state.get()->record_len += NUMBER_SAMPLE;

			_state.get()->total_tick_after_hotword++;
			if (avg < _state.get()->average_energy)
				_state.get()->tick_after_hotword ++;
			else
				_state.get()->tick_after_hotword = 0;
		}

		if ((_state.get()->tick_after_hotword > SILENCE_COUNT) || (_state.get()->total_tick_after_hotword > MAX_COUNT))
		{
			_state.get()->reset();

			_notif.notify(event_notifier::RECORD_END, (void*)_state.get());
		}
	}
}
