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
	Mat cpy = img.clone();

	//vector<Rect> some = multiScaleDetection(img);

	Rect t1(100, 100, 100, 100);
	Rect t2(100, 100, 75, 75);
	Rect t3(50, 50, 100, 100);
	Rect t4(500, 500, 200, 200);

	rectangle(img, t1, Scalar(0, 0, 255));
	rectangle(img, t2, Scalar(0, 0, 255));
	rectangle(img, t3, Scalar(0, 0, 255));
	rectangle(img, t4, Scalar(0, 0, 255));

	vector<vector<Rect>> detections;

	vector<Rect> vec;
	
	vec.push_back(t1);
	vec.push_back(t2);
	vec.push_back(t3);
	vec.push_back(t4);
	
	detections.push_back(vec);

	imshow("DISPLAY", img);
	waitKey(5000);
	
	double overlapThresh = 0.5;

	detections = nonMaxSuppression(detections, overlapThresh);

	for(int i = 0; i < detections[0].size(); i++){
		
		rectangle(cpy, detections[0][i], Scalar(0, 0, 255));

	}

	imshow("DISPLAY", cpy);
	waitKey(5000); 	

	return 0;

}

