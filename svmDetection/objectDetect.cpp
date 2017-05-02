#include "objectDetect.hpp"

vector<Rect> multiScaleDetection(Mat src, Size minSize, Size maxSize, double scaling, int stride){

	// Setting detection parameters:

	Size minSize = Size(30, 30);

	Size maxSize;

	double scaling = 1.1;

	int stride = 4;

	// Initialize SVM model:

	const char *MODEL_FILE = "trainSVM.model";

	struct svm_model *SVMModel;

	if((SVMModel = svm_load_model(MODEL_FILE)) == 0){

		fprintf(stderr, "Can't load SVM model. %s", MODEL_FILE);
	
	}

	// Initialize window size:

	Size windowSize = minSize;
	bool rowCheck, colCheck;
	Rect roi;
	Mat imageRoi;
	Mat preProcessed;

	vector<Rect> detections;
	
	// While the sliding window is not larger than the max window size:

	while(windowSize.height < maxSize.height && windowSize.width < maxSize.width){

		rowCheck = false;
		colCheck = false;

		for(int r = 0; r < src.rows; r += stride){
			
			// If the window height is larger than the remaining image height: 		

			if(r + windowSize.height > src.rows){

				r = src.rows - windowSize.height;
				rowCheck = true;

			}

			for(int c = 0; c < src.cols; c += stride){
				
				// If the window width is larger than the remaining image width:

				if(c + windowSize.width > src.cols){

					c = src.cols - windowSize.width;
					colCheck = true;

				}

				// Crop out the region of interest:

				roi = Rect(c, r, windowSize.width, windowSize.height);
				imageRoi = src(roi);
				
				// Pre-process region:

				preProcessed  = preProcessImage(imageRoi, minSize); 	
			
				vector<double> featureVector = extractLBPFeatureVector(preProcessed, 5, 1, 8, "hf", true);
	
				// If region contains a detection, append to detections:
				
				if(svmDetect(featureVector, SVMModel) > 0){

					detections.push_back(roi);

				}

				if(rowCheck && colCheck){

					break;				
	
				} 	
			}
		}

		// Update window size to new scale:

		windowSize.height = windowSize.height * scaling;
		windowSize.width = windowSize.width * scaling;
		
	}

	return detections;
	
}

Mat preProcessImage(Mat src, Size outputDims){

	Mat dst;

	cv::resize(src, dst, outputDims, 0, 0, INTER_AREA);

	cv::equalizeHist(dst, dst);

	cv::GaussianBlur(dst, dst, Size(5,5), 0, 0, BORDER_DEFAULT);

	return dst;

}

int svmDetect(vector<double> featureVector, struct svm_model *SVMModel){

	struct svm_node *svmVec;

	svmVec = (struct svm_node *)malloc((featureVector.size() + 1)*sizeof(struct svm_node));

	int i;

	for(i = 0; i < featureVector.size(); i++){

		svmVec[i].index = i + 1;
		svmVec[i].value = featureVector[i];

	}

	svmVec[i].index = -1;

	int prediction = svm_predict(SVMModel, svmVec);

	return prediction;

}
