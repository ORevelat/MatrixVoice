#include "main.h"
#include "speaker.h"

namespace sarah_matrix
{

	speaker::speaker(event_notifier& n)
		: _notif(n)
	{

	}
	
	void speaker::initialise()
	{
		LOG(INFO) << "speaker initialising";

		// FLAGS_alsaout;

		LOG(INFO) << "speaker initialised";
	}

	void speaker::deinitialise()
	{
		LOG(INFO) << "speaker deinitialising";
		
		LOG(INFO) << "speaker deinitialised";
	}
}
