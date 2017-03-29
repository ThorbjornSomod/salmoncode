#include "detectionTest.hpp"

using namespace std;

int main() {

	//resizeUsingOpenCvCpu("purple_mountain.jpg", "purple_mountain_cpu.jpg");

	//resizeUsingOpenCvGpu("purple_mountain.jpg", "purple_mountain_gpu.jpg");

	//changeColorSpaceOfImageUsingGpu("purple_mountain.jpg", "hue_mountain_gpu.jpg");

	//system("pause");

	//changeColorSpaceOfVideoUsingGpu("vlc_hitra_salmon_resized.mp4", "vlc_hitra_salmon_hsv.mp4");

	detectionTest("grayscale.xml");

	//std::cout << "Hello world!" << std::endl;

	//normalizeImageUsingGpu();

	//imageConverter();

	return 0;

}
