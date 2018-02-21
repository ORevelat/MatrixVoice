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
		_notif.function_register(event_notifier::HOTWORD_DETECTED, [&] (void*) { std::thread t(&speaker::play, this, (void*)"ding.wav"); t.detach(); });
		_notif.function_register(event_notifier::SPEAK_RECEIVED, [&] (void* p) { std::thread t(&speaker::speak, this, p); t.detach(); });
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

	void speaker::speak(void* param)
	{
		_notif.notify(event_notifier::SPEAK_START);

		std::string text(*(reinterpret_cast<std::string*>(param)));

	   	LOG(INFO) << " == speak - " << text;

		std::string command("pico2wave -l fr-FR -w /tmp/test.wav ");
		command += "\"" + text + "\"";
		system(command.c_str());
		
		std::string aplay = "aplay -q " + _alsaopt + " /tmp/test.wav";
		system(aplay.c_str());

		system("rm /tmp/test.wav");

		_notif.notify(event_notifier::SPEAK_END);
	}

	void speaker::play(void* file)
	{
	   	LOG(INFO) << " == sound - " << (char*)file;

		std::string aplay = "aplay -q " + _alsaopt + " ./resources/" + std::string(reinterpret_cast<char*>(file));
		system(aplay.c_str());
	}

}
