
#include "matrix_mics.h"
#include "matrix_mics.bandpass_coeff.h"

#define WINDOW_SIZE_MS		1000

namespace sarah_matrix
{

	microphones::microphones(matrix_hal::MatrixIOBus& bus, uint16_t rate, int16_t gain)
	{
		_mics.Setup(&bus);

		_mics.SetSamplingRate(rate);
		if (gain > 0)
			_mics.SetGain(gain);

		_samplerate = _mics.SamplingRate();
		_gain = _mics.Gain();
		_numbersample = _mics.NumberOfSamples();

		_miccore.reset(new matrix_hal::MicrophoneCore(_mics));
		_miccore.get()->Setup(&bus);
		_miccore.get()->SelectFIRCoeff(&FIR_bandpass[0]);

		_mics.CalculateDelays(0, 0, 100.0, 320 * 1000.0);

		_buffer.resize(WINDOW_SIZE_MS * (_samplerate / 1000));
	}

	// read microphone beam & store to circular buffer
	void microphones::read()
	{
		_mics.Read();

		for (uint32_t s = 0; s < _numbersample; s++) {
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
		return _buffer.lastsamples(_numbersample);
	}

}
