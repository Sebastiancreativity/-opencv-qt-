#include "widget.h"
#include "photo.h"
#include "train.h"
#include "reco.h"
#include "./ui_widget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimerEvent>
#include <QtDebug>
#include <QPushButton>
#include "QDialog"
#include <QMessageBox>
#include <QLineEdit>
#include <string>
#include <iostream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("人脸识别演示程序");
    this->resize(400,600);
    this->setMinimumSize(400,600);
    this->setMaximumSize(400,600);
    btn1->setParent(this);
    btn1->setGeometry(85,250,75,30);
    btn1->setText("采集人脸");
    btn1->setEnabled(false);
    btn2->setParent(this);
    btn2->setGeometry(160,250,75,30);
    btn2->setText("开始训练");
    btn2->setEnabled(false);
    btn3->setParent(this);
    btn3->setGeometry(235,250,75,30);
    btn3->setText("开始识别");
    btn3->setEnabled(false);
    lineEdit1->setEchoMode(QLineEdit::Normal);
    lineEdit1->setPlaceholderText("输入你的名字，麻溜点");
    lineEdit1->setClearButtonEnabled(true);
    lineEdit1->setGeometry(85,325,225,50);
    lineEdit1->setParent(this);
    connect(btn1, &QPushButton::clicked, this, &Widget::startPhoto);
    connect(btn2, &QPushButton::clicked, this, &Widget::startTrain);
    connect(btn3, &QPushButton::clicked, this, &Widget::startReco);
    connect(lineEdit1,&QLineEdit::textChanged ,this, &Widget::nameRecord);
    // this->startTimer(50);
}

string name;

Widget::~Widget()
{
    delete ui;
}

void Widget::startPhoto(){
    qDebug() << "StartPhoto";
    if(QMessageBox::Yes == QMessageBox::question(this,"提示","按下YES开始拍照，按下NO取消拍照", QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
    {
        photo::takePhoto();
        QMessageBox::information(this, "info", "采集成功");
        btn2->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this, "info", "整理整理再来吧");
    }
}

void Widget::startTrain(){
    qDebug() << "StartTrain";
    starttrain("../../source/train_faces/at.txt");
    btn3->setEnabled(true);
}

void Widget::startReco(){
    qDebug() << "StartReco";
    reco::recognize(name);
    btn3->setEnabled(true);
}

void Widget::nameRecord(const QString &text){
    name = text.toStdString();
    btn1->setEnabled(true);
}

void Widget::paintEvent(QPaintEvent *event)
{
    //初始页面
        QPainter painter(this);
        painter.setPen(QColor(0,0,0));
        painter.setFont(QFont("Arial", 15,700,true));
        painter.drawText(60,200, "这是一个简单的人脸识别系统。");
        painter.drawText(60,230, "   采集人脸后方可进行识别");


}
