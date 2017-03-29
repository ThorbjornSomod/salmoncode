#include "objectDetect.hpp"

vector<double> extractLBPFeatures(int rad, int pts, string mapping, string filename, bool normalizeHist){

	// Read an (RGB) image and convert to monochrome:

	Mat imgOrg = imread(filename, IMREAD_UNCHANGED);

	// Convert image to double precision:

	imgOrg.convertTo(imgOrg, CV_64F);

	Mat lbpImg;

	switch(imgOrg.channels()){

		case 1: 
			lbpImg = Mat(imgOrg.size(), CV_8UC1, Scalar(0));
			break;

		case 3: lbpImg = Mat(imgOrg.size(), CV_8UC3, Scalar(0));
			break;

		default:
			cerr << "Unsupported number of image channels, 1/3 only." << endl;
			exit(1);

	}

	// Create an LBP instance of type "mapping", using "pts" support points:

	LBP lbp(pts, LBP::strToType(mapping));

	for(int i = 0; i < imgOrg.channels(); i++){

		// Copy channel i of the image:

		Mat img(imgOrg.size(), imgOrg.depth(), 1);
		const int from_to1[] = {i, 0};
		mixChannels(&imgOrg, 1, &img, 1, from_to1, 1);

		// Calculate the descriptor:

		lbp.calcLBP(img, rad, true);

		// Copy the LBP image:

		const int from_to2[] = {0, i};
		Mat tmpImg = lbp.getLBPImage();
		mixChannels(&tmpImg, 1, &lbpImg, 1, from_to2, 1);
	
	}

	vector<double> hist = lbp.calcHist().getHist(normalizeHist);
		
	return hist;

}
