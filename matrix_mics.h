#pragma once

#include <matrix_hal/matrixio_bus.h>
#include <matrix_hal/microphone_array.h>
#include <matrix_hal/microphone_core.h>

#include "circularbuffer.h"

namespace sarah_matrix
{

	class microphones
	{
	public:
		microphones(matrix_hal::MatrixIOBus&, uint16_t rate = 16000, int16_t gain = -1);

		void read();
		
		const int16_t* last() const;
		int64_t average_energy() const;

		uint16_t SampleRate() const { return _samplerate; }
		uint16_t NumberSample() const { return _numbersample; }

	private:
		matrix_hal::MicrophoneArray	_mics;
		std::unique_ptr<matrix_hal::MicrophoneCore>	_miccore;
		circularbuffer				_buffer;

		uint16_t	_samplerate;
		uint16_t	_gain;
		uint16_t	_numbersample;
	};

}
