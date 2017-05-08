#include "tools.hpp"
#include "formatConverter.hpp"

using namespace std;

int main() {

	readAnnotationFile("/home/sealab/svmlearner/training/annotations/coidal_annotations.txt");

	//system("pause");

	//normalizeImageUsingGpu();

	//imageConverter();

	//Mat src;

	//for(int i = 1; i < 197; i++){

	//	src = imread("/home/sealab/svmlearner/training/pos/pos3/pos" + std::to_string(i) + ".png", IMREAD_UNCHANGED);

	//	imwrite("/home/sealab/svmlearner/training/pos/tmp/pos" + std::to_string(868 + i) + ".png", src);

	//}

	//imageConverter();

	//getRandomNegatives(); 

	return 0;

}
