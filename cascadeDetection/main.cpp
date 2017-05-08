#include "cascadeDetect.hpp"
#include "imageRetriever.hpp"

int main() {

	struct ctx* context = (struct ctx*)malloc(sizeof(*context));

	// Media sources:

	const char* streamSource = "rtsp://admin:ral1004@192.168.2.3:2020/videoinput_1/h264_1/media.stm";

	const char* fileSource = "file:///home/sealab/salmoncode/videos/salmon_1.mp4";

	// Start videofeed:

	std::thread imageRetriever(updateMatrixData, context, fileSource);

	// Detection using LBP Cascade Classifier:

	std::thread cascadeDetection(cascadeDetect, context);

	// Join threads:

	imageRetriever.join();
	cascadeDetection.join();

	return 0;

}
