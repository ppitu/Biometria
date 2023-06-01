#include "opencv2/core.hpp"
#include "opencv2/core/cvstd_wrapper.hpp"
#include "opencv2/face.hpp"
#include "opencv2/face/facerec.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void read_cvs(const std::string& filename, std::vector<cv::Mat>& images, std::vector<int>& lables, char sep = ';') {
  std::ifstream file(filename.c_str(), std::ifstream::in);

  std::string line, path, classlabel;

  while(std::getline(file, line)) {
    std::stringstream liness(line);
    std::getline(liness, path, sep);
    std::getline(liness, classlabel);
    if(!path.empty() && !classlabel.empty()) {
      images.emplace_back(cv::imread(path, 0));
      lables.emplace_back(std::atoi(classlabel.c_str()));
    }
  }
}

int main(int argc, const char* argv[]) {
  std::string output_folder = ".";

  if(argc == 3) {
    output_folder = std::string(argv[2]);
  }

  std::string cvs_path = std::string(argv[1]);

  std::vector<cv::Mat> images;
  std::vector<int> labels;

  read_cvs(cvs_path, images, labels);

  auto height = images[0].rows;

  cv::Mat test_sample = images[images.size() - 1];
  int test_label = labels[images.size() - 1];
  images.pop_back();
  labels.pop_back();

  cv::Ptr<cv::face::FisherFaceRecognizer> model = cv::face::FisherFaceRecognizer::create();
  model->train(images, labels);

  auto predicted_label = model->predict(test_sample);

  auto eigenvalues = model->getEigenValues();
  auto w = model->getEigenVectors();
  auto mean = model->getMean();

  imwrite(cv::format("%s/mean.png", output_folder.c_str()), mean.reshape(1, images[0].rows));
  //cv::imshow("mean", mean.reshape(1, images[0].rows));
}


