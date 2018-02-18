#pragma once

#include <string>
#include <thread>

namespace sarah_matrix
{

	class mics;
	class leds;
	class http_client;
	class speak;

	class listen
	{
 	public:
		static listen* getInstance();
		static void delInstance();

		void start(mics*, leds*, http_client*, speak*);
		void stop();
		void wait();

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
		speak*			_speak;
		
		static listen*	_instance;
	};

}