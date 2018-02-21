#pragma once

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
		
 		private:
		event_notifier& _notif;
	};

}
