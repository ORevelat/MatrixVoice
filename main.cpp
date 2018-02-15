#include <cassert>
#include <csignal>
#include <unistd.h>
#include <iostream>
#include <functional>

#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "http_server.h"
#include "speak.h"

using namespace sarah_matrix;

DEFINE_string(ip, "0.0.0.0", "What ip to bind on");
DEFINE_int32(port, 1234, "What port to listen on");

void SignalHandler(int)
{
	http_server* server = http_server::getInstance();
	if (server)
		server->stop();
}

int main(int argc, char** argv)
{
    FLAGS_logtostderr = 1; 

    gflags::SetUsageMessage("Listen and speak module for Sarah using Matrix Voice & Raspberry");
    gflags::SetVersionString("0.2.0");

    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);

	// Configures signal handling for ctrl+c
	struct sigaction sig_int_handler;
	sig_int_handler.sa_handler = SignalHandler;
	sigemptyset(&sig_int_handler.sa_mask);
	sig_int_handler.sa_flags = 0;
	sigaction(SIGINT, &sig_int_handler, NULL);

	sarah_matrix::speak sp;

	http_server* server = http_server::getInstance();
	server->start(FLAGS_ip, FLAGS_port, std::bind(&sarah_matrix::speak::run, &sp, std::placeholders::_1));
	server->wait();

	http_server::delInstance();

    gflags::ShutDownCommandLineFlags();
    return 0;
}
