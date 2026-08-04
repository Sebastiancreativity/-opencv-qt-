#include "photo.h"
#include <Windows.h>
#include <QDebug>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <vector>
#include<stdio.h>
using namespace std;
using namespace cv;

photo::photo() {}

void photo::takePhoto(){
    CascadeClassifier train;
    train.load("haarcascade_frontalface_alt2.xml"); //使用官方训练器
    VideoCapture cap(0);  //电脑自带摄像头
    Mat frame, train_face;
    int pic_num = 1;
    while (1) {
        //摄像头读图像
        cap >> frame;
        vector<Rect> faces;//vector容器存检测到的faces
        Mat frame_gray;

        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);//转灰度化，减少运算

        train.detectMultiScale(frame_gray, faces, 1.1, 4, CASCADE_DO_ROUGH_SEARCH, Size(70, 70), Size(1000, 1000));
        printf("检测到人脸个数：%d\n", faces.size());

        //识别到的脸用矩形圈出
        for (int i = 0; i < (int)faces.size(); i++)
        {
            rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);
        }
        //当只有一个人脸时，开始拍照
        if (faces.size() == 1)
        {
            Mat faceROI = frame_gray(faces[0]);//在灰度图中将圈出的脸所在区域裁剪出
            //cout << faces[0].x << endl;//测试下face[0].x
            resize(faceROI, train_face, Size(92, 112));//将兴趣域size为92*112
            putText(frame, to_string(pic_num), faces[0].tl(), 3, 1.2, (0, 0, 225), 2, 0);//在 faces[0].tl()的左上角上面写序号
            string filename = format("../../source/photo_faces/%d.jpg", pic_num);            //图片的存放位置
            qDebug()<<filename;
            imwrite(filename, train_face);//存在当前目录下
            imshow(filename, train_face);//显示下size后的脸
            waitKey(500);//等待500us
            destroyWindow(filename);//:销毁指定的窗口
            pic_num++;//序号加1
            if (pic_num == 11)
            {
                return;//当序号为11时退出循环,一共拍10张照片
            }
        }
        int c = waitKey(10);
        if ((char)c == 27) { break; } //10us内输入esc则退出循环
        imshow("frame", frame);//显示视频流
        waitKey(100);//等待100us
    }
    return;
}
