#pragma once

#include <matrix_hal/wishbone_bus.h>
#include <matrix_hal/microphone_array.h>

#include "circularbuffer.h"

#define SAMPLING_RATE	16000
#define NUMBER_SAMPLE	256
#define WINDOW_SIZE		4096
#define AUDIO_GAIN		1

namespace sarah_matrix
{

	class mics
	{
	public:
		mics(matrix_hal::WishboneBus&);

		int64_t read();
		
		const int16_t* last() const;

	private:
		matrix_hal::MicrophoneArray	_mics;
		circularbuffer				_buffer;
	};

}
