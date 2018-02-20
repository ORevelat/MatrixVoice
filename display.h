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
		
		void hotword_detected();
		void speech_ended();
		void speak_started();
		
 		private:
		event_notifier& _notif;
		leds& _leds;

		bool _stop_speak;
   };

}