#include "reco.h"
#include <iostream>
#include "face.hpp"
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include <QPaintEvent>
#include <QPainter>
#include <QTimerEvent>
#include <QtDebug>
#include <QPushButton>
#include "QDialog"
#include <QMessageBox>
#include <QLineEdit>
#include <QDebug>
#include <opencv2/dnn.hpp>

using namespace std;
using namespace cv;

reco::reco() {

}

RNG reco:: g_rng(12345);

Ptr<face::FaceRecognizer> reco:: model;

int reco::Predict(Mat src_image)  //识别图片
{
    Mat face_test;
    int predict = 0;
    //截取的ROI人脸尺寸调整
    if (src_image.rows >= 120)
    {
        //改变图像大小，使用双线性差值
        resize(src_image, face_test, Size(92, 112));

    }
    //判断是否正确检测ROI
    if (!face_test.empty())
    {
        //测试图像应该是灰度图
        predict = model->predict(face_test);
    }
    cout << predict << endl;
    return predict;
}

void reco::fontSet(cv::Mat &img, QString text, int x,int y, QFont font, QPen pen){
        QImage::Format imgFormat;
        QPoint org=QPoint(x,y);

        switch (img.channels()) {
        case 1:
            imgFormat = QImage::Format_Grayscale8;
            break;
        case 3:
            imgFormat = QImage::Format_RGB888; // 没有BGR格式，因此要注意一下pen的color
            break;
        default:
            qDebug()<<"font_error";
            return;
            break;
        }

        // 直接共享了Mat的内存，不存在内存复制。所以应该不怎么耗时
        QImage tmpImg(img.data, img.cols, img.rows, img.step, imgFormat);

        QPainter painter(&tmpImg);
        painter.setPen(pen);
        painter.setFont(font);
        painter.drawText(org, text);

        return;

}


int reco::recognize(string name)
{
    VideoCapture cap(0);    //打开默认摄像头
    if (!cap.isOpened())
    {
        return -1;
    }
    Mat frame;
    Mat gray;
    //这个分类器是人脸检测所用
    CascadeClassifier cascade;
    bool stop = false;
    //训练好的文件名称，放置在可执行文件同目录下
    cascade.load("../../haarcascade_frontalface_alt2.xml");
    model = face::FisherFaceRecognizer::create();
    //1.加载训练好的模型
    model->read("MyFacePCAModel.xml");// 加载模型
    /*cv::dnn::Net net = cv::dnn::readNetFromONNX("../../best.onnx");*/  // 加载训练好的识别模型
    while (1)
    {
        cap >> frame;

        vector<Rect> faces(0);//建立用于存放人脸的向量容器

        cvtColor(frame, gray, COLOR_RGB2GRAY);//测试图像必须为灰度图

        equalizeHist(gray, gray); //变换后的图像进行直方图均值化处理
        //检测人脸
        cascade.detectMultiScale(gray, faces,1.1, 4, 0| CASCADE_DO_ROUGH_SEARCH,Size(30, 30), Size(500, 500));
        Mat* pImage_roi = new Mat[faces.size()];
        Mat face;
        Point text_lb;//文本
        QPen pen;
        pen.setColor(QColor(255, 255, 255));
        QFont font;
        font.setPixelSize(32);
        QString qstr;
        //框出人脸
        string str;
        for (int i = 0; i < faces.size(); i++)
        {
            pImage_roi[i] = gray(faces[i]); //将所有的脸部保存起来
            // text_lb = Point(faces[i].x, faces[i].y);
            // cv::Mat blob = cv::dnn::blobFromImage(pImage_roi[i]);
            // net.setInput(blob);
            // cv::Mat predict = net.forward();
            // cout<<predict;
            if (pImage_roi[i].empty())
                continue;
            // cout<<Predict(pImage_roi[i]);
            switch (Predict(pImage_roi[i])) //对每张脸都识别
            {
            case 66:str = name; qstr = QString::fromStdString(str);break;
            default: str = "Error"; break;
            }
            Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));//所取的颜色任意值
            rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), color, 1, 8);//放入缓存
            // putText(frame, str, text_lb, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));//添加文字
            fontSet(frame, qstr, faces[i].x, faces[i].y,font, pen);
        }

        delete[]pImage_roi;
        imshow("face", frame);
        waitKey(200);
    }

    return 0;

    cout << "Hello World!" << endl;
    return 0;
}
