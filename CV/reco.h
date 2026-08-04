#ifndef RECO_H
#define RECO_H

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
using namespace std;
using namespace cv;


class reco
{
public:
    reco();
    static int recognize(string);
    static int Predict(Mat);
    static void fontSet(cv::Mat&,QString,int,int,QFont,QPen);
    static RNG g_rng;
    static Ptr<face::FaceRecognizer> model;

};

#endif // RECO_H
