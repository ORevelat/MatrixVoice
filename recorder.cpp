#include <thread>
#include <fstream>

#include "main.h"
#include "recorder.h"

namespace sarah_matrix
{

	inline static std::string newFilename()
	{
		time_t t = time(0);
		struct tm * now = localtime( & t );

		char buffer [80];
		strftime (buffer,80,"testrecord-%Y%m%d%H%M%S.raw",now);

		return std::string(buffer);
	}
	
	recorder::recorder(event_notifier& n, microphones& mics)
		: _notif(n), _mics(mics)
		, _exit(false), _isplaying(false)
	{
		_notif.function_register(event_notifier::INITIALISE, std::bind(&recorder::initialise, this));
		_notif.function_register(event_notifier::DEINITIALISE, std::bind(&recorder::deinitialise, this));

		_notif.function_register(event_notifier::START,
			[&] (void*) { 
				_exit = false;
				std::thread t(&recorder::main_loop, this);
				t.detach();
		});
		_notif.function_register(event_notifier::STOP,
			[&] (void*) { 
				_exit = true; 
				std::unique_lock<std::mutex> lck(_exit_mtx);
    			_exit_cvar.wait(lck);
		});

		_notif.function_register(event_notifier::SPEAK_START, [&] (void*) { _isplaying = true; });
		_notif.function_register(event_notifier::SPEAK_END, [&] (void*) { _isplaying = false; });
	}

	void recorder::initialise()
	{
		LOG(INFO) << "recorder initialising";

		_detector.reset(new Snowboy("resources/common.res", std::string("resources/" + FLAGS_hotword_model).c_str()));
		_detector.get()->SetSensitivity(FLAGS_hotword_sensitivity.c_str());
		_detector.get()->SetAudioGain(1.0);
		_detector.get()->ApplyFrontend(FLAGS_frontend_algo);

		LOG(INFO) << " == using model=" << FLAGS_hotword_model << " , sensitivity=" << FLAGS_hotword_sensitivity << " , frontend=" << FLAGS_frontend_algo;
		LOG(INFO) << " == saverecord=" << FLAGS_saverecord;

		_state.reset(new record_state(10, _mics.NumberSample(), _mics.SampleRate()));

		LOG(INFO) << "recorder initialised";
	}

	void recorder::deinitialise()
	{
		LOG(INFO) << "recorder deinitialising";

		_state.reset();
		_detector.reset();
		
		LOG(INFO) << "recorder deinitialised";
	}

	void recorder::main_loop()
	{
		if (!_detector || !_state)
			return;

		LOG(INFO) << " == recorder started";
		
		while(!_exit)
		{
			_mics.read();

			int64_t avg = _mics.average_energy();

			// if playing or speech recording, do not perform hotword detection
			if (!_isplaying && !_state.get()->is_recording())
			{
				int result = _detector.get()->RunDetection(_mics.last(), _mics.NumberSample());
				if (result > 0) 
				{
					_state.get()->start_record(avg);

					_notif.notify(event_notifier::HOTWORD_DETECTED);
					_notif.notify(event_notifier::RECORD_START);
				}
			}

			if (_state.get()->is_recording())
			{
				// copy to keep raw buffer
				_state.get()->copy_to_buffer(_mics.last(), _mics.NumberSample());

				// increment frame record counter
				_state.get()->increment_ticks(avg);
			}

			// if end of speech (silence or max record time)
			if (_state.get()->speech_ended())
			{
				auto recorded = _state.get()->infobuffer();

				if(FLAGS_saverecord)
				{
					std::ofstream os(newFilename(), std::ofstream::binary);
					os.write((const char*)recorded->Buffer(), recorded->Length() * sizeof(int16_t));
					os.close();
				}
				
				_notif.notify(event_notifier::RECORD_END, recorded);
				_state.get()->reset();
			}
		}

		LOG(INFO) << " == recorder stopped";
		_exit_cvar.notify_one();
	}
}
