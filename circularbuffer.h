#pragma once

#include <cmath>
#include <vector>

namespace sarah_matrix
{

	class circularbuffer
	{ 
		public:
		circularbuffer(const uint16_t size = 8192)
		{
			resize(size);
		}

		public:
		void resize(const uint16_t size)
		{
			buffer.resize(size, 0);
		}
		
		void pushpop(const int16_t value) 
		{
			buffer.erase(buffer.begin());
			buffer.push_back(value);
		}

		int64_t average_energy() const
		{
			int64_t buffer_sum = 0;
			for(auto it: buffer) {
				buffer_sum += it * it;
			}
			return (int64_t) std::floor(buffer_sum / buffer.size());
		}

		const int16_t* lastsamples(const uint32_t numberofsample) const
		{
			return buffer.data() + buffer.size() - numberofsample;
		}

		private:
		std::vector<int16_t> buffer;
	};

}
