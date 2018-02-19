
#include "matrix_mics.h"

namespace sarah_matrix
{

	microphones::microphones(matrix_hal::WishboneBus& bus)
	{
		_mics.Setup(&bus);

		_mics.SetSamplingRate(SAMPLING_RATE);
		_mics.SetGain(AUDIO_GAIN);

		_buffer.resize(WINDOW_SIZE);
	}

	// read microphone beam & store to circular buffer
	void microphones::read()
	{
		_mics.Read();

		for (uint32_t s = 0; s < NUMBER_SAMPLE; s++) {
			_buffer.pushpop(_mics.Beam(s));
		}
	}

	// average energy of the sample stored into circular buffer
	int64_t microphones::average_energy() const
	{
		return _buffer.average_energy();
	}

	// return last read buffer
	const int16_t* microphones::last() const
	{
		return _buffer.lastsamples(NUMBER_SAMPLE);
	}

}
