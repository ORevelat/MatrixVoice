#pragma once

#include <cmath>
#include <deque>

class CircularBuffer
{ 
	public:
	CircularBuffer(const uint16_t size = 8192)
	{
		sliding_wnd.resize(size, 0);
	}

	public:
	void wnd_resize(const uint16_t size)
	{
		sliding_wnd.resize(size, 0);
	}
	
	/// store value to our buffer
	void wnd_add(const int16_t value) 
	{
		sliding_wnd.erase(sliding_wnd.begin());
		sliding_wnd.push_back(value);
	}

	/// compute average energy of the sliding_wnd
	int64_t wnd_average() const
	{
		int64_t wnd_sum = 0;
		for(std::vector<int16_t>::const_iterator it = sliding_wnd.begin(); it != sliding_wnd.end(); ++it) {
			wnd_sum += *it * *it;
		}
		return (int64_t) std::floor(wnd_sum / sliding_wnd.size());
	}

	const int16_t* wnd_lastsamples(const uint32_t numberofsample) const
	{
		return sliding_wnd.data() + sliding_wnd.size() - numberofsample;
	}

	private:
	// sliding window buffer
	std::vector<int16_t> sliding_wnd;
};
