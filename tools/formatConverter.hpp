#ifndef FORMATCONVERTER_H
#define FORMATCONVERTER_H

/*System Headers*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>

/*OpenCV Headers*/

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "opencv2/cudaimgproc.hpp"

using namespace std;
using namespace cv;
using namespace cuda;

/*Functions*/

void readAnnotationFile(string filename);

#endif /*!FORMATCONVERTER_H*/
