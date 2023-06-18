#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
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
	
	if (argc < 3) {
		cout << "usage: " << argv[0] << " <csv.ext> <output_folder> <test_image>" << endl;
		exit(1);
	}

	string output_folder = ".";
	
	if (argc == 4) {
		output_folder = string(argv[2]);
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
	//Mat testSample = images[images.size() - 1];
	//int testLabel = labels[labels.size() - 1];
	
	//images.pop_back();
	//labels.pop_back();
	Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
	
	model->train(images, labels);
	
	int predictedLabel = model->predict(testSample);
	
	string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
	cout << result_message << endl;

	
	
	//std::string str = format("Class: %d", predictedLabel);
	
	//cv::putText(testSample, str, cv::Point(10, testSample.rows / 2), cv::FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(118, 185, 0), 2);
	//cv::imwrite(format("%s/predic.png", output_folder.c_str()), testSample);
	
	/*Mat eigenvalues = model->getEigenValues();
	Mat W = model->getEigenVectors();
	Mat mean = model->getMean();
	
	if(argc == 2) {
		imshow("mean", norm_0_255(mean.reshape(1, images[0].rows)));
	} else {
		imwrite(format("%s/mean.png", output_folder.c_str()), norm_0_255(mean.reshape(1, images[0].rows)));
	}
	
	for (int i = 0; i < min(16, W.cols); i++) {
		string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
		cout << msg << endl;
		Mat ev = W.col(i).clone();
		Mat grayscale = norm_0_255(ev.reshape(1, height));
		Mat cgrayscale;
		applyColorMap(grayscale, cgrayscale, COLORMAP_BONE);
		if(argc == 2) {
			imshow(format("fisherface_%d", i), cgrayscale);
		} else {
			imwrite(format("%s/fisherface_%d.png", output_folder.c_str(), i), norm_0_255(cgrayscale));
		}
	}

	for(int num_component = 0; num_component < min(16, W.cols); num_component++) {
		Mat ev = W.col(num_component);
		Mat projection = LDA::subspaceProject(ev, mean, images[0].reshape(1,1));
		Mat reconstruction = LDA::subspaceReconstruct(ev, mean, projection);
		reconstruction = norm_0_255(reconstruction.reshape(1, images[0].rows));
		if(argc == 2) {
			imshow(format("fisherface_reconstruction_%d", num_component), reconstruction);
		} else {
			imwrite(format("%s/fisherface_reconstruction_%d.png", output_folder.c_str(), num_component), reconstruction);
		}
		}*/

	return 0;
}
