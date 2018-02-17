#include <cassert>
#include <csignal>
#include <unistd.h>
#include <iostream>
#include <functional>

#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>
#include <glog/logging.h>

#include <matrix_hal/wishbone_bus.h>

#include "main.h"

#include "http_server.h"
#include "http_client.h"
#include "matrix_leds.h"
#include "matrix_mics.h"
#include "speak.h"
#include "listen.h"

DEFINE_string(ip, "0.0.0.0", "What ip to bind on");
DEFINE_int32(port, 1234, "What port to listen on");

DEFINE_string(remote_ip, "0.0.0.0", "What ip to use to send recorded audio");
DEFINE_int32(remote_port, 1880, "What port to use to send recorded audio");

using namespace sarah_matrix;

void SignalHandler(int)
{
	http_server* server = http_server::getInstance();
	if (server)
		server->stop();

	listen* ls = listen::getInstance();
	if (ls)
		ls->stop();
}

int main(int argc, char** argv)
{
    FLAGS_logtostderr = 1; 

    gflags::SetUsageMessage("Listen and speak module for Sarah using Matrix Voice & Raspberry");
    gflags::SetVersionString("0.2.0");

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

	leds voiceLeds(bus);
	mics voiceMics(bus);

	listen* ls = listen::getInstance();
	speak sp(&voiceLeds, ls);
	http_server* server = http_server::getInstance();
	http_client client(FLAGS_remote_ip, FLAGS_remote_port);

	LOG(INFO) << "Initialise done";

	server->start(FLAGS_ip, FLAGS_port, std::bind(&speak::run, &sp, std::placeholders::_1));
	ls->start(&voiceMics, &voiceLeds, &client);

	server->wait();
	ls->wait();

	LOG(INFO) << "Deinitialising ...";

	http_server::delInstance();
	listen::delInstance();

    gflags::ShutDownCommandLineFlags();

	LOG(INFO) << "Exit";
    return 0;
}
