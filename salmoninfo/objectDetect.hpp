#ifndef OBJECTDETECT_H
#define OBJECTDETECT_H

/*System Headers*/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <stdio.h>
#include <thread>
#include <string.h>
#include <vector>

/*OpenCV Object Detection Headers*/

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

/*LibLBP Descriptor Headers*/

#include "./LBP/LBP.hpp"

/*Namespaces*/

using namespace cv;
using namespace cv::xfeatures2d;
using namespace cuda;
using namespace std;
using namespace lbp;

/*Functions*/

vector<double> extractLBPFeatures(int rad, int pts, string mapping, string filename, bool normalizeHist);


#endif /*!OBJECTDETECT_H*/