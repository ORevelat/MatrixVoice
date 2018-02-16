#pragma once

#include <string>

namespace sarah_matrix
{

	class http_client
	{
	public:
		http_client(const std::string&, const uint16_t&);

		bool Send(int16_t*, size_t);

	private:
		static inline size_t write_data(void *, size_t, size_t, void *);
		static inline size_t read_callback(void *, size_t, size_t, void *);

	private:
		std::string	_url;
		uint16_t	_port;
	};

}