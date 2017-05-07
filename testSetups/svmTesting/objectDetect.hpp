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
#include <unistd.h>

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

// Defines:

#define HEAD	1
#define DORSAL	2
#define COIDAL	3

/*Functions*/

vector<Mat> constructImagePyramid(Mat src, Size windowSize, double scaling, bool gaussianBlur);
vector<vector<Rect>> slidingWindowDetection(vector<Mat> imagePyramid, Size window, double scaling, int stride, bool nonMaxSuppression, struct svm_model* SVMModel);
int svmPredict(vector<double> featureVector, struct svm_model* SVMModel);
vector<vector<Rect>> nonMaxSuppression(vector<vector<Rect>> detections, double overlapThresh);
double overlapCalc(Rect a, Rect b);
Rect meanWindowCalc(vector<Rect> cluster);
vector<vector<Rect>> multiScaleDetection(Mat src, Size windowSize, double scaling, int stride, bool gaussianBlur, bool nonMaxSuppression, struct svm_model* SVMModel);
void svmDetect(ctx* context, Size windowSize, double scaling, int stride, bool gaussianBlur, bool nonMaxSuppression);

#endif /*!OBJECTDETECT_H*/
