#pragma once

#include <iostream>
#include <cstring>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "event-notifier.h"
#include "matrix_mics.h"

#include "snowboy_wrapper.h"

#define RECORD_MAX_DURATION_MS	10000
#define SPEECH_SILENCE_MS		1024

#define SPEECH_SILENCE_COUNT		(SPEECH_SILENCE_MS * SAMPLING_RATE) / (1000 * NUMBER_SAMPLE)
#define MAX_SPEECH_RECORD_COUNT		(RECORD_MAX_DURATION_MS * SAMPLING_RATE) / (1000 * NUMBER_SAMPLE)

namespace sarah_matrix
{

	struct audio_buffer
	{
		uint64_t _len;
		int16_t	_buffer[SAMPLING_RATE * RECORD_MAX_DURATION_MS / 1000];
	};

	struct record_state
	{
		record_state()
		{
			recording = false;
			average_energy = 0;
			tick_after_hotword = 0;
			total_tick_after_hotword = 0;
			record_len = 0;

			std::memset(record_buffer, 0, sizeof(record_buffer));
		}

		void start_record(int64_t avg)
		{
			recording = true;
			average_energy = avg;
			tick_after_hotword = 0;
			total_tick_after_hotword = 0;
			record_len = 0;	
		}

		void reset()
		{
			recording = false;
			average_energy = 0;
			tick_after_hotword = 0;
			total_tick_after_hotword = 0;
			record_len = 0;		
		}

		void copy_to_buffer(const int16_t* new_samples, uint64_t nb_samples)
		{
			if ((record_len + nb_samples) >= sizeof(record_buffer))
				return;

			std::memcpy(record_buffer + record_len, new_samples, nb_samples * sizeof(int16_t));
			record_len += nb_samples;
		}

		bool is_recording()
		{
			return recording;
		}

		void increment_ticks(int64_t avg)
		{
			total_tick_after_hotword++;
			if (avg <= average_energy)
				tick_after_hotword ++;
			else
				tick_after_hotword = 0;
		}

		bool speech_ended()
		{
			if ( (tick_after_hotword >= SPEECH_SILENCE_COUNT) || (total_tick_after_hotword >= MAX_SPEECH_RECORD_COUNT))
				return true;
			return false;
		}

		audio_buffer* infobuffer()
		{
			auto x = new audio_buffer();
			x->_len = record_len;
			std::memcpy(x->_buffer, record_buffer, record_len * sizeof(int16_t));

			return x;
		}

		private:
		bool	recording;
		int64_t average_energy;
		uint16_t tick_after_hotword;
		uint16_t total_tick_after_hotword;
		uint64_t record_len;

		int16_t	record_buffer[SAMPLING_RATE * RECORD_MAX_DURATION_MS / 1000];
	};

	class recorder
	{
		public:
		recorder(event_notifier&, microphones&);

		private:
		void initialise();
		void deinitialise();

		void main_loop();

		void speak_started();
		void speak_stopped();

		private:
		event_notifier& _notif;
		microphones& _mics;

		bool _exit;
		bool _isplaying;

		std::unique_ptr<Snowboy>		_detector;
		std::unique_ptr<record_state>	_state;

		std::mutex _exit_mtx;
		std::condition_variable _exit_cvar;
	};

}