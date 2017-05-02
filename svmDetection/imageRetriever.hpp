#ifndef IMAGERETRIEVER_H
#define IMAGERETRIEVER_H

/*System Headers*/

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <mutex>
#include <pthread.h>
#include <unistd.h>

/*VLC Library Headers*/

#include <vlc/vlc.h>

/*OpenCV Library Headers*/

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

/*Namespaces*/

using namespace std;
using namespace cv;

/*Defines*/

#define VIDEO_WIDTH	1920
#define VIDEO_HEIGHT	1080

/*Datastructures*/

#include "./types.hpp"

/*Functions*/

void* lock(void* data, void** p_pixels);
void display(void* data, void* id);
void unlock(void* data, void* id, void *const* p_pixels);
void* updateMatrixData(void* voidContext);

#endif /*!IMAGERETRIEVER_H*/
