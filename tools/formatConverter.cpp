#include "formatConverter.hpp"

void readAnnotationFile(string filename){

	ifstream file;

	file.open(filename);

	vector<vector<Rect>> vvr;

	Mat img;

	Mat img_r;

	int c = 0;

	for(string line; getline(file, line);){

		istringstream iss(line);

		string word;

		iss >> word;

		string filename_img = word;
		
		img = imread("/home/sealab/lbpcudacascade/training/pos/" + filename_img, IMREAD_UNCHANGED);
	
		iss >> word;

		int obj = stoi(word);

		vector<Rect> vr;

		for(int i = 0; i < obj; i++){

			int x;
			int y;
			int delta_x;
			int delta_y;
			
			iss >> word;
			x = stoi(word);

			iss >> word;
			y = stoi(word);

			iss >> word;
			delta_x = stoi(word);

			iss >> word;
			delta_y = stoi(word);

			Rect r(x, y, delta_x, delta_y);

			img_r = img(r);

			imwrite("/home/sealab/svmlearner/training/classImages/coidal/coidal" + to_string(c) + "_" + to_string(i) + ".jpg", img_r);

			vr.push_back(r);

		}
		
		c++;

		vvr.push_back(vr);

	}

	/***********************************************************/

	vector<vector<Rect>>::iterator vvr_iterator;
	vector<Rect>::iterator vr_iterator;

	for(vvr_iterator = vvr.begin(); vvr_iterator != vvr.end(); ++vvr_iterator){

		for(vr_iterator = (*vvr_iterator).begin(); vr_iterator != (*vvr_iterator).end(); ++vr_iterator){

			cout << *vr_iterator << "\t";

		}

		cout << endl;

	}

	file.close();

}
