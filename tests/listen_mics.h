#pragma once

#include <vector>

#include <matrix_hal/wishbone_bus.h>
#include <matrix_hal/microphone_array.h>

#include "listen_circularbuffer.h"

#define SAMPLING_RATE	16000
#define NUMBER_SAMPLE	256
#define WINDOW_SIZE		4096
#define AUDIO_GAIN		1

class ListenMics
{
	public:
	ListenMics(matrix_hal::WishboneBus& bus, uint32_t rate = SAMPLING_RATE, uint16_t gain = AUDIO_GAIN)
	{
		mics.Setup(&bus);

		mics.SetSamplingRate(rate);
		mics.SetGain(gain);

		buffer.wnd_resize(WINDOW_SIZE);
	}

	// read microphone beam 
	// store to circular buffer
	// and return average energy of the whole circular buffer
	int64_t Read()
	{
		mics.Read();
		for (uint32_t s = 0; s < NUMBER_SAMPLE; s++) {
			buffer.wnd_add(mics.Beam(s));
		}

		return buffer.wnd_average();
	}

	// return last read buffer
	const int16_t* Last() const
	{
		return buffer.wnd_lastsamples(NUMBER_SAMPLE);
	}

	private:
	matrix_hal::MicrophoneArray	mics;
	CircularBuffer				buffer;
};
