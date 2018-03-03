#pragma once

#include <mutex>
#include <condition_variable>

#include "event-notifier.h"

namespace sarah_matrix
{

	class http_server
	{
		public:
		http_server(event_notifier&);
		
		private:
		void initialise();
		void deinitialise();

		void main_loop();

		private:
		event_notifier&	_notif;

		std::string _ip;
		uint16_t	_port;

		void* _http;

		std::mutex _exit_mtx;
		std::condition_variable _exit_cvar;
	};

}