#pragma once

#include <string>
#include <thread>

namespace sarah_matrix
{

	class listen
	{
 	public:
		static listen* getInstance();
		static void delInstance();

		void start(void*, void*);
		void stop();
		void wait();

   private:
		listen();
		~listen();

		void run(void*, void*);
	
	private:
		bool			    _exit;
		std::thread*	    _thread;
		
		static listen* _instance;
	};

}