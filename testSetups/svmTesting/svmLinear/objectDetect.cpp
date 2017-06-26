#include "objectDetect.hpp"

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

vector<vector<Rect>> slidingWindowDetection(vector<Mat> imagePyramid, Size windowSize, double scaling, int stride, bool nonMaxSuppression, const struct model* SVMModel){

	// Initialize detection containers:

	vector<vector<Rect>> detections;

	vector<Rect> headDetections;
	vector<Rect> dorsalDetections;
	vector<Rect> coidalDetections;

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

				vector<double> featureVector = extractLBPFeatureVector(roiImage, 5, 2, 8, "hf", true);
				
				double max = *max_element(featureVector.begin(), featureVector.end());
				double min = *min_element(featureVector.begin(), featureVector.end());

				for(int i = 0; i < featureVector.size(); i++){

					featureVector[i] = 2 * ((featureVector[i] - min) / (max - min)) - 1;
				
				}

				// Check if the area contains an object in a sought class:

				classCheck = svmPredict(featureVector, SVMModel);

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
						break;
					
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

int svmPredict(vector<double> featureVector, const struct model *SVMModel){

	struct feature_node *svmVec;

	svmVec = (struct feature_node *)malloc((featureVector.size() + 1)*sizeof(struct feature_node));

	int i;

	for(i = 0; i < featureVector.size(); i++){

		svmVec[i].index = i + 1;
		svmVec[i].value = featureVector[i];

	}

	svmVec[i].index = -1;

	int prediction = static_cast<int>(predict(SVMModel, svmVec));

	return prediction;

}

// Implementation of non maximum suppression:

vector<vector<Rect>> nonMaxSuppression(vector<vector<Rect>> detections, double overlapThresh){

	vector<vector<Rect>> processedDetections;
	vector<Rect> classDetections;
	vector<Rect> cluster;

	for(int i = 0; i < detections.size(); i++){

		classDetections.clear();

		while(detections[i].size() > 1){

			cluster.clear();

			for(int j = 1; j < detections[i].size(); j++){

				cout << overlapCalc(detections[i][0], detections[i][j]) << endl;

				if(overlapCalc(detections[i][0], detections[i][j]) > overlapThresh){

					cluster.push_back(detections[i][j]);

					detections[i].erase(detections[i].begin() + j);

					j -= 1;

				}
			}

			cluster.push_back(detections[i][0]);
	
			detections[i].erase(detections[i].begin() + 0);

			classDetections.push_back(meanWindowCalc(cluster));
		}

		classDetections.push_back(detections[i][0]);

		detections[i].erase(detections[i].begin() + 0);

		processedDetections.push_back(classDetections);		

	}

	return processedDetections;

}

// Implementation of overlap calcualtor:

double overlapCalc(Rect a, Rect b){

	double intersection = (a & b).area();
	double rectUnion = a.area() + b.area() - intersection;
	double overlap = intersection / rectUnion;

	return overlap;

}

// Implementation of mean window calculator:

Rect meanWindowCalc(vector<Rect> cluster){

	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	for(int i = 0; i < cluster.size(); i++){

		x += cluster[i].x;
		y += cluster[i].y;
		width += cluster[i].width;
		height += cluster[i].height;

	}

	return Rect(x / cluster.size(), y / cluster.size(), width / cluster.size(), height / cluster.size());

}

// Implementation of the multi-scale detection algorithm:

vector<vector<Rect>> multiScaleDetection(Mat src, Size windowSize, double scaling, int stride, bool gaussianBlur, bool nonMaxSuppression, const struct model *SVMModel){
	
	// Create the image pyramid:

	vector<Mat> imagePyramid = constructImagePyramid(src, windowSize, scaling, gaussianBlur);

	// Acquire detections:

	vector<vector<Rect>> detections = slidingWindowDetection(imagePyramid, windowSize, scaling, stride, nonMaxSuppression, SVMModel);

	return detections;
	
}

void svmDetect(ctx* context, Size windowSize, double scaling, int stride, bool gaussianBlur, bool nonMaxSuppression){

	// Toggle detections:

	bool detectHeads = true;
	bool detectDorsals = true;
	bool detectCoidals = true;

	// Initialize SVM model:

	const char *MODEL_FILE = "trainSVM.model";

	const struct model* SVMModel;

	if((SVMModel = load_model(MODEL_FILE)) == 0){

		fprintf(stderr, "Can't load SVM model. %s", MODEL_FILE);

	}

	// Initialize detection vectors:

	vector<Rect> fishHeads;
	vector<Rect> dorsalFins;
	vector<Rect> coidalFins;

	// Initialize variables:

	Mat cpuFrame;
	Mat tmp;

	while(true){

		context->imagemutex->lock();

		tmp = *context->image;

		cpuFrame = tmp.clone();

		vector<vector<Rect>> svmDetections = multiScaleDetection(cpuFrame, windowSize, scaling, stride, gaussianBlur, nonMaxSuppression, SVMModel);

		fishHeads = svmDetections[0];
		dorsalFins = svmDetections[1];
		coidalFins = svmDetections[2];

		if(!fishHeads.empty() && detectHeads){

			for(int i = 0; i < fishHeads.size(); i++){

				cv::rectangle(cpuFrame, fishHeads[i], cv::Scalar(70, 212, 37));

			}
		}

		if(!dorsalFins.empty() && detectDorsals){

			for(int i = 0; i < dorsalFins.size(); i++){

				cv::rectangle(cpuFrame, dorsalFins[i], cv::Scalar(49, 37, 212));

			}
		}
		
		if(!coidalFins.empty() && detectCoidals){

			for(int i = 0; i < coidalFins.size(); i++){

				cv::rectangle(cpuFrame, coidalFins[i], cv::Scalar(181, 38, 213));

			}
		}

		imshow("Display", cpuFrame);

		waitKey(5);

	}
}

vector<vector<Rect>> testDetection(Mat src, const struct model *SVMModel){

	vector<vector<Rect>> detections;

	vector<Rect> headDetections;
	vector<Rect> dorsalDetections;
	vector<Rect> coidalDetections;

	Rect roiRect;
	
	int classCheck;

	Mat image = src;

	vector<double> featureVector = extractLBPFeatureVector(image, 5, 2, 8, "u2", true);
				
	double max = *max_element(featureVector.begin(), featureVector.end());
	double min = *min_element(featureVector.begin(), featureVector.end());

	for(int i = 0; i < featureVector.size(); i++){

		featureVector[i] = 2 * ((featureVector[i] - min) / (max - min)) - 1;
				
	}

	// Check if the area contains an object in a sought class:

	classCheck = svmPredict(featureVector, SVMModel);

	if(classCheck != 0){

		roiRect.x = 1;
		roiRect.y = 1;
		roiRect.width = image.cols - 1; 
		roiRect.height = image.rows - 1; 

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
			break;
					
		default:

			break;

		}	
		
	detections.push_back(headDetections);
	detections.push_back(dorsalDetections);
	detections.push_back(coidalDetections);
	
	return detections;

}















