#pragma once

#include <cmath>
#include <vector>

class circularbuffer
{ 
	public:
	circularbuffer(const uint16_t size = 8192)
	{
		sliding_wnd.resize(size, 0);
	}

	public:
	void resize(const uint16_t size)
	{
		sliding_wnd.resize(size, 0);
	}
	
	/// store value to our buffer
	void add(const int16_t value) 
	{
		sliding_wnd.erase(sliding_wnd.begin());
		sliding_wnd.push_back(value);
	}

	/// compute average energy of the sliding_wnd
	int64_t average() const
	{
		int64_t wnd_sum = 0;
		for(std::vector<int16_t>::const_iterator it = sliding_wnd.begin(); it != sliding_wnd.end(); ++it) {
			wnd_sum += *it * *it;
		}
		return (int64_t) std::floor(wnd_sum / sliding_wnd.size());
	}

	const int16_t* lastsamples(const uint32_t numberofsample) const
	{
		return sliding_wnd.data() + sliding_wnd.size() - numberofsample;
	}

	private:
	// sliding window buffer
	std::vector<int16_t> sliding_wnd;
};
