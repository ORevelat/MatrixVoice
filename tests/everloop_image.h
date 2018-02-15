
#ifndef CPP_DRIVER_EVERLOOP_IMAGE_H_
#define CPP_DRIVER_EVERLOOP_IMAGE_H_

#include <cstdint>
#include <vector>

/* from voice driver but fix number of leds */

namespace matrix_hal {

class LedValue 
{
public:
	LedValue() {
		red = 0;
		green = 0;
		blue = 0;
		white = 0;
	}

	uint32_t red;
	uint32_t green;
	uint32_t blue;
	uint32_t white;
};

const int kMatrixVoiceNLeds = 18;

class EverloopImage {
public:
	EverloopImage(int nleds = kMatrixVoiceNLeds) { leds.resize(nleds); }
	std::vector<LedValue> leds;
};

};      // namespace matrix_hal
#endif  // CPP_DRIVER_EVERLOOP_IMAGE_H_