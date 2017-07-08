#ifndef OBJECTDETECT_H
#define OBJECTDETECT_H

/*System headers*/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <stdio.h>
#include <mutex>
#include <algorithm>
#include <vector>
#include <thread>
#include <string.h>
#include <unistd.h>

/*OpenCV headers*/

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/objdetect.hpp"


/*OpenCV contribution package headers*/

#include "opencv2/ximgproc.hpp"
#include "opencv2/xfeatures2d.hpp"

/*LBP library header*/

#include "lbp.hpp"

/*LIBLINEAR header*/

#include "linear.h"

/*Datastructures header*/

#include "./types.hpp"

/*Namespaces*/

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ximgproc;
using namespace lbp;

/*Defines*/

#define HEAD	1
#define DORSAL	2
#define COIDAL	3

/*Functions*/

double overlapCalc(Rect a, Rect b);
Rect meanWindowCalc(vector<Rect> cluster);
Rect boundingWindowCalc(vector<Rect> cluster);
vector<Rect> deleteVectorElems(vector<Rect> regionProposals, vector<Rect> cluster);

vector<Rect> mergeRegions(vector<Rect> regionProposals);
vector<Rect> edgeboxFiltering(Mat src);

vector<vector<Mat>> constructRegionPyramids(Mat src, vector<Rect> regionProposals, Size windowSize, double scaleFactor);

int svmPredict(vector<double> featureVector, struct model *SVMModel);
vector<double> extractLBPFeatureVector(Mat src, int numBlocks, int rad, int pts, string mapping, bool normalizeHist);
vector<vector<Rect>> multiScaleDetection(Mat src, Size windowSize, double scaleFactor, int stride, struct model* SVMModel);
vector<Rect> nonMaximaSuppression(vector<Rect> detections, int minNeighbours, double overlapThresh);
void drawDetections(Mat& src, vector<Rect> detections, cv::Scalar color);
void svmDetect(ctx* context, Size windowSize, double scaleFactor, int stride, bool nms, int minNeighbours, double overlapThresh);

Mat returnLBPImage(Mat src, int rad, int pts, string mapping);
void createSVMTrainingFile(cv::String directory, string fileName, int label, int k, int rad, int pts, string mapping, bool normalizeHist);

#endif /*!OBJECTDETECT_H*/











