#pragma once

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

		void run();

		private:
		event_notifier&	_notif;

		std::string _ip;
		uint16_t	_port;

		void* _http;
	};

}