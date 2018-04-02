#include <functional>
#include <thread>

#include <curl/curl.h>

#include "main.h"
#include "http_post.h"

#include "recorder.h"
#include "include/base64.h"

namespace sarah_matrix
{

	struct WriteThis
	{
		const char *readptr;
		size_t sizeleft;
	};

	http_post::http_post(event_notifier& n)
		: _notif(n)
	{
		_notif.function_register(event_notifier::INITIALISE, std::bind(&http_post::initialise, this));
		_notif.function_register(event_notifier::DEINITIALISE, std::bind(&http_post::deinitialise, this));
		_notif.function_register(event_notifier::RECORD_END, 
			[&] (void* param) { 
				std::thread t(&http_post::post, this, param); 
				t.detach();
		});
	}

	void http_post::initialise()
	{
		LOG(INFO) << "http_post initialising";

		_url = FLAGS_remote_url;
		LOG(INFO) << " == using remote url=" << _url;

		LOG(INFO) << "http_post initialised";
	}

	void http_post::deinitialise()
	{
		LOG(INFO) << "http_post deinitialising";

		LOG(INFO) << "http_post deinitialised";
	}
	
	void http_post::post(void* st)
	{
		std::unique_ptr<audio_buffer> buffer;

		// get ownership to release memory
		buffer.reset( reinterpret_cast<audio_buffer*>(st) );

		curl_global_init(CURL_GLOBAL_ALL);
		
		CURL* curl = curl_easy_init();
		if (!curl) {
			LOG(ERROR) << "CURL - unable to initialize the library";
			return;
		}

		std::ostringstream url;
		url << "http://" << _url;

		std::string encoded = base64_encode((char unsigned const*) buffer.get()->Buffer(),  buffer.get()->Length() * sizeof(int16_t));
		encoded = "buffer=" + std::string(curl_easy_escape(curl , encoded.data(), encoded.size()));

		struct WriteThis wt;

		wt.readptr = &encoded[0];
		wt.sizeleft = encoded.size();

		LOG(INFO) << " == send started - " << wt.sizeleft<< " bytes to " << url.str();

		curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_READDATA, &wt);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)wt.sizeleft);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			LOG(ERROR) << "curl_easy_perform() failed: " <<  curl_easy_strerror(res);
			return;
		}
		
		LOG(INFO) << " == send done";

		curl_easy_cleanup(curl);
		curl_global_cleanup();
	}

	size_t http_post::write_data(void *, size_t size, size_t nmemb, void *)
	{
		return size * nmemb;
	}

	size_t http_post::read_callback(void *dest, size_t size, size_t nmemb, void *userp)
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