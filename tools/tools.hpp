#ifndef TOOLS_H
#define TOOLS_H

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

/*System Headers*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <thread>
#include <string.h>

/*Namespaces*/

using namespace cv;
using namespace cv::xfeatures2d;
using namespace cuda;
using namespace std;

/*Functions*/

void resizeUsingOpenCvCpu(std::string input_file, std::string output_file);
void resizeUsingOpenCvGpu(std::string input_file, std::string output_file);
void changeColorSpaceOfImageUsingGpu(std::string input_file, std::string output_file);
void changeColorSpaceOfVideoUsingGpu(std::string input_file, std::string output_file);
void normalizeImageUsingGpu();
void imageConverter();

#endif /*!TOOLS_H*/
