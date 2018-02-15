#include <iostream>

#include <glog/logging.h>

#include "include/httplib.h"

#include "http_server.h"
#include "speak.h"

namespace sarah_matrix
{

	// singleton initialization
	http_server* http_server::_instance = 0;

	http_server* http_server::getInstance()
	{
		if (_instance == 0)
			_instance = new http_server();
		return _instance;
	}
	
	void http_server::delInstance()
	{
		if (_instance)
		{
			_instance->stop();

			delete _instance;
			_instance = 0;
		}
	}

	http_server::http_server()
		: _exit(false), _thread(0), _http(0)
	{ }

	http_server::~http_server()
	{
		stop();
	}

	void http_server::start(const std::string& ip, uint16_t port, std::function< void(std::string) > func)
	{
		_thread = new std::thread(&http_server::run, this, ip, port, func);
	}

	void http_server::stop()
	{
		using namespace httplib;

		if (_http)
			reinterpret_cast<Server*>(_http)->stop();
	}

	void http_server::wait()
	{
		if(_thread)
		{
			_thread->join();
			delete _thread;
			_thread = 0;
		}
	}

	void http_server::run(const std::string& ip, uint16_t port, std::function< void(std::string) > func)
	{
		using namespace httplib;

		Server svr;

		svr.get(R"(/speak/(.*))", [&](const Request& req, Response& res) {
			auto speak = req.matches[1];

			std::thread t(func, speak);
			t.detach();

			res.set_content("OK", "text/plain");
		});

		LOG(INFO) << "Http server started on " << ip << ":" << port << " - access to /speak/(.*)";

		_http = &svr;
		svr.listen(ip.c_str(), port);
		_http = 0;
		
		LOG(INFO) << "Http server stopped";
	}

}
