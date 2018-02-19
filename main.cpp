#include <cassert>
#include <csignal>
#include <unistd.h>
#include <iostream>

#include "main.h"

DEFINE_string(local_ip, "0.0.0.0", "What ip to bind on");
DEFINE_int32(local_port, 1234, "What port to listen on");
DEFINE_string(remote_ip, "0.0.0.0", "What ip to use to send recorded audio");
DEFINE_int32(remote_port, 1880, "What port to use to send recorded audio");

DEFINE_string(alsaout, "", "device to use for playing");
DEFINE_string(hotword_sensitivity, "0.54", "sensitivity to use for the hotword engine");
DEFINE_string(hotword_model, "sarah.pmdl", "model file to use for the hotword engine");

#include <matrix_hal/wishbone_bus.h>

#include "event-notifier.h"

#include "recorder.h"

using namespace sarah_matrix;

bool exit_requested;

void SignalHandler(int)
{
	exit_requested = true;
}

int main(int argc, char** argv)
{
	FLAGS_logtostderr = 1; 

    gflags::SetUsageMessage("Listen and speak module for Sarah using Matrix Voice & Raspberry");
    gflags::SetVersionString("0.5.0");

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

	matrix_hal::WishboneBus bus;
	bus.SpiInit();

	event_notifier notifier;
	microphones mics(bus);
	recorder rec(notifier, mics);

	// if there is some subscription for init
	notifier.notify(event_notifier::INITIALISE);

	LOG(INFO) << "Initialise done";

	for(;;)
	{
		if (exit_requested)
			break;

		rec.record();
	}

	LOG(INFO) << "Deinitialising ...";

	notifier.notify(event_notifier::DEINITIALISE);

    gflags::ShutDownCommandLineFlags();

	LOG(INFO) << "Exit";
    return 0;
}
