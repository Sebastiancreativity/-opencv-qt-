#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <qpushbutton.h>
#include <QLineEdit>
#include <string>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void startPhoto();
    void startTrain();
    void startReco();
    void nameRecord(const QString&);
    QPushButton * btn1 = new QPushButton;
    QPushButton * btn2 = new QPushButton;
    QPushButton * btn3 = new QPushButton;
    QLineEdit *lineEdit1 = new QLineEdit;
    ~Widget();
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
