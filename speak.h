#pragma once

#include <string>


namespace sarah_matrix
{
	class leds;

	class speak
	{
	public:
		speak(const std::string&, leds*);

		void tts(const std::string&);
		void sound(const std::string& w = "ding.wav");

		bool is_speaking() const { return _isSpeaking; }

	private:
		void thread_tts(std::string);
		void thread_sound(std::string);

		void thread_leds(bool*);

	private:
		leds* 	_leds;

		std::string _alsaout;

		bool	_isSpeaking;
	};

}
