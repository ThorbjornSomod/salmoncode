#include "objectDetect.hpp"

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

// Implementation of bounding window calculator:

Rect boundingWindowCalc(vector<Rect> cluster){

	int x = cluster[0].x;
	int y = cluster[0].y;
	int width = cluster[0].width;
	int height = cluster[0].height;

	if(cluster.size() > 1){

		for(int i = 1; i < cluster.size(); i++){

			if(cluster[i].x < x){
				x = cluster[i].x;
			}

			if(cluster[i].y < y){
				y = cluster[i].y;
			}

			if(cluster[i].width > width){
				width = cluster[i].width;
			}

			if(cluster[i].height > height){
				height = cluster[i].height;
			}
		}
	}

	return Rect(x, y, width, height);

}

// Function to delete vector elements from containing vector:

vector<Rect> deleteVectorElems(vector<Rect> regionProposals, vector<Rect> cluster){

	vector<bool> markedElements(regionProposals.size(), false);

	vector<Rect> tempBuffer;
	tempBuffer.reserve(regionProposals.size() - cluster.size());

	for(int i = 0; i < cluster.size(); i++){

		markedElements[i] = true;	
	
	}

	for(size_t j = 0; j < regionProposals.size(); j++){

		if(!markedElements[j]){

			tempBuffer.push_back(regionProposals[j]);

		}
	}

	return tempBuffer;

}

// Function to merge region proposals by edgebox algorithm:

vector<Rect> mergeRegions(vector<Rect> regionProposals){

	vector<Rect> cluster;
	vector<vector<Rect>> clusters;

	unsigned int largestSize;
	vector<Rect> largestCluster;

	bool merged;

	if(!regionProposals.empty()){

		do{

			merged = false;

			clusters.clear();

			for(unsigned int i = 0; i < regionProposals.size(); i++){

				cluster.clear();

				for(unsigned int j = 0; j < regionProposals.size(); j++){

					if(regionProposals[i] != regionProposals[j]){

						if(overlapCalc(regionProposals[i], regionProposals[j]) > 0){

							cluster.push_back(regionProposals[j]);

						}
					}
				}

				cluster.push_back(regionProposals[i]);
				clusters.push_back(cluster);

			}

			largestSize = 0;
			largestCluster.clear();

			for(unsigned int k = 0; k < clusters.size(); k++){

				if(clusters[k].size() > largestSize){
		
					largestSize = clusters[k].size();
					largestCluster = clusters[k];
			
				}
			}	
		
			regionProposals = deleteVectorElems(regionProposals, largestCluster);
	
			regionProposals.push_back(boundingWindowCalc(largestCluster));

			if(largestCluster.size() > 1){

				merged = true;

			}
		
		} while(merged);

	}

	return regionProposals;	

}

// Function to retrieve region proposals by edgebox filtering:

vector<Rect> edgeboxFiltering(Mat src){

	// Load edge model:

	String edgeModel = "model.yml";
	Ptr<StructuredEdgeDetection> pDollar = createStructuredEdgeDetection(edgeModel);

	// Convert src image:

	Mat srcRgb;
	cvtColor(src, srcRgb, COLOR_BGR2RGB);
	srcRgb.convertTo(srcRgb, CV_32F, 1.0 / 255.0f);
	
	// Create edge map:	

	Mat srcEdge;
	pDollar->detectEdges(srcRgb, srcEdge);

	// Compute orientation from edge map:

	Mat orientation;
	pDollar->computeOrientation(srcEdge, orientation);

	// Apply edge non maxima suppression:

	Mat nmsEdge;
	pDollar->edgesNms(srcEdge, orientation, nmsEdge, 2, 0, 1, true);

	// Retrieve region proposal boxes:

	vector<Rect> regionProposals;
	Ptr<EdgeBoxes> edgeboxes = createEdgeBoxes();
	edgeboxes->setMaxBoxes(30);
	edgeboxes->getBoundingBoxes(nmsEdge, orientation, regionProposals);

	// Merge overlapping region proposal boxes:

	regionProposals = mergeRegions(regionProposals);

	// Draw region proposals:

	/*

	if(!regionProposals.empty()){

		for(unsigned int i = 0; i < (int)regionProposals.size(); i++){
			Point p(regionProposals[i].x, regionProposals[i].y); 
			Point q(regionProposals[i].x + regionProposals[i].width, regionProposals[i].y + regionProposals[i].height);
			Scalar color(0, 255, 0);
			rectangle(src, p, q, color, 1);
	  	}
	}

	imshow("Display", src);
	waitKey(15);

	*/

	return regionProposals;

}

// Function to construct the image pyramid for multi-scale detection:

vector<vector<Mat>> constructRegionPyramids(Mat src, vector<Rect> regionProposals, Size windowSize, double scaleFactor){

	Mat tmp;
	Mat roi;
	
	// Convert source image to grayscale:

	cvtColor(src, tmp, CV_BGR2GRAY);

	vector<Mat> imagePyramid;
	vector<vector<Mat>> regionPyramids;

	// For each region proposal:

	for(unsigned int i = 0; i < regionProposals.size(); i++){

		roi = tmp(regionProposals[i]);

		imagePyramid.clear();

		// While the image is larger than the window size:

		while(windowSize.height < roi.rows && windowSize.width < roi.cols){

			// Add image to the pyramid and downsample:

			imagePyramid.push_back(roi);

			pyrDown(roi, roi, Size(roi.cols*scaleFactor, roi.rows*scaleFactor));

		}

		regionPyramids.push_back(imagePyramid);

	}

	return regionPyramids;

}

// Wrapper for LIBSVM's prediction function:

int svmPredict(vector<double> featureVector, struct svm_model *SVMModel){

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

// Function for extracting LBP feature vectors:

vector<double> extractLBPFeatureVector(Mat src, int numBlocks, int rad, int pts, string mapping, bool normalizeHist){

	// Save src image type:

	int type = src.type();

	// Convert image to double precision:

	src.convertTo(src, CV_64F);

	Mat lbpImg;

	// Check for sufficient channels:	
	
	switch(src.channels()){

		case 1:
			lbpImg = Mat(src.size(), CV_8UC1, Scalar(0));
			break;

		default:
			cerr << "Unsupported number of image channels, must be 1." << endl;
			exit(1);
		
	}

	// Create mapping:

	LBP lbp(pts, LBP::strToType(mapping));

	// Calculate the descriptor image:

	lbp.calcLBP(src, rad, true);

	// Allocate memory the same size as the image:

	Mat mask(src.rows, src.cols, type);

	// Calculate the block width/height:

	int blockWidth = static_cast<int>(floor(src.cols / numBlocks));
	int blockHeight = static_cast<int>(floor(src.rows / numBlocks));

	// Create vector to store histograms:

	vector<double> featureVector;

	// Get histogram for each block:

	vector<double> hist;

	int blockX, blockY;

	for(int r = 0; r < numBlocks; r++){

		for(int c = 0; c < numBlocks; c++){
		
			mask = Mat::zeros(src.rows, src.cols, type);

			blockX = (src.cols / numBlocks) * c;

			blockY = (src.rows / numBlocks) * r;

			Mat block(mask, Range(blockY, blockY + blockHeight), Range(blockX, blockX + blockWidth));
		
			block = Scalar(255);

			hist = lbp.calcHist(mask).getHist();
	
			featureVector.insert(featureVector.end(), hist.begin(), hist.end());

		}
	}
	
	return featureVector;

}

// Function to perform scale and location invariant detection:

vector<vector<Rect>> multiScaleDetection(Mat src, Size windowSize, double scaleFactor, int stride, struct svm_model* SVMModel){

	vector<Rect> regionProposals = edgeboxFiltering(src);

	vector<vector<Mat>> regionPyramids = constructRegionPyramids(src, regionProposals, windowSize, scaleFactor);

	// Initialize detection containers:

	vector<Rect> headDetections;
	vector<Rect> dorsalDetections;
	vector<Rect> coidalDetections;
	vector<vector<Rect>> detections;

	if(!regionPyramids.empty()){

		bool rowCheck;
		bool colCheck;

		Mat image;
		Mat imageRegion;
		Rect regionRect;

		int regionClass;
	
		double internalScaling = scaleFactor;

		// For each region of interest:

		for(unsigned int i = 0; i < regionPyramids.size(); i++){

			vector<Mat> imagePyramid = regionPyramids[i];

			std::vector<Mat>::iterator it;

			// For each layer of the respective image pyramid:

			for(it = imagePyramid.begin(); it != imagePyramid.end(); ++it){

				image = *it;

				rowCheck = false;
				colCheck = false;

				for(unsigned int r = 0; r < image.rows; r += stride){

					// If the window height is larger than the remaining image height:

					if(r + windowSize.height > image.rows){

						r = image.rows - windowSize.height;
						rowCheck = true;

					}

					for(unsigned int c = 0; c < image.cols; c += stride){

						// If the window width is larger than the remaining image width:

						if(c + windowSize.width > image.cols){

							c = image.cols - windowSize.width;
							colCheck = true;

						}

						// Isolate the region of interest we want to check:
			
						regionRect = Rect(c, r, windowSize.width, windowSize.height);
						imageRegion = image(regionRect);

						// Extract the LBP feature vector and scale it between -1 and 1:

						vector<double> featureVector = extractLBPFeatureVector(imageRegion, 5, 2, 8, "hf", true);

						double max = *max_element(featureVector.begin(), featureVector.end());
						double min = *min_element(featureVector.begin(), featureVector.end());

						for(unsigned int i = 0; i < featureVector.size(); i++){

							featureVector[i] = 2 * ((featureVector[i] - min) / (max - min)) - 1;

						}

						// Check if the area contains an object in a sought class:

						regionClass = svmPredict(featureVector, SVMModel);

						if(regionClass != 0){

							regionRect.x = floor(regionRect.x * (1 / internalScaling));
							regionRect.y = floor(regionRect.y * (1 / internalScaling));
							regionRect.width = floor(regionRect.width * (1 / internalScaling));
							regionRect.height = floor(regionRect.height * (1 / internalScaling));

						}

						// Append to correct detection container:

						switch(regionClass){

							case HEAD:
								headDetections.push_back(regionRect);
								break;
							case DORSAL:
								dorsalDetections.push_back(regionRect);
								break;
							case COIDAL:
								coidalDetections.push_back(regionRect);
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

				// Fix scaling for detections:

				internalScaling = internalScaling*scaleFactor;

			}

			detections.push_back(headDetections);
			detections.push_back(dorsalDetections);
			detections.push_back(coidalDetections);

		}
	}

	return detections;	
	
}

// Function implementing the non-maxima suppression algorithm:

vector<Rect> nonMaximaSuppression(vector<Rect> detections, int minNeighbours, double overlapThresh){

	vector<Rect> cluster;
	vector<vector<Rect>> clusters;

	// For each detection:

	if(!detections.empty()){

		clusters.clear();

		for(unsigned int i = 0; i < detections.size(); i++){

			cluster.clear();

			for(unsigned int j = 0; j < detections.size(); j++){

				// Check against all over detections:

				if(detections[i] != detections[j]){

					// If the bounding box of two detections overlap more than the threshold:

					if(overlapCalc(detections[i], detections[j]) > overlapThresh){

						// Add nth detection to cluster:

						cluster.push_back(detections[j]);

					}
				}
			}

			// Add cluster to clusters:

			cluster.push_back(detections[i]);
			clusters.push_back(cluster);

		}

		detections.clear();

		// If member of cluster has enough neighbours:

		for(unsigned int k = 0; k < clusters.size(); k++){

			if((clusters[k].size() - 1) > minNeighbours){

				// Calculate mean detection window and append to detections:

				detections.push_back(meanWindowCalc(clusters[k]));

			}
		}
	}

	return detections;

}

// Function to draw detections:

void drawDetections(Mat &src, vector<Rect> detections, cv::Scalar color){

	if(!detections.empty()){

		for(unsigned int i = 0; i < detections.size(); i++){

			cv::rectangle(src, detections[i], color);

		}
	}
}

void svmDetect(ctx* context, Size windowSize, double scaling, int stride, bool nms, int minNeighbours, double overlapThresh){

	// Initialize SVM model:

	const char *MODEL_FILE = "lbp-hf-1000-train.model";

	struct svm_model* SVMModel;

	if((SVMModel = svm_load_model(MODEL_FILE)) == 0){

		fprintf(stderr, "Can't load SVM model. %s", MODEL_FILE);

	}

	// Initialize detection vectors:

	vector<Rect> fishHeads;
	vector<Rect> dorsalFins;
	vector<Rect> coidalFins;

	// Initialize variables:

	Mat cpuFrame;
	Mat tmp;

	//int count = 0;

	sleep(1);

	while(true){

		// Acquire the mutex lock:

		context->imagemutex->lock();

		// Take a deep copy of current image:

		tmp = *context->image;

		cpuFrame = tmp.clone();

		// Release mutex lock:

		context->imagemutex->unlock();

		// Perform resizing:

		resize(cpuFrame, cpuFrame, Size(960, 540));

		// Acquire object detections:

		vector<vector<Rect>> svmDetections = multiScaleDetection(cpuFrame, windowSize, scaling, stride, SVMModel);

		// If flagged, perform non-maxima suppression:

		if(nms){

			fishHeads = nonMaximaSuppression(svmDetections[0], minNeighbours, overlapThresh);
			dorsalFins = nonMaximaSuppression(svmDetections[1], minNeighbours, overlapThresh);
			coidalFins = nonMaximaSuppression(svmDetections[2], minNeighbours, overlapThresh);

		}

		else{

			fishHeads = svmDetections[0];
			dorsalFins = svmDetections[1];
			coidalFins = svmDetections[2];
		
		}

		// Draw detections:

		drawDetections(cpuFrame, fishHeads, cv::Scalar(70, 212, 37));
		drawDetections(cpuFrame, dorsalFins, cv::Scalar(49, 37, 212));
		drawDetections(cpuFrame, coidalFins, cv::Scalar(181, 38, 213));

		//imwrite("/home/thorbjornsomod/Desktop/test/test" + to_string(count) + ".png", cpuFrame);

		//count++;

		imshow("Display", cpuFrame);		

		waitKey(5);

	}
}

// Function for returning LBP images:

Mat returnLBPImage(Mat src, int rad, int pts, string mapping){

	// Convert image to double precision:

	//cv::cvtColor(src, src, CV_BGR2GRAY);

	src.convertTo(src, CV_64F);

	Mat lbpImg;

	LBP lbp(pts, LBP::strToType(mapping));

	lbp.calcLBP(src, rad, true);

	lbpImg = lbp.getLBPImage();

	return lbpImg;
	
}

// Function to create SVM training files:

void createSVMTrainingFile(cv::String directory, string fileName, int label, int numBlocks, int rad, int pts, string mapping, bool normalizeHist){

	Mat src;

	ofstream myfile(fileName);

	// Retrieve filenames:

	vector<cv::String> filenames;

	cv::glob(directory, filenames);

	// Iterate through images:

	if(myfile.is_open()){

		for(size_t i = 0; i < filenames.size(); ++i){

			src = imread(filenames[i], IMREAD_GRAYSCALE);

			if(!src.data){

				cerr << "Problem loading image!" << endl;

			}
			
			// Retrieve descriptor for each image:

			vector<double> desc = extractLBPFeatureVector(src, numBlocks, rad, pts, mapping, normalizeHist);

			myfile << label << " ";

			for(int i = 0; i < desc.size(); i++){

				int index = i + 1;

				if(i == desc.size() - 1){

					myfile << index << ":" << desc[i] << "\n";

				}

				else{

				myfile << index << ":" << desc[i] << " ";

				}
			}
		}

		myfile.close();	
	
	}

	else{

		cout << "Reading file failed." << endl;

	}
}






































