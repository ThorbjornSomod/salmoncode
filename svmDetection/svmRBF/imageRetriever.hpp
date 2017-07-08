#ifndef IMAGERETRIEVER_H
#define IMAGERETRIEVER_H

/*System headers*/

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <thread>
#include <mutex>
#include <unistd.h>

/*VLC library headers*/

#include <vlc/vlc.h>

/*OpenCV library headers*/

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

/*Datastructures*/

#include "./types.hpp"

/*Namespaces*/

using namespace std;
using namespace cv;

/*Defines*/

#define VIDEO_WIDTH	1920
#define VIDEO_HEIGHT	1080

/*Functions*/

void* lock(void* data, void** p_pixels);
void display(void* data, void* id);
void unlock(void* data, void* id, void *const* p_pixels);
void* updateMatrixData(ctx* context, const char* source);

#endif /*!IMAGERETRIEVER_H*/
