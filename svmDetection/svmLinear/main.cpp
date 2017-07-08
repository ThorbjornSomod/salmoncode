#include "imageRetriever.hpp"
#include "objectDetect.hpp"
#include "lbp.hpp"

int main(){

	// Set parameters:

	struct ctx* context = (struct ctx*)malloc(sizeof(*context));

	Size windowSize(35,35);

	double scaleFactor = 0.5;

	int stride = 10;

	bool nms = true;

	int minNeighbours = 2;

	double overlapThresh = 0.25;

	// Media sources:

	const char* streamSource = "rtsp://admin:ral1004@192.168.2.3:2020/videoinput_1/h264_1/media.stm";

	const char* fileSource = "file:///home/thorbjornsomod/salmoncode/svmDetection/svmLinear/validation.mp4";

	// Start videofeed:

	std::thread imageRetriever(updateMatrixData, context, fileSource);

	// Detection using LBP SVM classifier:

	std::thread svmDetection(svmDetect, context, windowSize, scaleFactor, stride, nms, minNeighbours, overlapThresh);

	// Join threads:

	imageRetriever.join();
	svmDetection.join();

	return 0;

}
