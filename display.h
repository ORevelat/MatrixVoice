#pragma once

#include "event-notifier.h"
#include "matrix_leds.h"

namespace sarah_matrix
{

	class display
	{
		public:
		display(event_notifier&, leds&);

		private:
		void initialise();
		void deinitialise();
		
		void speech_started();
		void speak_started();
		
 		private:
		event_notifier& _notif;
		leds& _leds;

		bool _stop_speech;
		bool _stop_speak;
   };

}