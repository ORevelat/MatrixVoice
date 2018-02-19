#include <cassert>
#include <csignal>
#include <unistd.h>
#include <iostream>


#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>
#include <glog/logging.h>

#include <matrix_hal/wishbone_bus.h>

#include "main.h"

#include "event-notifier.h"
#include "matrix_mics.h"


DEFINE_string(ip, "0.0.0.0", "What ip to bind on");
DEFINE_int32(port, 1234, "What port to listen on");

DEFINE_string(remote_ip, "0.0.0.0", "What ip to use to send recorded audio");
DEFINE_int32(remote_port, 1880, "What port to use to send recorded audio");

DEFINE_string(alsaout, "", "device to use for playing");

using namespace sarah_matrix;

void SignalHandler(int)
{
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

	LOG(INFO) << "Initialise done";

	LOG(INFO) << "Deinitialising ...";

    gflags::ShutDownCommandLineFlags();

	LOG(INFO) << "Exit";
    return 0;
}
