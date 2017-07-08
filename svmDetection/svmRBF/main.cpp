#include "imageRetriever.hpp"
#include "objectDetect.hpp"
#include "lbp.hpp"

int main(){

	// Set parameters:

	struct ctx* context = (struct ctx*)malloc(sizeof(*context));

	Size windowSize(15,15);

	double scaling = 0.5;

	int stride = 5;

	bool nms = true;

	int minNeighbours = 2;

	bool overlapThresh = 0.3;

	// Media sources:

	const char* streamSource = "rtsp://admin:ral1004@192.168.2.3:2020/videoinput_1/h264_1/media.stm";

	const char* fileSource = "file:///home/thorbjornsomod/salmoncode/svmDetection/svmRBF/validation.mp4";

	// Start videofeed:

	std::thread imageRetriever(updateMatrixData, context, fileSource);

	// Detection using LBP SVM classifier:

	std::thread svmDetection(svmDetect, context, windowSize, scaling, stride, nms, minNeighbours, overlapThresh);

	// Join threads:

	imageRetriever.join();
	svmDetection.join();	

	/*

	String directory = "/home/thorbjornsomod/Desktop/collage/collage-lbp-u2";

	vector<cv::String> filenames;

	cv::glob(directory, filenames);

	for(size_t i = 0; i < filenames.size(); ++i){

		Mat tmp;		

		tmp = imread(filenames[i], CV_LOAD_IMAGE_GRAYSCALE);
	
		tmp = returnLBPImage(tmp, 2, 8, "u2");

		//cv::equalizeHist(tmp, tmp);	

		imwrite(filenames[i], tmp);

	}*/

	return 0;

}
