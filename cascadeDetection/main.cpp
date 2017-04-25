#include "cascadeDetect.hpp"

using namespace std;

int main() {

	// Video sources:

	std::string aquapod = "rtsp://admin:ral1004@192.168.1.6:2020/videoinput_1/h264_1/media.stm";

	std::string minipod = "rtsp://something";

	std::string video_source = "/home/sealab/svmlearner/videos/testing.mp4";

	// Detection using LBP Cascade Classifier:

	cascadeDetect(video_source, "current_coidal.xml", "current_head.xml", "current_dorsal.xml");

	return 0;

}
