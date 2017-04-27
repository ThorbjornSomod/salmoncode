#include "cascadeDetect.hpp"

int main() {

	// Detection using LBP Cascade Classifier:

	std::string minipod = "rtsp://admin:ral1004@192.168.2.3:2020/videoinput_1/h264_1/media.stm";

	std::string file_source = "/home/sealab/salmoncode/cascadeDetection/converted.mp4";

	cascadeDetect(file_source, "current_coidal.xml", "current_head.xml", "current_dorsal.xml");

	return 0;

}
