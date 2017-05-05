#include "imageRetriever.hpp"
#include "objectDetect.hpp"
#include "lbp.hpp"

int main(){

	struct ctx* context = (struct ctx*)malloc(sizeof(*context));

	// Media sources:

	const char* streamSource = "rtsp://admin:ral1004@192.168.2.3:2020/videoinput_1/h264_1/media.stm";

	const char* fileSource = "file:///home/sealab/svmlearner/videos/testing.mp4";

	//std::thread imageRetriever(updateMatrixData, context, fileSource);

	// Join threads:

	//imageRetriever.join();

	// TESTS:

	Mat img = imread("kitty.jpg", IMREAD_UNCHANGED);

	//vector<Rect> some = multiScaleDetection(img);

	return 0;

}
