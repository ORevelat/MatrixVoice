#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include <curl/curl.h>

#include "cencode.h"

class ListenHTTPSend
{
	public:
	ListenHTTPSend(const std::string& url, const uint16_t& port)
		: _url(url), _port(port)
	{
	}

	bool Send(int16_t* buffer, size_t len)
	{
		curl_global_init(CURL_GLOBAL_ALL);
		
		CURL* curl = curl_easy_init();
		if (!curl) {
			std::cerr << "unable to initialize curl" << std::endl;
			return false;
		}

		std::ostringstream url;
		url << "http://" << _url << ":" << _port << "/sarah/listen";

		std::string payload = build_postfield(buffer, len);
		curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed: " <<  curl_easy_strerror(res) << std::endl;
			return false;
		}
		
		curl_easy_cleanup(curl);
		curl_global_cleanup();

		return true;
	}

	private:
	static inline size_t write_data(void *, size_t size, size_t nmemb, void *) { return size * nmemb; }

	std::string build_postfield(int16_t const* buffer, size_t len)
	{
		std::ostringstream postdata;

		std::vector<char> output(4 * len * sizeof(uint16_t));

		base64_encodestate s;
		base64_init_encodestate(&s);

		int encode_len = base64_encode_block((char const*)&buffer[0], len * sizeof(uint16_t), output.data(), &s);
		encode_len += base64_encode_blockend(output.data() + encode_len, &s);

		postdata << "len=" << encode_len << "&";
		postdata << "buffer=";
		for(int i=0; i<encode_len; i++)
			postdata << output[i];

		//std::cerr << postdata.str() << std::endl;

		return postdata.str();
	}

	private:
	std::string	_url;
	uint16_t	_port;
};
