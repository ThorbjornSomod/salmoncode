#include "tools.hpp"

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

	for(int i = 1; i < 4172; i++){

		//input_image = cv::imread("/home/sealab/lbpcudacascade/training_images/pos/color/pos (" + to_string(i) + ").png", -1);

		//gpu.upload(input_image);

		//cv::cuda::cvtColor(gpu, gpu, CV_BGR2GRAY);

		//gpu.download(output_image);

		//cv::imwrite("/home/sealab/lbpcudacascade/training_images/pos/color/pos" + to_string(i) + ".png", output_image);

		std::string oldname  = "/home/sealab/lbpcudacascade/training/neg/neg (" + to_string(i) + ").jpg";

		std::string newname  = "/home/sealab/lbpcudacascade/training/neg/neg" + to_string(i) + ".jpg";
		
		const char* a = oldname.c_str();

		const char* b = newname.c_str();

		std::rename(a, b);

	}

}


void changeColorSpaceOfVideoUsingGpu(std::string input_file, std::string output_file) {

	//double FPS = 50;

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

void getRandomNegatives(){

	Size negSize(30,30);

	srand(time(NULL));

	Mat img;
	
	Mat imgRoi;

	int r, x, y;

	Rect roi;

	for(int i = 2755; i < 5000; i++){

		r = rand() % 4172;

		cout << r << endl;

		img = imread("/home/sealab/lbpcudacascade/training/neg/neg" + to_string(r) + ".jpg");

		x = rand() % (img.cols - negSize.width -10);

		y = rand() % (img.rows - negSize.height -10);

		roi = Rect(x, y, negSize.width, negSize.height);

		imgRoi = img(roi);

		imwrite("/home/sealab/svmlearner/training/neg/back2/back" + to_string(i + 1) + ".png", imgRoi);

	}
}
