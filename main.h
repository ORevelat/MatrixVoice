#pragma once

#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>
#include <glog/logging.h>

DECLARE_string(local_ip);
DECLARE_int32(local_port);

DECLARE_string(remote_url);

DECLARE_string(alsaout);

DECLARE_string(hotword_sensitivity);
DECLARE_string(hotword_model);
DECLARE_bool(frontend_algo);

DECLARE_int32(audio_samplerate);
DECLARE_int32(audio_gain);

DECLARE_bool(saverecord);
