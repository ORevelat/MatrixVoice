#include "main.h"
#include "http_server.h"

#include "include/httplib.h"

namespace sarah_matrix
{

	http_server::http_server(event_notifier& n)
		: _notif(n), _ip(""), _port(0), _http(0)
	{
		_notif.function_register(event_notifier::INITIALISE, std::bind(&http_server::initialise, this));
		_notif.function_register(event_notifier::DEINITIALISE, std::bind(&http_server::deinitialise, this));
	}

	void http_server::initialise()
	{
		LOG(INFO) << "http_server initialising";

		_http = 0;
		_ip = FLAGS_local_ip;
		_port = FLAGS_local_port;

		std::thread t(&http_server::run, this);
		t.detach();

		LOG(INFO) << "http_server initialised";
	}

	void http_server::deinitialise()
	{
		LOG(INFO) << "http_server deinitialising";
		
		if (_http)
			reinterpret_cast<httplib::Server*>(_http)->stop();

		LOG(INFO) << "http_server deinitialised";
	}

	void http_server::run()
	{
		using namespace httplib;

		Server svr;

		svr.get(R"(/speak/(.*))", [&](const Request& req, Response& res) {
			auto text = req.matches[1];

			_notif.notify(event_notifier::SPEAK_RECEIVED, &text);

			res.set_content("OK", "text/plain");
		});

		LOG(INFO) << " == http_server started on " << _ip << ":" << _port << " - access to /speak/(.*)";

		_http = &svr;
		svr.listen(_ip.c_str(), _port);
		_http = 0;
		
		LOG(INFO) << " == http_server stopped";
	}

}