#ifndef KMLICONREQUEST_H
#define KMLICONREQUEST_H

#include <string>

class QImage;

class KmlIconRequest
{

public:
	KmlIconRequest();
	~KmlIconRequest();

	QImage requestImage(const std::string & url);


	
};

#endif //KMLICONREQUEST_H