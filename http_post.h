#pragma once

#include <string>

#include "event-notifier.h"

namespace sarah_matrix
{

	class http_post
	{
		public:
		http_post(event_notifier&);

		private:
		void initialise();
		void deinitialise();

		void post(void*);
		
		static inline size_t write_data(void *, size_t, size_t, void *);
		static inline size_t read_callback(void *, size_t, size_t, void *);

		private:
		event_notifier&	_notif;
		
		std::string	_url;
	};

}