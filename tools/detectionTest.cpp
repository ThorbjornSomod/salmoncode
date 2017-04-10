#include "detectionTest.hpp"

void resizeUsingOpenCvCpu(std::string input_file, std::string output_file) {

	cv::Mat input = cv::imread(input_file, IMREAD_COLOR);

	if (input.empty()) {

		std::cout << "Image not found: " << input_file << std::endl;
		return;

	}

	cv::Mat output;

	clock_t begin = clock();

	cv::resize(input, output, Size(), 0.25, 0.25, cv::INTER_AREA);

	clock_t end = clock();

	double elapsed = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << "Time to resize using CPU: " << elapsed << std::endl;

	cv::imwrite(output_file, output);

}

void resizeUsingOpenCvGpu(std::string input_file, std::string output_file) {

	cv::Mat inputCpu = cv::imread(input_file, IMREAD_COLOR);

	cv::cuda::GpuMat input (inputCpu);

	if (input.empty()) {

		std::cout << "Image not found: " << input_file << std::endl;
		return;

	}

	cv::cuda::GpuMat output;

	clock_t begin = clock();

	cv::cuda::resize(input, output, Size(), 0.25, 0.25, INTER_AREA);

	clock_t end = clock();

	double elapsed = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << "Time to resize using GPU: " << elapsed << std::endl;

	cv::Mat outputCpu;

	output.download(outputCpu);

	cv::imwrite(output_file, outputCpu);

	input.release();

	output.release();

}

void changeColorSpaceOfImageUsingGpu(std::string input_file, std::string output_file) {

	cv::Mat inputCpu = cv::imread(input_file, IMREAD_COLOR);

	cv::cuda::GpuMat input(inputCpu);

	if (input.empty()) {

		std::cout << "Image not found: " << input_file << std::endl;
		return;

	}

	cv::cuda::GpuMat output;

	clock_t begin = clock();

	cv::cuda::cvtColor(input, output, CV_BGR2HSV);

	clock_t end = clock();

	double elapsed = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << "Time to change color space using GPU: " << elapsed << std::endl;

	cv::Mat outputCpu;

	output.download(outputCpu);

	cv::imwrite(output_file, outputCpu);

	input.release();

	output.release();

}

void normalizeImageUsingGpu(){

	cv::Mat input_image;

	cv::Mat output_image;

	for(int i = 1; i < 467; i++){

		input_image = cv::imread("/media/sealab/My Passport/lbpcudacascade/training_images/pos/grayscale/pos" + to_string(i) + ".png", -1);

		//cv::cuda::GpuMat gpu;

		//gpu.upload(input_image);

		//cv::cuda::equalizeHist(gpu, gpu);

		normalize(input_image, output_image, 50, 0, NORM_L2, -1, Mat());

		//gpu.download(output_image);

		cv::imwrite("/media/sealab/My Passport/lbpcudacascade/training_images/pos/grayscale_norm_l2/pos" + to_string(i) + ".png", output_image);

		//gpu.release();

	}

}

void imageConverter(){

	cv::Mat input_image;

	cv::Mat output_image;

	cv::cuda::GpuMat gpu;

	for(int i = 1; i < 467; i++){

		//input_image = cv::imread("/home/sealab/lbpcudacascade/training_images/pos/color/pos (" + to_string(i) + ").png", -1);

		//gpu.upload(input_image);

		//cv::cuda::cvtColor(gpu, gpu, CV_BGR2GRAY);

		//gpu.download(output_image);

		//cv::imwrite("/home/sealab/lbpcudacascade/training_images/pos/color/pos" + to_string(i) + ".png", output_image);

		std::string oldname  = "/home/sealab/lbpcudacascade/training_images/val/val (" + to_string(i) + ").png";

		std::string newname  = "/home/sealab/lbpcudacascade/training_images/val/val" + to_string(i) + ".png";
		
		const char* a = oldname.c_str();

		const char* b = newname.c_str();

		std::rename(a, b);

	}

}


void changeColorSpaceOfVideoUsingGpu(std::string input_file, std::string output_file) {

	double FPS = 50;

	cv::Ptr<cv::cudacodec::VideoReader> reader = cv::cudacodec::createVideoReader(input_file);

	cv::cudacodec::FormatInfo format = reader->format();

	//cv::Ptr<cv::cudacodec::VideoWriter> writer = cv::cudacodec::createVideoWriter(output_file, cv::Size(format.width, format.height), FPS);

	cv::cuda::GpuMat gpu_frame;

	cv::namedWindow("Face", CV_WINDOW_OPENGL);

	cv::resizeWindow("Face", format.width, format.height);

	cv::setWindowProperty("Face", CV_WND_PROP_ASPECTRATIO, CV_WINDOW_KEEPRATIO);

	for (;;) {

		if (!reader->nextFrame(gpu_frame)) {

			std::cout << "There is no frame." << std::endl;

			break;

		}

		cv::cuda::cvtColor(gpu_frame, gpu_frame, CV_BGR2HSV);

		cv::imshow("Hitrasalmon", gpu_frame);

		//writer->write(gpu_frame);

		cv::waitKey(20);

		gpu_frame.release();

	}
}

void detectionTest(std::string cascade_name) {

	Ptr<cv::cuda::CascadeClassifier> gpu_cascade = cv::cuda::CascadeClassifier::create(cascade_name);

	//gpu_cascade->setFindLargestObject(true);
	//gpu_cascade->setScaleFactor(1.1);
	gpu_cascade->setMinNeighbors(8);
	//gpu_cascade->setMinObjectSize(cv::Size(25,35));

	//cv::VideoCapture reader(0);
	cv::VideoCapture reader("rtsp://admin:ral1004@192.168.1.6:2020/videoinput_1/h264_1/media.stm");
	//cv::VideoCapture reader("testing.mp4");
	
	//cv::VideoWriter writer("gpu_detection.avi", CV_FOURCC('X', 'V', 'I', 'D'), 50, cv::Size(1920, 1080), true);

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

		gpu_cascade->detectMultiScale(gpu_frame, objbuf);

		std::vector<Rect> objects;

		gpu_cascade->convert(objbuf, objects);

		if (!objects.empty()) 

			//std::cout << objects.size() << std::endl;

			for (int i = 0; i < static_cast<int>(objects.size()); i++) {

				cv::rectangle(cpu_frame, objects[i], cv::Scalar(255, 255, 255));

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
