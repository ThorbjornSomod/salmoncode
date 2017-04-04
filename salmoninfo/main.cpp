#include "objectDetect.hpp"
#include "lbp.hpp"

int main(){

	//printAvgDims("/media/sealab/My Passport/fish_parts/coidal_fins");

	//printLargestDims("/media/sealab/My Passport/fish_parts/coidal_fins");

	createSVMTrainingFile("/home/sealab/salmoncode/salmoninfo/tmp/", "testSVM", 1, 5, 1, 8, "hf", true);

	return 0;

}
