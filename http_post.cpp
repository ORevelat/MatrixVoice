#include <functional>

#include "main.h"
#include "http_post.h"

#include "recorder.h"

namespace sarah_matrix
{
	http_post::http_post(event_notifier& n)
		: _notif(n)
	{
		_notif.function_register(event_notifier::INITIALISE, std::bind(&http_post::initialise, this));
		_notif.function_register(event_notifier::DEINITIALISE, std::bind(&http_post::deinitialise, this));
		_notif.function_register(event_notifier::RECORD_END, std::bind(&http_post::post, this, std::placeholders::_1));
	}

	void http_post::initialise()
	{
		LOG(INFO) << "http_post initialising";

		_url = FLAGS_remote_url;
		LOG(INFO) << " == using remote url=" << _url;

		LOG(INFO) << "http_post initialised";
	}

	void http_post::deinitialise()
	{
		LOG(INFO) << "http_post deinitialising";

		LOG(INFO) << "http_post deinitialised";
	}

	void http_post::post(void* st)
	{
		record_state* state = reinterpret_cast<record_state*>(st);

		LOG(INFO) << " == sending " << state->record_len << " bytes to " << _url;
	}
}