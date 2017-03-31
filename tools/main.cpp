#include "detectionTest.hpp"
#include "formatConverter.hpp"

using namespace std;

int main() {

	//detectionTest("grayscale.xml");

	readAnnotationFile("/home/sealab/lbpcudacascade/training/pos/annotations.txt");

	//system("pause");

	//normalizeImageUsingGpu();

	//imageConverter();

	return 0;

}
