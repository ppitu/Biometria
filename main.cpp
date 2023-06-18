#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace cv::face;
using namespace std;

static Mat norm_0_255(InputArray _src) {
	Mat src = _src.getMat();
	Mat dst;
	switch(src.channels()) {
		case 1:
			cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
			break;
		case 3:
			cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
			break;
		default:
			src.copyTo(dst);
			break;
	}
	return dst;
}

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(Error::StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if(!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

int main(int argc, const char *argv[]) {
	
	if (argc < 2) {
		cout << "usage: " << argv[0] << " <csv.ext> <test_image>" << endl;
		exit(1);
	}

	string fn_csv = string(argv[1]);
	vector<Mat> images;
	vector<int> labels;
	
	try {
		read_csv(fn_csv, images, labels);
	} catch (const cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		exit(1);
	}

	if(images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(Error::StsError, error_message);
	}

	int height = images[0].rows;

	Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
	
	model->train(images, labels);
	string testImagePath = string(argv[2]);
	
	auto testImage = imread(testImagePath, 0);

	int pred = 0;
	double conf = 0.0;
	
	model->predict(testImage, pred, conf);

	string result_message = format("Pred class = %d, loss = %f", pred, conf);
	cout << result_message << endl;

	return 0;
}
