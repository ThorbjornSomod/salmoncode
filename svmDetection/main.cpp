#include "imageRetriever.hpp"
#include "objectDetect.hpp"
#include "lbp.hpp"

Size windowSize(30, 30);
double scaling = 0.75;
int stride = 15;

bool gaussianBlur = true;
bool NMS = true;

int main(){

	/*

	// Set parameters:

	struct ctx* context = (struct ctx*)malloc(sizeof(*context));

	Size windowSize(50,50);

	double scaling = 0.5;

	int stride = 25;

	gaussianBlur = false;

	bool NMS = true;

	// Media sources:

	const char* streamSource = "rtsp://admin:ral1004@192.168.2.3:2020/videoinput_1/h264_1/media.stm";

	const char* fileSource = "file:///home/thorbjornsomod/salmoncode/svmDetection/vlc_hitra_salmon_resized.mp4";

	// Start videofeed:

	std::thread imageRetriever(updateMatrixData, context, fileSource);

	// Detection using LBP SVM classifier:

	std::thread svmDetection(svmDetect, context, windowSize, scaling, stride, gaussianBlur, NMS);

	// Join threads:

	imageRetriever.join();
	svmDetection.join();

	*/	

	createSVMTrainingFile("/home/thorbjornsomod/Desktop/classImages/background", "/home/thorbjornsomod/Desktop/svmTrainingFiles/lbp/lbp-5000/lbp-5000-background.txt", 0, 5, 2, 8, "hf", 1);

	return 0;

}

