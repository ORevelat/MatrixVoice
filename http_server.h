#pragma once

#include <thread>
#include <functional>


namespace sarah_matrix
{

	class speak;

	class http_server
	{
	public:
		static http_server* getInstance();
		static void delInstance();

		void start(const std::string&, uint16_t, speak*);
		void stop();
		void wait();

	private:
		http_server();
		~http_server();

		void run(const std::string&, uint16_t, speak*);
		
	private:
		bool			    _exit;
		std::thread*	    _thread;
		void*    			_http;

		static http_server* _instance;
	};

}
