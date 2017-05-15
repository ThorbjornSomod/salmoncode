#include "imageRetriever.hpp"
#include "objectDetect.hpp"
#include "lbp.hpp"

Size windowSize(30, 30);
double scaling = 0.75;
int stride = 15;

bool gaussianBlur = true;
bool NMS = true;

int main(){

	// Set parameters:

	struct ctx* context = (struct ctx*)malloc(sizeof(*context));

	Size windowSize(30,30);

	double scaling = 0.5;

	int stride = 10;

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

	//createSVMTrainingFile("/home/sealab/svmlearner/training/classImages/coidal/", "/home/sealab/svmlearner/training/svmTrainingFiles/svmCoidalFile.txt", 3, 5, 2, 8, "u2", 1);

	return 0;

}

