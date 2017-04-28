#include "cascadeDetect.hpp"
#include "imageRetriever.hpp"

void* imageTest(void* voidContext){
	
	struct ctx* context = static_cast<struct ctx*>(voidContext);
	
	int i = 0;

	while(true){

		i++;
		
		if (i > 5) {

		context->imagemutex->lock();

		imshow("Display", *context->image);

		context->imagemutex->unlock();
		
		}

		waitKey(15);

	}

	return NULL;

}

int main() {

	struct ctx* context = (struct ctx*)malloc(sizeof(*context));

	// Media sources:

	std::string videoSource = "rtsp://admin:ral1004@192.168.2.3:2020/videoinput_1/h264_1/media.stm";

	std::string fileSource = "/home/sealab/salmoncode/cascadeDetection/converted.mp4";

	// Start RTSP Streaming:

	pthread_t imageRetrieve;
	pthread_t imageTester;

	pthread_create(&imageRetrieve, NULL, updateMatrixData, static_cast<void*>(context));

	usleep(1000000);

	pthread_create(&imageTester, NULL, imageTest, static_cast<void*>(context));

	//imshow("Display", *context->image);

	pthread_join(imageRetrieve, NULL);
	pthread_join(imageTester, NULL);

	// Detection using LBP Cascade Classifier:

	//cascadeDetect(file_source, "current_coidal.xml", "current_head.xml", "current_dorsal.xml");

	return 0;

}
