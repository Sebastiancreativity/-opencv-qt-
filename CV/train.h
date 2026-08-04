#ifndef TRAIN_H
#define TRAIN_H
#include <iostream>
#include "face.hpp"
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;
using namespace cv;


static Mat norm_0_255(InputArray _src);
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator);

void starttrain(string);

#endif // TRAIN_H
