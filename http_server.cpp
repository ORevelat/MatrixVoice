#include <thread>

#include "main.h"
#include "http_server.h"

#include "include/httplib.h"

namespace sarah_matrix
{

	http_server::http_server(event_notifier& n)
		: _notif(n)
		, _ip(""), _port(0)
		, _http(0)
	{
		_notif.function_register(event_notifier::INITIALISE, std::bind(&http_server::initialise, this));
		_notif.function_register(event_notifier::DEINITIALISE, std::bind(&http_server::deinitialise, this));

		_notif.function_register(event_notifier::START,
			[&] (void*) { 
				std::thread t(&http_server::main_loop, this);
				t.detach();
		});
		_notif.function_register(event_notifier::STOP, [&] (void*) { 
			if (_http) {
				reinterpret_cast<httplib::Server*>(_http)->stop();
				std::unique_lock<std::mutex> lck(_exit_mtx);
    			_exit_cvar.wait(lck);
			}
		});
	}

	void http_server::initialise()
	{
		LOG(INFO) << "http_server initialising";

		_http = 0;
		_ip = FLAGS_local_ip;
		_port = FLAGS_local_port;

		LOG(INFO) << "http_server initialised";
	}

	void http_server::deinitialise()
	{
		LOG(INFO) << "http_server deinitialising";
		
		LOG(INFO) << "http_server deinitialised";
	}

	void http_server::main_loop()
	{
		using namespace httplib;

		Server svr;

		svr.get(R"(/speak/(.*))", [&](const Request& req, Response& res) {
			std::string text = req.matches[1];

			_notif.notify(event_notifier::SPEAK_RECEIVED, &text);

			res.set_content("OK", "text/plain");
		});

		svr.post(R"(/speak/)", [&](const Request& req, Response& res) {
			if (req.has_param("input")) {
				LOG(INFO) << " == question was: " << req.get_param_value("input");
			}

			if (req.has_param("output")) {
				std::string text = req.get_param_value("output");

				_notif.notify(event_notifier::SPEAK_RECEIVED, &text);
				res.set_content("OK", "text/plain");
			}
			else {
				res.set_content("KO", "text/plain");
			}
		});

		LOG(INFO) << " == http_server started on " << _ip << ":" << _port << " - access to /speak/(.*)";

		_http = &svr;
		svr.listen(_ip.c_str(), _port);
		_http = 0;
		
		LOG(INFO) << " == http_server stopped";
		_exit_cvar.notify_one();
	}

}