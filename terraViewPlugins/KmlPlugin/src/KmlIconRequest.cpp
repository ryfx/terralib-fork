#include <KmlIconRequest.h>

// STL
#include <iostream>

// Curl
#include <curl/curl.h>

// Qt
#include <qimage.h>

#include <stdio.h>

int writer(char* data, size_t size, size_t nmemb, std::string* writerData)
{
    if(writerData == 0)
        return 0;

    writerData->append(data, size * nmemb);
    return static_cast<int>(size * nmemb);
}

KmlIconRequest::KmlIconRequest()
{
	
}

KmlIconRequest::~KmlIconRequest()
{
	
}

QImage KmlIconRequest::requestImage(const std::string & url)
{
	QImage ima;

	CURL* conn = 0;
	char* errorBuffer = new char[CURL_ERROR_SIZE];
	CURLcode code;

    std::string buffer;

	try
	{
		conn = curl_easy_init();

		if(conn == 0)
			return 0;

		code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);

		if(code != CURLE_OK)
			return 0;

		//Setando CURLOPT_USERAGENT
		code = curl_easy_setopt(conn, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8) Gecko/20051111 Firefox/1.5 BAVM/1.0.0");

		if(code != CURLE_OK)
			return 0;
		//Fim

		code = curl_easy_setopt(conn, CURLOPT_TIMEOUT, 4);

		if(code != CURLE_OK)
			return 0;

		code = curl_easy_setopt(conn, CURLOPT_URL, url.c_str());

		if(code != CURLE_OK)
			return 0;

		code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);

		if( code != CURLE_OK )
			return 0;

		code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &buffer);

		if( code != CURLE_OK )
			return 0;
		
		code = curl_easy_perform(conn);

		if(code != CURLE_OK)
            return 0;

		char* ct;
		code = curl_easy_getinfo(conn, CURLINFO_CONTENT_TYPE, &ct);
	
		if(code != CURLE_OK)
			return 0;
	}
	catch(...)
	{
		curl_easy_cleanup(conn);
		delete [] errorBuffer;
		return 0;
	}

	curl_easy_cleanup(conn);
	delete [] errorBuffer;

    ima.loadFromData((const unsigned char*)buffer.c_str(), buffer.size());

	return ima;
}