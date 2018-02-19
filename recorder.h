#pragma once

#include <memory>

#include "event-notifier.h"
#include "matrix_mics.h"

class Snowboy;

namespace sarah_matrix
{

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

		uint16_t total_tick_after_hotword;

		std::unique_ptr<Snowboy>	_detector;
	};

}