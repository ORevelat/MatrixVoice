#include <cassert>
#include <csignal>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "main.h"

DEFINE_string(local_ip, "0.0.0.0", "What ip to bind on");
DEFINE_int32(local_port, 1234, "What port to listen on");
DEFINE_string(remote_url, "127.0.0.1:1880/sarah/listen", "What url to use to send recorded audio");
DEFINE_string(alsaout, "", "device to use for playing");
DEFINE_string(hotword_sensitivity, "0.54", "sensitivity to use for the hotword engine");
DEFINE_string(hotword_model, "sarah.pmdl", "model file to use for the hotword engine");
DEFINE_bool(frontend_algo, false, "if hotword engine must use its frontend algorithms");
DEFINE_int32(audio_samplerate, 16000, "Audio sample rate to apply to Matrix voice, default to 16000Hz");
DEFINE_int32(audio_gain, -1, "Audio gain to apply for matrix voice, default to -1 (use default value depending on sample rate)");

#include <matrix_hal/matrixio_bus.h>

#include "event-notifier.h"

#include "recorder.h"
#include "display.h"
#include "http_post.h"
#include "http_server.h"
#include "speaker.h"

using namespace sarah_matrix;

std::mutex exit_mtx;
std::condition_variable exit_cvar;

void SignalHandler(int)
{
	exit_cvar.notify_one();
}

int main(int argc, char** argv)
{
	FLAGS_logtostderr = 1; 

    gflags::SetUsageMessage("Listen and speak module for Sarah using Matrix Voice & Raspberry");
    gflags::SetVersionString("0.5.1");

    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

	LOG(INFO) << "Start";

	// Configures signal handling for ctrl+c
	struct sigaction sig_int_handler;
	sig_int_handler.sa_handler = SignalHandler;
	sigemptyset(&sig_int_handler.sa_mask);
	sig_int_handler.sa_flags = 0;
	sigaction(SIGINT, &sig_int_handler, NULL);

	LOG(INFO) << "Initialising ...";

	// hardware init
	matrix_hal::MatrixIOBus bus;
	if (!bus.Init())
		return -1;

	if(!bus.IsDirectBus())
	{
		LOG(ERROR) << "Kernel Modules has been loaded. Use ALSA implementation";
    	return -1; 
  	}

	microphones mics(bus, FLAGS_audio_samplerate, FLAGS_audio_gain);
	leds led(bus);

	// notifier to send messages between classes
	event_notifier notifier;

	// our high level objects
	recorder rec(notifier, mics);
	display disp(notifier, led);
	http_post post(notifier);
	http_server server(notifier);
	speaker sp(notifier);

	// if there is some subscription for init
	notifier.notify(event_notifier::INITIALISE);

	LOG(INFO) << "Initialise done";

	LOG(INFO) << "Running ...";
	
	notifier.notify(event_notifier::START);

	// and wait for ctrl+c
 	std::unique_lock<std::mutex> lck(exit_mtx);
    exit_cvar.wait(lck);
	
	notifier.notify(event_notifier::STOP);

	LOG(INFO) << "Deinitialising ...";

	notifier.notify(event_notifier::DEINITIALISE);

    gflags::ShutDownCommandLineFlags();

	LOG(INFO) << "Exit";
    return 0;
}
