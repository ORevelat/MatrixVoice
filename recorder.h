#pragma once

#include <cstring>
#include <memory>

#include "event-notifier.h"
#include "matrix_mics.h"

#include "snowboy_wrapper.h"

namespace sarah_matrix
{

	struct record_state
	{
		record_state()
		{
			average_energy = 0;
			tick_after_hotword = 0;
			total_tick_after_hotword = 0;
			record_len = 0;

			std::memset(record_buffer, 0, sizeof record_buffer);
		}

		void reset(int64_t avg = 0, bool reset_len = false)
		{
			average_energy = avg;
			tick_after_hotword = 0;
			total_tick_after_hotword = 0;
			if (reset_len)
				record_len = 0;		
		}

		int64_t average_energy;
		uint16_t tick_after_hotword;
		uint16_t total_tick_after_hotword;
		uint64_t record_len;

		int16_t	record_buffer[(10000 + 1) * (WINDOW_SIZE / 256)];
	};

	class recorder
	{
		public:
		recorder(event_notifier&, microphones&);

		void record();

		private:
		void initialise();
		void deinitialise();

		void speak_started();
		void speak_stopped();

		private:
		event_notifier& _notif;
		microphones& _mics;

		bool _isplaying;

		std::unique_ptr<Snowboy>		_detector;
		std::unique_ptr<record_state>	_state;
	};

}