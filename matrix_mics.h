#pragma once

#include <matrix_hal/wishbone_bus.h>
#include <matrix_hal/microphone_array.h>

#include "circularbuffer.h"

#define SAMPLING_RATE		16000
#define NUMBER_SAMPLE		256
#define WINDOW_SIZE_MS		512
#define AUDIO_GAIN			1

namespace sarah_matrix
{

	class microphones
	{
	public:
		microphones(matrix_hal::WishboneBus&);

		void read();
		
		const int16_t* last() const;
		int64_t average_energy() const;

	private:
		matrix_hal::MicrophoneArray	_mics;
		circularbuffer				_buffer;
	};

}
