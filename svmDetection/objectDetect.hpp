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

#include "lbp.hpp"

/*LibSVM Headers*/

#include "svm.h"

/*Datastructures*/

#include "./types.hpp"

/*Namespaces*/

using namespace cv;
using namespace cv::xfeatures2d;
using namespace cuda;
using namespace std;

/*Functions*/

vector<Mat> constructImagePyramid(Mat src, Mat window, double scaling, bool gaussianBlur);
vector<Rect> multiScaleDetection(Mat src);
int svmDetect(vector<double> featureVector, struct svm_model *SVMModel);
Mat preProcessImage(Mat src, Size outputDims);


#endif /*!OBJECTDETECT_H*/
