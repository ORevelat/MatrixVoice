#include <cassert>
#include <csignal>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <thread>
#include <cmath>
#include <deque>

#include <matrix_hal/wishbone_bus.h>

#include "mics.h"
#include "leds.h"

#include "snowboy_wrapper.h"

namespace hal = matrix_hal;

bool exitRequested = false;

void SignalHandler(int)
{
  exitRequested = true;
}

void SpeechStarted(Leds* leds)
{
	hal::LedValue x;
	x.red = 50;

	leds->Set(x);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	leds->Reset();
}

void SpeechStopped(Leds* leds)
{
	hal::LedValue x;
	x.green = 50;

	leds->Set(x);
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	leds->Reset();
}

int64_t wnd_average(std::deque<int16_t>& values)
{
	int64_t wnd_sum = 0;
	for(std::deque<int16_t>::const_iterator it = values.begin(); it != values.end(); ++it) {
		wnd_sum += *it * *it;
	}
	return (int64_t) std::floor(wnd_sum / values.size());
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

	Leds leds(bus);
	leds.Reset();

	const int sampling_rate = 16000;

	Mics mics(bus);
	mics.Object().SetSamplingRate(sampling_rate);
	mics.Object().SetGain(1);

	// buffer can hold 1s of sound + 1 frame
	int16_t buffer[mics.Object().NumberOfSamples()];

	// Initializes Snowboy detector.
	Snowboy::Model model;
	model.filename = "resources/sarah.pmdl";
	model.sensitivity = 0.38;

	Snowboy detector("resources/common.res", model, 1.0);

	std::cout << "Running ..." << std::endl;

	// sliding window that holds 256 * 16 ms 
	std::deque<int16_t> sliding_wnd(4096, 0);

	int64_t avg_for_hotword = 0;
	uint16_t tick_after_hotword = 0;

	while (!exitRequested) {
		mics.Object().Read();

		for (uint32_t s = 0; s < mics.Object().NumberOfSamples(); s++) {
			int16_t value = mics.Object().Beam(s);
			buffer[s] = value;
			
			sliding_wnd.push_back(value);
			sliding_wnd.pop_front();
		}

		int64_t wnd_avg = wnd_average(sliding_wnd);

		int result = detector.RunDetection(buffer, mics.Object().NumberOfSamples());
		if (result > 0) {
			// std::cout << "start of speech" << std::endl;
			avg_for_hotword = wnd_avg;
			tick_after_hotword = 0;

			std::thread t(SpeechStarted, &leds);
			t.detach();
		}
		else if (result == 0) {
			// std::cout << "voice" << std::endl;
		}
		else if (result == -2) {
			// std::cout << "silence" << std::endl;
		}

		if (avg_for_hotword > 0) {
		 	if (wnd_avg < avg_for_hotword)
		 		tick_after_hotword ++;
		 	else
		 		tick_after_hotword = 0;
		}

		if (tick_after_hotword > 64) {
		 	//std::cout << "end of speech" << std::endl;

		 	avg_for_hotword = 0;
		 	tick_after_hotword = 0;

			std::thread t(SpeechStopped, &leds);
			t.detach();
		}
	}

	std::cout << "Cleaning ..." << std::endl;

	bus.SpiClose();

	std::cout << "Done" << std::endl;
	return 0;
}
