#undef _GLIBCXX_USE_CXX11_ABI
#define _GLIBCXX_USE_CXX11_ABI 0
#include "snowboy_wrapper.h"

#include "include/snowboy-detect.h"

static snowboy::SnowboyDetect* c(void* p) { return reinterpret_cast<snowboy::SnowboyDetect*>(p); }

Snowboy::Snowboy(const char* resource_name, const char* model_name)
		: detector_(new snowboy::SnowboyDetect(resource_name, model_name)) {
}

void Snowboy::SetSensitivity(const char* sensivity)
{
	c(detector_)->SetSensitivity(sensivity);
}

void Snowboy::SetAudioGain(float audio_gain)
{
	c(detector_)->SetAudioGain(audio_gain);
}

void Snowboy::ApplyFrontend(bool apply_frontend)
{
	c(detector_)->ApplyFrontend(apply_frontend);
}

int Snowboy::RunDetection(const int16_t* data, int num_samples)
{
	return c(detector_)->RunDetection(data, num_samples);
}
