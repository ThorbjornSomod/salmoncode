#include "objectDetect.hpp"

vector<Rect> multiScaleDetection(Mat src, Size minSize, Size maxSize, double scaling, int stride){

	// Initialize window size:

	Size windowSize = minSize;
	Mat imageRoi;
	Rect roi;
	bool rowCheck, colCheck;


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
				
				// If region contains a detection, append to detections:

				//if(detection){detections.push_back(roi);

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
