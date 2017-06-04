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

	const char* MODEL_FILE = "/home/thorbjornsomod/Desktop/svmTrainingFiles/lbp-u2/lbp-u2-5000/lbp-u2-5000-train.model";

	struct svm_model* SVMModel;

	if((SVMModel = svm_load_model(MODEL_FILE)) == 0){

		fprintf(stderr, "Can't load SVM model. %s", MODEL_FILE);

	}

	// Initialize result containers:

	int head = 0;
	int dorsal = 0;
	int coidal = 0;
	int back = 0;
	int total = 0;

	vector<Rect> fishHeads;
	vector<Rect> dorsalFins;
	vector<Rect> coidalFins;

	// Initialize variables:

	Size windowSize(30,30);

	double scaling = 0.5;

	int stride = 10;

	bool gaussianBlur = false;

	bool NMS = true;

	Mat src;

	// Retrieve filenames:

	cv::String testDir = "/home/thorbjornsomod/Desktop/head_validation";

	vector<cv::String> filenames;

	cv::glob(testDir, filenames);

	for(size_t i = 0; i < filenames.size(); ++i){

		src = imread(filenames[i], IMREAD_GRAYSCALE);

		vector<vector<Rect>> svmPredictions = multiScaleDetection(src, windowSize, scaling, stride, gaussianBlur, NMS, SVMModel);

		fishHeads = svmPredictions[0];
		dorsalFins = svmPredictions[1];
		coidalFins = svmPredictions[2];

		if(fishHeads.size() != 0){

			head++;

		}

		if(dorsalFins.size() != 0){

			dorsal++;

		}

		if(coidalFins.size() != 0){

			coidal++;

		}

		if(fishHeads.size() == 0 && dorsalFins.size() == 0 && coidalFins.size() == 0){

			back++;

		}	

	total++;

	}

	cout << "Head predictions:	" << head << endl;
	cout << "Dorsal predictions:	" << dorsal << endl;
	cout << "Coidal predictions: 	" << coidal << endl;
	cout << "Background predictions:	" << back << endl;
	cout << "Searched images:	" << total << endl;

	return 0;

}

