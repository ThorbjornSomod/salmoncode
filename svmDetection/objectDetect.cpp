#include "objectDetect.hpp"

// Implementation of the multi-scale detection algorithm:

vector<Rect> multiScaleDetection(Mat src){

	// Toggle Gaussian blurring:

	bool gaussianBlur = 0;

	// Setting detection parameters:

	Size windowSize(30,30);

	double scaling = 0.5;

	int stride = 10;

	// Initialize SVM model:

	const char *MODEL_FILE = "trainSVM.model";

	struct svm_model *SVMModel;

	if((SVMModel = svm_load_model(MODEL_FILE)) == 0){

		fprintf(stderr, "Can't load SVM model. %s", MODEL_FILE);
	
	}

	// Initialize variables:

	Rect roi;
	Mat imageRoi;
	Mat preProcessed;
	vector<Rect> detections;
	
	// Create the image pyramid:

	vector<Mat> imagePyramid = constructImagePyramid(src, windowSize, scaling, gaussianBlur);

	// Acquire detections:

	vector<vector<Rect>> test = slidingWindowDetection(imagePyramid, windowSize, scaling, stride, SVMModel);

	return detections;
	
}

// Implementation of the image pyramid:

vector<Mat>constructImagePyramid(Mat src, Size windowSize, double scaling, bool gaussianBlur){

	Mat tmp = src;
	
	vector<Mat> imagePyramid;

	int i = 0;

	while(windowSize.height < tmp.rows && windowSize.width < tmp.cols){

		//std::string name = "/home/sealab/salmoncode/svmDetection/pyramid/imagePyramidLevel-" + to_string(i) + ".png";

		//imwrite(name, tmp);

		imagePyramid.push_back(tmp);

		cv::resize(tmp, tmp, Size(), scaling, scaling, INTER_AREA);

		if(gaussianBlur){

			cv::GaussianBlur(tmp, tmp, Size(5,5), 0, 0, BORDER_DEFAULT);

		}

		i++;

	}

	return imagePyramid;
	
}

// Implementation of the sliding window function:

vector<vector<Rect>> slidingWindowDetection(vector<Mat> imagePyramid, Size windowSize, double scaling, int stride, struct svm_model* SVMModel){

	// Initialize detection containers:

	vector<vector<Rect>> detections;

	vector<Rect> headDetections;
	vector<Rect> coidalDetections;
	vector<Rect> dorsalDetections;

	// Initialize variables:

	bool rowCheck;
	bool colCheck;

	Mat image;
	Mat roiImage;
	Rect roiRect;

	int classCheck;

	double internalScaling = scaling;

	std::vector<Mat>::iterator it;

	// For each intermediate image in the image pyramid:

	for(it = imagePyramid.begin(); it != imagePyramid.end(); ++it){

		image = *it; 

		rowCheck = false;
		colCheck = false;

		for(int r = 0; r < image.rows; r += stride){

			// If the window height is larger than the remaining image height:

			if(r + windowSize.height > image.rows){

				r = image.rows - windowSize.height;
				rowCheck = true;

			}

			for(int c = 0; c < image.cols; c += stride){

				// If the window width is larger than the remaining image width:

				if(c + windowSize.width > image.cols){

					c = image.cols - windowSize.width;
					colCheck = true;

				}

				// Isolate the region of interest we want to check:
			
				roiRect = Rect(c, r, windowSize.width, windowSize.height);
				roiImage = image(roiRect);

				// Extract the LBP Feature Vector:

				vector<double> featureVector = extractLBPFeatureVector(roiImage, 5, 1, 8, "hf", true);

				// Check if the area contains an object in a sought class:

				classCheck = svmDetect(featureVector, SVMModel);

				if(classCheck != 0){

					roiRect.x = floor(roiRect.x * (1 / internalScaling));
					roiRect.y = floor(roiRect.y * (1 / internalScaling));
					roiRect.width = floor(roiRect.width * (1 / internalScaling));
					roiRect.height = floor(roiRect.height * (1 / internalScaling));

				}

				switch(classCheck){

					case HEAD:
						
						headDetections.push_back(roiRect);

						break;

					case DORSAL:

						dorsalDetections.push_back(roiRect);

						break;

					case COIDAL:

						coidalDetections.push_back(roiRect);
					
					default:

						break;

				}	
			
				if(colCheck){

					colCheck = false;
					break;

				}
			}

			if(rowCheck){

				break;

			}
		}

		internalScaling = internalScaling*scaling;

	}

	detections.push_back(headDetections);
	detections.push_back(dorsalDetections);
	detections.push_back(coidalDetections);

	return detections;

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
