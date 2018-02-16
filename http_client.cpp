
#include <cstring>

#include <curl/curl.h>
#include <glog/logging.h>

#include "http_client.h"

#include "include/base64.h"

namespace sarah_matrix
{

	struct WriteThis
	{
		const char *readptr;
		size_t sizeleft;
	};
	
	http_client::http_client(const std::string& url, const uint16_t& port)
		: _url(url), _port(port)
	{}

	bool http_client::Send(int16_t* buffer, size_t len)
	{
		curl_global_init(CURL_GLOBAL_ALL);
		
		CURL* curl = curl_easy_init();
		if (!curl) {
			LOG(ERROR) << "CURL - unable to initialize the library";
			return false;
		}

		std::ostringstream url;
		url << "http://" << _url << ":" << _port << "/sarah/listen";

		std::string encoded = base64_encode((char unsigned const*)&buffer[0], len * sizeof(int16_t));
		encoded = "buffer=" + std::string(curl_easy_escape(curl , encoded.data(), encoded.size()));

		struct WriteThis wt;

		wt.readptr = &encoded[0];
		wt.sizeleft = encoded.size();

		LOG(INFO) << "== sending " << wt.sizeleft << " bytes to " << url.str();
		
		curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_READDATA, &wt);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)wt.sizeleft);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			LOG(ERROR) << "curl_easy_perform() failed: " <<  curl_easy_strerror(res);
			return false;
		}
		
		curl_easy_cleanup(curl);
		curl_global_cleanup();

		return true;
	}

	size_t http_client::write_data(void *, size_t size, size_t nmemb, void *)
	{
		return size * nmemb;
	}

	size_t http_client::read_callback(void *dest, size_t size, size_t nmemb, void *userp)
	{
		struct WriteThis *wt = (struct WriteThis *)userp;
		size_t buffer_size = size*nmemb;

		if (wt->sizeleft) {
			size_t copy_this_much = wt->sizeleft;
			if(copy_this_much > buffer_size)
				copy_this_much = buffer_size;
			std::memcpy(dest, wt->readptr, copy_this_much);

			wt->readptr += copy_this_much;
			wt->sizeleft -= copy_this_much;
			return copy_this_much;
		}

		return 0;
	}

}