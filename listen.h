#pragma once

#include <string>
#include <thread>

namespace sarah_matrix
{

	class mics;
	class leds;
	class http_client;

	class listen
	{
 	public:
		static listen* getInstance();
		static void delInstance();

		void start(mics*, leds*, http_client*);
		void stop();
		void wait();

		void speaking(bool b) { _speaking = b;}
		bool speaking() const { return _speaking; }

   private:
		listen();
		~listen();

		void run();
	
	private:
		bool			_exit;
		std::thread*	_thread;
		
		mics* 			_mics;
		leds* 			_leds;
		http_client*	_http;
		
		bool			_speaking;

		static listen*	_instance;
	};

}