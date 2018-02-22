#include <thread>

#include "main.h"
#include "speaker.h"

namespace sarah_matrix
{

	speaker::speaker(event_notifier& n)
		: _notif(n), _alsaopt("")
	{
		_notif.function_register(event_notifier::INITIALISE, std::bind(&speaker::initialise, this));
		_notif.function_register(event_notifier::DEINITIALISE, std::bind(&speaker::deinitialise, this));
		_notif.function_register(event_notifier::HOTWORD_DETECTED, 
			[&] (void*) { 
				std::thread t(&speaker::play, this, std::string("ding.wav")); 
				t.detach(); 
		});
		_notif.function_register(event_notifier::SPEAK_RECEIVED, 
			[&] (void* p) { 
				std::string s( *(reinterpret_cast<std::string*>(p)) );
				std::thread t(&speaker::speak, this, s); 
				t.detach();
		});
	}
	
	void speaker::initialise()
	{
		LOG(INFO) << "speaker initialising";

		_alsaopt = FLAGS_alsaout;
		if (!_alsaopt.empty())
			LOG(INFO) << " == using alsa option " << _alsaopt;

		LOG(INFO) << "speaker initialised";
	}

	void speaker::deinitialise()
	{
		LOG(INFO) << "speaker deinitialising";
		
		LOG(INFO) << "speaker deinitialised";
	}

	void speaker::speak(std::string text)
	{
		_notif.notify(event_notifier::SPEAK_START);

	   	LOG(INFO) << " == speak - " << text;

		std::string command("pico2wave -l fr-FR -w /tmp/test.wav ");
		command += "\"" + text + "\"";
		system(command.c_str());
		
		std::string aplay = "aplay -q " + _alsaopt + " /tmp/test.wav";
		system(aplay.c_str());

		system("rm /tmp/test.wav");

		_notif.notify(event_notifier::SPEAK_END);
	}

	void speaker::play(std::string file)
	{
	   	LOG(INFO) << " == sound - " << file;

		std::string aplay = "aplay -q " + _alsaopt + " ./resources/" + file;
		system(aplay.c_str());
	}

}
