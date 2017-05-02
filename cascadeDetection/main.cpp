#include "cascadeDetect.hpp"
#include "imageRetriever.hpp"

void* imageTest(void* voidContext){
	
	struct ctx* context = static_cast<struct ctx*>(voidContext);
	
	int i = 0;

	Mat show;

	while(true){

		i++;
		
		if (i > 5) {

		context->imagemutex->lock();

		show = *context->image;

		context->imagemutex->unlock();

		imshow("Detections", show);
		
		}

		waitKey(15);

	}

	return NULL;

}

int main() {

	// Intializing context:

	struct ctx* context = (struct ctx*)malloc(sizeof(*context));

	// Start RTSP Streaming:

	pthread_t imageRetrieve;
	//pthread_t imageTester;
	pthread_t cascadeDetection;

	pthread_create(&imageRetrieve, NULL, updateMatrixData, static_cast<void*>(context));

	usleep(100000);

	//pthread_create(&imageTester, NULL, imageTest, static_cast<void*>(context)); 
	
	pthread_create(&cascadeDetection, NULL, cascadeDetect, static_cast<void*>(context));

	pthread_join(imageRetrieve, NULL);
	//pthread_join(imageTester, NULL);
	pthread_join(cascadeDetection, NULL);

	return 0;

}
