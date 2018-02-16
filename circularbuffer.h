#pragma once

#include <cmath>
#include <vector>

class circularbuffer
{ 
	public:
	circularbuffer(const uint16_t size = 8192)
	{
		buffer.resize(size, 0);
	}

	public:
	void resize(const uint16_t size)
	{
		buffer.resize(size, 0);
	}
	
	/// store value to our buffer
	void add(const int16_t value) 
	{
		buffer.erase(buffer.begin());
		buffer.push_back(value);
	}

	/// compute average energy of the buffer
	int64_t average() const
	{
		int64_t buffer_sum = 0;
		for(std::vector<int16_t>::const_iterator it = buffer.begin(); it != buffer.end(); ++it) {
			buffer_sum += *it * *it;
		}
		return (int64_t) std::floor(buffer_sum / buffer.size());
	}

	const int16_t* lastsamples(const uint32_t numberofsample) const
	{
		return buffer.data() + buffer.size() - numberofsample;
	}

	private:
	// sliding window buffer
	std::vector<int16_t> buffer;
};
