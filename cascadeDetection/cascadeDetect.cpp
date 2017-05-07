#include "cascadeDetect.hpp"

void* cascadeDetect(ctx* context) {

	sleep(0.5);

	// Toggle detections:

	bool detectCoidals = 1;
	bool detectHeads = 1;
	bool detectDorsals = 1;

	// Load cascades:

	Ptr<cv::cuda::CascadeClassifier> coidal_cascade = cv::cuda::CascadeClassifier::create("current_coidal.xml");
	Ptr<cv::cuda::CascadeClassifier> head_cascade = cv::cuda::CascadeClassifier::create("current_head.xml");
	Ptr<cv::cuda::CascadeClassifier> dorsal_cascade = cv::cuda::CascadeClassifier::create("current_dorsal.xml");

	// Choose detection parameters:

	coidal_cascade->setMinNeighbors(5);
	head_cascade->setMinNeighbors(2);
	dorsal_cascade->setMinNeighbors(2);

	// Toggle writer:

	//VideoWriter writer = VideoWriter("libvlc_out.avi", CV_FOURCC('X', 'V', 'I', 'D'), 15, Size(1920, 1080), true);

	// Initializing variables:

	cv::Mat cpuFrame;

	Mat tmp;

	cv::cuda::GpuMat gpuFrame;

	cv::cuda::GpuMat objbuf;

	while(true){

		// Aquire image data:

		context->imagemutex->lock();

		tmp = *context->image;	

		cpuFrame = tmp.clone();

		context->imagemutex->unlock();

		gpuFrame.upload(cpuFrame);

		cv::cuda::cvtColor(gpuFrame, gpuFrame, CV_BGR2GRAY);

		// Initialize ROI vectors:

		std::vector<Rect> fishHeads;
		std::vector<Rect> coidalFins;
		std::vector<Rect> dorsalFins;

		// Head detection:

		if(detectHeads){

			head_cascade->detectMultiScale(gpuFrame, objbuf);
			
			head_cascade->convert(objbuf, fishHeads);

		}

		// Coidal fin detection:

		if(detectCoidals){

			coidal_cascade->detectMultiScale(gpuFrame, objbuf);

			coidal_cascade->convert(objbuf, coidalFins);

		}

		// Dorsal fin detection:

		if(detectDorsals){

			dorsal_cascade->detectMultiScale(gpuFrame, objbuf);

			dorsal_cascade->convert(objbuf, dorsalFins);

		}

		// Draw fish heads:

		if(!fishHeads.empty() && detectHeads){

			for(int i = 0; i < static_cast<int>(fishHeads.size()); i++){

				cv::rectangle(cpuFrame, fishHeads[i], cv::Scalar(70, 212, 37));

			}
		}

		// Draw coidal fins:

		if (!coidalFins.empty() && detectCoidals){ 

			for(int i = 0; i < static_cast<int>(coidalFins.size()); i++){

				cv::rectangle(cpuFrame, coidalFins[i], cv::Scalar(49, 37, 212));

			}
		}

		// Draw dorsal fins:

		if(!dorsalFins.empty() && detectDorsals){

			for(int i = 0; i < static_cast<int>(dorsalFins.size()); i++){

				cv::rectangle(cpuFrame, dorsalFins[i], cv::Scalar(181, 38, 213));
				
			}
		}
		
		// Toggle writer:

		//writer.write(cpuFrame);

		// Show results:

		imshow("Display", cpuFrame);	

		waitKey(3);
	
	}

	gpuFrame.release();
	objbuf.release();	

}
