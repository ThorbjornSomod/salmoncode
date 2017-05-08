#include "imageRetriever.hpp"
#include "objectDetect.hpp"
#include "lbp.hpp"

Size windowSize(30, 30);
double scaling = 0.75;
int stride = 15;

bool gaussianBlur = true;
bool NMS = true;

int main(){

	// Initialize SVM model:

	const char* MODEL_FILE = "trainSVM.model";

	struct svm_model* SVMModel;

	if((SVMModel = svm_load_model(MODEL_FILE)) == 0){

		fprintf(stderr, "Can't load SVM model. %s", MODEL_FILE);

	}

	// Initialize result containers:

	int head, dorsal, coidal, back;

	vector<Rect> fishHeads;
	vector<Rect> dorsalFins;
	vector<Rect> coidalFins;

	// Initialize variables:

	Size windowSize(30,30);

	double scaling = 0.5;

	int stride = 10;

	bool gaussianBlur = true;

	bool NMS = true;

	Mat src;

	cv::String testDir = "";

	vector<cv::String> filenames;

	cv::glob(testDir, filenames);

	for(size_t i = 0; i < filenames.size(); ++i){

		src = imread(filenames[i], IMREAD_GRAYSCALE);

		vector<vector<Rect>> svmPredictions = multiScaleDetection(src, windowSize, scaling, stride, gaussianBlur, NMS, SVMModel);

		fishHeads = svmPredictions[0];
		dorsalFins = svmPredictions[1];
		coidalFins = svmPredictions[2];

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

	}

	cout << "Head predictions:	" << head << endl;
	cout << "Dorsal predictions:	" << dorsal << endl;
	cout << "Coidal predictions: 	" << coidal << endl;
	cout << "Background predictions:	" << back << endl;

	return 0;

}

