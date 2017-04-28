#ifndef CASCADEDETECTION_H
#define CASCADEDETECTION_H

/*OpenCV Library Headers*/

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"

#include "opencv2/freetype.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/objdetect.hpp"

#include "opencv2/cudawarping.hpp"
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudacodec.hpp"

/*System Headers*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <thread>
#include <string.h>
#include <mutex>

/*Namespaces*/

using namespace cv;
using namespace cv::xfeatures2d;
using namespace cuda;
using namespace std;

/*Functions*/

void cascadeDetect(std::string video_source, std::string coidal_cascade_name, std::string head_cascade_name, std::string dorsal_cascade_name);


#endif /*!CASCADEDETECTION_H*/
