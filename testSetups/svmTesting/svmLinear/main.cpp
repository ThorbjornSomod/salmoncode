#include "imageRetriever.hpp"
#include "objectDetect.hpp"
#include "lbp.hpp"

int main(){

	// Initialize SVM model:

	const char* MODEL_FILE = "lbp-hf-5000-train.scale.model";

	const struct model* SVMModel;

	if((SVMModel = load_model(MODEL_FILE)) == 0){

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

	Mat src;

	// Retrieve filenames:

	cv::String testDir = "/home/thorbjornsomod/Desktop/coidal_validation";

	vector<cv::String> filenames;

	cv::glob(testDir, filenames);

	for(size_t i = 0; i < filenames.size(); ++i){

		src = imread(filenames[i], IMREAD_GRAYSCALE);

		cv::resize(src, src, Size(30,30), 0, 0, INTER_AREA);

		vector<vector<Rect>> svmPredictions = testDetection(src, SVMModel);

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

	cout << "Background pridictions:	" << back << endl;
	cout << "Head predictions:	" << head << endl;
	cout << "Dorsal predictions:	" << dorsal << endl;
	cout << "Coidal predictions: 	" << coidal << endl;
	cout << "Searched images:	" << total << endl;

	return 0;

}

