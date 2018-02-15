
#include "matrix_mics.h"

namespace sarah_matrix
{

	mics::mics(matrix_hal::WishboneBus& bus)
	{
		_mics.Setup(&bus);

		_mics.SetSamplingRate(SAMPLING_RATE);
		_mics.SetGain(AUDIO_GAIN);

		_buffer.resize(WINDOW_SIZE);
	}

	int64_t mics::read()
	{
		_mics.Read();

		for (uint32_t s = 0; s < NUMBER_SAMPLE; s++) {
			_buffer.add(_mics.Beam(s));
		}

		return _buffer.average();
	}

	const int16_t* mics::last() const
	{
		return _buffer.lastsamples(NUMBER_SAMPLE);
	}

}
