#include "cascadeDetect.hpp"

void cascadeDetect(std::string video_source, std::string coidal_cascade_name, std::string head_cascade_name, std::string dorsal_cascade_name) {

	// Toggle detections:

	bool detectCoidals = 1;
	bool detectHeads = 1;
	bool detectDorsals = 1;

	// Load cascades:

	Ptr<cv::cuda::CascadeClassifier> coidal_cascade = cv::cuda::CascadeClassifier::create(coidal_cascade_name);
	Ptr<cv::cuda::CascadeClassifier> head_cascade = cv::cuda::CascadeClassifier::create(head_cascade_name);
	Ptr<cv::cuda::CascadeClassifier> dorsal_cascade = cv::cuda::CascadeClassifier::create(dorsal_cascade_name);

	// Choose detection parameters:

	coidal_cascade->setMinNeighbors(2);
	head_cascade->setMinNeighbors(2);
	dorsal_cascade->setMinNeighbors(2);

	// Choose video input:

	cv::VideoCapture reader(video_source);

	// Toggle writing to file:
	
	//cv::VideoWriter writer("gpu_detection.avi", CV_FOURCC('X', 'V', 'I', 'D'), 25, cv::Size(1920, 1080), true);

	if (!reader.isOpened()) {

		//std::cout << "Camera could not be opened." << std::endl;
		std:: cout << "File could not be opened." << std::endl;

	}

	cv::Mat cpu_frame;

	cv::cuda::GpuMat gpu_show;

	cv::namedWindow("Detections", WINDOW_OPENGL);

	cv::resizeWindow("Detections", 1920, 1080);

	for (;;) {

		reader >> cpu_frame;

		if(cpu_frame.empty()){

			break;

		}

		cv::cuda::GpuMat gpu_frame(cpu_frame);

		cv::cuda::GpuMat objbuf;

		cv::cuda::cvtColor(gpu_frame, gpu_frame, CV_BGR2GRAY);

		std::vector<Rect> fish_heads;
		std::vector<Rect> coidal_fins;
		std::vector<Rect> dorsal_fins;

		// Head detection:

		if(detectHeads){

			head_cascade->detectMultiScale(gpu_frame, objbuf);
			
			head_cascade->convert(objbuf, fish_heads);

		}

		// Coidal fin detection:

		if(detectCoidals){

			coidal_cascade->detectMultiScale(gpu_frame, objbuf);

			coidal_cascade->convert(objbuf, coidal_fins);

		}

		// Dorsal fin detection:

		if(detectDorsals){

			dorsal_cascade->detectMultiScale(gpu_frame, objbuf);

			dorsal_cascade->convert(objbuf, dorsal_fins);

		}

		// Draw fish heads:

		if(!fish_heads.empty()){

			for(int i = 0; i < static_cast<int>(fish_heads.size()); i++){

				cv::rectangle(cpu_frame, fish_heads[i], cv::Scalar(70, 212, 37));
	
			}
		}

		// Draw coidal fins:

		if (!coidal_fins.empty()){ 

			for(int i = 0; i < static_cast<int>(coidal_fins.size()); i++){

				cv::rectangle(cpu_frame, coidal_fins[i], cv::Scalar(49, 37, 212));

			}
		}

		// Draw dorsal fins:

		if(!dorsal_fins.empty()){

			for(int i = 0; i < static_cast<int>(dorsal_fins.size()); i++){

				cv::rectangle(cpu_frame, dorsal_fins[i], cv::Scalar(181, 38, 213));

			}
		}

		//writer.write(cpu_frame);

		gpu_show.upload(cpu_frame);

		cv::imshow("Detections", gpu_show);

		gpu_frame.release();
		objbuf.release();

		if ((char)27 == (char)waitKey(10)) {

			break;

		}
	}
}
