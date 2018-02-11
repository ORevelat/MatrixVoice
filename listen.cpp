#include <cassert>
#include <csignal>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <iostream>

#include <matrix_hal/wishbone_bus.h>

#include "listen_leds.h"
#include "listen_mics.h"
#include "listen_httpsend.h"

#define SILENCE_COUNT	3 * WINDOW_SIZE / 256
// 10 sec maximum
#define MAX_COUNT		10000 / (WINDOW_SIZE / 256)

#include "snowboy_wrapper.h"

namespace hal = matrix_hal;

bool exitRequested = false;

void SignalHandler(int)
{
  exitRequested = true;
}

int main(int, const char**)
{
	// Configures signal handling for ctrl+c
	struct sigaction sig_int_handler;
	sig_int_handler.sa_handler = SignalHandler;
	sigemptyset(&sig_int_handler.sa_mask);
	sig_int_handler.sa_flags = 0;
	sigaction(SIGINT, &sig_int_handler, NULL);

	std::cout << "Initializing ..." << std::endl;

	hal::WishboneBus bus;
	bus.SpiInit();

	// initialize mics & leds
	ListenLeds leds(bus);
	ListenMics mics(bus);

	ListenHTTPSend http("192.168.254.220", 1880);

	// Initializes Snowboy detector.
	Snowboy::Model model;
	model.filename = "resources/sarah.pmdl";
	model.sensitivity = 0.38;

	Snowboy detector("resources/common.res", model, 1.0);

	std::cout << "Running ..." << std::endl;

	int64_t avg_for_hotword = 0;
	uint16_t tick_after_hotword = 0;
	uint16_t total_tick_after_hotword = 0;

	// up to 10 seconds of sound
	int16_t	record_buffer[10000 * (WINDOW_SIZE / 256)];
	size_t	record_len = 0;

	while (!exitRequested) {
		int64_t wnd_avg = mics.Read();

		int result = detector.RunDetection(mics.Last(), NUMBER_SAMPLE);
		if (result > 0) {
			avg_for_hotword = wnd_avg;
			tick_after_hotword = 0;
			total_tick_after_hotword = 0;
			record_len = 0;

			leds.On(ListenLeds::red, 500);
		}

	
		if (avg_for_hotword > 0) {
			// copy to keep raw buffer
			std::memcpy(record_buffer + record_len, mics.Last(), NUMBER_SAMPLE * sizeof(int16_t));
			record_len += NUMBER_SAMPLE;

			total_tick_after_hotword++;
		 	if (wnd_avg < avg_for_hotword)
		 		tick_after_hotword ++;
		 	else
		 		tick_after_hotword = 0;
		}

		if ((tick_after_hotword > SILENCE_COUNT) || (total_tick_after_hotword > MAX_COUNT))
		{
		 	avg_for_hotword = 0;
		 	tick_after_hotword = 0;
			total_tick_after_hotword = 0;

			leds.On(ListenLeds::green);

			http.Send(record_buffer, record_len);
		}
	}

	std::cout << "Cleaning ..." << std::endl;

	bus.SpiClose();

	std::cout << "Done" << std::endl;
	return 0;
}
