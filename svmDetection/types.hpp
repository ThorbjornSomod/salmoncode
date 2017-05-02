#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <mutex>

#include "opencv2/core.hpp"

using namespace cv;

struct ctx{

	Mat* image;
	uchar* pixels;
	std::mutex* imagemutex;

};

#endif /*!TYPES_H*/
