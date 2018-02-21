#pragma once

#include <string>

#include "event-notifier.h"

namespace sarah_matrix
{

	class speaker
	{
		public:
		speaker(event_notifier&);

		private:
		void initialise();
		void deinitialise();
		
		void speak(void*);
		void play(void*);

 		private:
		event_notifier& _notif;

		std::string _alsaopt;
	};

}
