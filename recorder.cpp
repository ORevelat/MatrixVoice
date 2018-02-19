

#include "main.h"
#include "recorder.h"

#include "snowboy_wrapper.h"

namespace sarah_matrix
{
	
	recorder::recorder(event_notifier& n, microphones& mics)
		: _notif(n), _mics(mics)
		, _isplaying(false)
	{
		_notif.function_register(event_notifier::INITIALISE, std::bind(&recorder::initialise, this));
		_notif.function_register(event_notifier::DEINITIALISE, std::bind(&recorder::deinitialise, this));
		_notif.function_register(event_notifier::SPEAK_START, [&] { _isplaying = true; });
		_notif.function_register(event_notifier::SPEAK_END, std::bind(&recorder::speak_stopped, this));
	}

	void recorder::initialise()
	{
		LOG(INFO) << "recorder initialising";

		_decoder = new Snowboy("resources/common.res", "resources/sarah.pmdl");
		LOG(INFO) << "recorder initialised";
	}

	void recorder::deinitialise()
	{
		LOG(INFO) << "recorder deinitialising";
		LOG(INFO) << "recorder deinitialised";
	}

	void recorder::speak_started()
	{
		_isplaying = true;
	}

	void recorder::speak_stopped()
	{
		_isplaying = false;
	}

	void recorder::record()
	{
		_mics.read();

		int64_t avg = _mics.average_energy();
		if (!_isplaying && (total_tick_after_hotword == 0))
		{
			int result = detector.RunDetection(_mics->last(), NUMBER_SAMPLE);
		}
	}
}
