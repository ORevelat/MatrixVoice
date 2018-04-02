#pragma once

#include <cstdint>

class Snowboy
{
 public:

  Snowboy(const char* resource_name, const char* model_name);

  void SetSensitivity(const char* sensivity);
  void SetAudioGain(float audio_gain);
  void ApplyFrontend(bool apply_frontend);

  int RunDetection(const int16_t* data, int num_samples);

  int SampleRate();
  int NumChannels();
  int BitsPerSample();

 private:
  void* detector_;
};
