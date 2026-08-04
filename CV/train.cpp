#include "train.h"
#include <iostream>
#include "face.hpp"
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include <opencv2\dnn.hpp>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace dnn;

Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // 创建和返回一个归一化后的图像矩阵:
    Mat dst;
    switch (src.channels()) {
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

void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    ifstream file(filename.c_str(), ifstream::in);
    if (!file)
    {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) //从文本中读取分隔符
    {
        stringstream liness(line);//分割字符串
        getline(liness, path, separator);//读入路径
        getline(liness, classlabel);//读入标签
        if (!path.empty() && !classlabel.empty()) //PUSH!!!
        {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

void starttrain(string file_path){
    //读取训练集路径
    string fn_csv = file_path;

    // 2个容器来存放图像数据和对应的标签
    vector<Mat> images;
    vector<int> labels;
    // 读取数据. 如果文件不合法就会出错
    // 输入的文件名已经有了.
    try
    {
        read_csv(fn_csv, images, labels);
        Mat img;        //从csv文件中批量读取训练数据
        Mat dst;
        for(int i=0;i<150;i++){
            string filename = format("../../source/train_faces/asia/%d.jpg", i+1);
            img = imread(filename, IMREAD_GRAYSCALE);
            cv::resize(img,dst,Size(92,112));
            images.push_back(dst); // 将图片添加到images中
        }

        for(int i=0;i<150;i++){
            labels.push_back(100+i);
        }

        for(int i=0;i<10;i++){
            string filename = format("../../source/photo_faces/%d.jpg", i+1);
            img = imread(filename, IMREAD_GRAYSCALE);
            images.push_back(img); // 将图片添加到images中
        }

        for(int i=0;i<10;i++){
            labels.push_back(66);
        }
    }
    catch (cv::Exception& e)
    {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;//文件有问题
        exit(1);
    }
    // 图片太少，不训练
    if (images.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(Error::StsBadArg, error_message);
    }

    for (int i = 0; i < images.size(); i++)
    {
        cout<<images.size();
        if (images[i].size() != Size(92, 112))
        {
            cout << i << endl;
            cout << images[i].size() << endl;
        }

    }

    // 简单地移出每一个文件夹的最后一张图片作为测试图片
    Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();//删除最后一张照片，此照片作为测试图片
    labels.pop_back();//删除最有一张照片的labels

    //三种训练方法，三个模型
    Ptr<face::BasicFaceRecognizer> model = face::EigenFaceRecognizer::create();
    model->train(images, labels);
    model->save("MyFacePCAModel.xml"); //这一种是基础模型

    Ptr<face::BasicFaceRecognizer> model1 = face::FisherFaceRecognizer::create();
    model1->train(images, labels);
    model1->save("MyFaceFisherModel.xml"); //这一种相对第一种，对姿态及光照更有优势

    Ptr<face::LBPHFaceRecognizer> model2 = face::LBPHFaceRecognizer::create();
    model2->train(images, labels);
    model2->save("MyFaceLBPHModel.xml"); //尤其针对于光照




    // 下面对测试图像进行预测，predictedLabel是预测标签结果
    //注意predict()入口参数必须为单通道灰度图像，如果图像类型不符，需要先进行转换
    //predict()函数返回一个整形变量作为识别标签
    int predictedLabel = model->predict(testSample);//加载分类器
    int predictedLabel1 = model1->predict(testSample);
    int predictedLabel2 = model2->predict(testSample);

    // 还有一种调用方式，可以获取结果同时得到阈值:
    // int predictedLabel = -1;
    // double confidence = 0.0;
    //  model->predict(testSample, predictedLabel, confidence);

    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
    string result_message1 = format("Predicted class = %d / Actual class = %d.", predictedLabel1, testLabel);
    string result_message2 = format("Predicted class = %d / Actual class = %d.", predictedLabel2, testLabel);
    cout << result_message << endl;
    cout << result_message1 << endl;
    cout << result_message2 << endl;

    getchar();
    //waitKey(0);
}
