#include "objectDetect.hpp"
#include "lbp.hpp"

int main(){

	//start = clock();

	//vector<double> hist = extractLBPFeatures(1, 8, "hf", "coidal_1_1.png", false);

	//finish = clock();

	//duration = double(finish - start);

	//seconds = duration / double(CLOCKS_PER_SEC);

	//cout << "Feature extraction took: " << seconds << " seconds." << endl;
	
	//cout << "Size of feature vector: " << hist.size() << endl;

	//cout << "Feature vector elements: ";


	//for(int i = 0; i < hist.size(); i++){

	//	cout << hist[i] << "\t";

	//}

	createSVMTrainingFile("/home/sealab/salmoncode/salmoninfo/tmp/", "testSVM", 1, 1, 8, "u2", false);

	return 0;

}
