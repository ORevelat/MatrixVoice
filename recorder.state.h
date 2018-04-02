#pragma once

#include "main.h"

#include <memory>

#define RECORD_MAX_DURATION_MS	10000
#define SPEECH_SILENCE_MS		1000

namespace sarah_matrix
{

	struct audio_buffer
	{
		audio_buffer(uint64_t size)
		{
			_len = size;
			_buffer.reset(new int16_t[size]);
		}

		uint64_t Length() const { return _len; }
		int16_t* Buffer() const { return _buffer.get(); }

	private:
		uint64_t _len;
		std::unique_ptr<int16_t> _buffer;
	};

	struct record_state
	{
		record_state(uint16_t max_duration_sec, uint16_t nb_sample, uint16_t samplerate = 16000)
		{
			recording = false;
			average_energy = 0;
			tick_after_hotword = 0;
			total_tick_after_hotword = 0;
			record_len = 0;

			count_speech_silence = (SPEECH_SILENCE_MS * samplerate) / (1000 * nb_sample);
			count_speech_record_max = (RECORD_MAX_DURATION_MS * samplerate) / (1000 * nb_sample);

			buffer_len = max_duration_sec * samplerate;
			buffer.reset(new int16_t[buffer_len]);

			std::memset(buffer.get(), 0, buffer_len);

			LOG(INFO) << " == record_state max_duration_sec=" << max_duration_sec << " , count_speech_silence=" << count_speech_silence << " , count_speech_record_max=" << count_speech_record_max << " , buffer_len=" << buffer_len;
		}

		~record_state()
		{
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
			if ((record_len + nb_samples) >= buffer_len)
				return;

			std::memcpy(buffer.get() + record_len, new_samples, nb_samples * sizeof(int16_t));
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
			if ( (tick_after_hotword >= count_speech_silence) || (total_tick_after_hotword >= count_speech_record_max))
				return true;
			return false;
		}

		uint64_t buffer_length() const { return buffer_len; }

		audio_buffer* infobuffer()
		{
			auto x = new audio_buffer(record_len);
			std::memcpy(x->Buffer(), buffer.get(), record_len * sizeof(int16_t));

			return x;
		}

	private:
		bool	recording;
		int64_t average_energy;
		uint16_t tick_after_hotword;
		uint16_t total_tick_after_hotword;
		uint64_t record_len;

		uint64_t	count_speech_silence;
		uint64_t	count_speech_record_max;

		std::unique_ptr<int16_t> buffer;
		uint64_t buffer_len;
	};

}