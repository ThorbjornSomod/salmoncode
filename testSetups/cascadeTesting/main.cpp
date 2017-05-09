#include "cascadeDetect.hpp"
#include "imageRetriever.hpp"

int main() {

	Mat src;

	GpuMat gpuSrc;

	GpuMat objbuf;

	int head, dorsal, coidal, back, total;

	String testDir = "/home/sealab/lbpcudacascade/training/val/coidal_validation/";

	// Load cascades:

	Ptr<cv::cuda::CascadeClassifier> coidal_cascade = cv::cuda::CascadeClassifier::create("current_coidal.xml");
	Ptr<cv::cuda::CascadeClassifier> head_cascade = cv::cuda::CascadeClassifier::create("current_head.xml");
	Ptr<cv::cuda::CascadeClassifier> dorsal_cascade = cv::cuda::CascadeClassifier::create("current_dorsal.xml");

	// Retrieve filenames:

	vector<cv::String> filenames;

	cv::glob(testDir, filenames);

	// Iterate through images:

	for(size_t i = 0; i < filenames.size(); ++i){

		src = imread(filenames[i], IMREAD_GRAYSCALE);

		if(!src.data){

			cerr << "Problem loading image!" << endl;				

		}

		gpuSrc.upload(src);

		//cv::cuda::cvtColor(gpuSrc, gpuSrc, CV_BGR2GRAY);

		std::vector<Rect> fishHeads;
		std::vector<Rect> coidalFins;
		std::vector<Rect> dorsalFins;

		head_cascade->detectMultiScale(gpuSrc, objbuf);
		head_cascade->convert(objbuf, fishHeads);

		coidal_cascade->detectMultiScale(gpuSrc, objbuf);
		coidal_cascade->convert(objbuf, coidalFins);

		dorsal_cascade->detectMultiScale(gpuSrc, objbuf);
		dorsal_cascade->convert(objbuf, dorsalFins);

		if(!fishHeads.empty()){

			head++;

		}

		if(!dorsalFins.empty()){

			dorsal++;

		}

		if(!coidalFins.empty()){

			coidal++;
		
		}

		if(fishHeads.empty() && dorsalFins.empty() && coidalFins.empty()){

			back++;

		}
	
	total++;

	}

	cout << "Head predictions:	" << head << endl;
	cout << "Dorsal predictions:	" << dorsal << endl;
	cout << "Coidal predictions:	" << coidal << endl;
	cout << "Background predictions:	" << back << endl;

	gpuSrc.release();
	objbuf.release();

	return 0;

}
