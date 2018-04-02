#pragma once

#include <iostream>
#include <cstring>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "event-notifier.h"
#include "matrix_mics.h"

#include "recorder.state.h"

#include "snowboy_wrapper.h"

namespace sarah_matrix
{

	class recorder
	{
		public:
		recorder(event_notifier&, microphones&);

		private:
		void initialise();
		void deinitialise();

		void main_loop();

		void speak_started();
		void speak_stopped();

		private:
		event_notifier& _notif;
		microphones& _mics;

		bool _exit;
		bool _isplaying;

		std::unique_ptr<Snowboy>		_detector;
		std::unique_ptr<record_state>	_state;

		std::mutex _exit_mtx;
		std::condition_variable _exit_cvar;
	};

}