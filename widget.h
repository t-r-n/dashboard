#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPushButton>
#include<string>
#include<QString>
#include<QDebug>
#include<QScrollArea>
#include<QVBoxLayout>
#include<QScrollBar>
#include<QLabel>
#include<QHBoxLayout>
#include<QSlider>
#include<QColorDialog>
#include"dashboard.h"
#include<QGroupBox>
#include<QGridLayout>
#include<QTextLine>
#include<QTextEdit>
#include<QDialog>
#include<QMessageBox>
#include"valueserver.h"
#include<QCheckBox>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
signals:
    void valueChanged(int val);
private:
    ValueServer*server;
    QTextEdit*rangeLineL;
    QTextEdit*rangeLineR;
    QTextEdit*numberLine1;
    QTextEdit*numberLine2;
    QTextEdit*angleLine1;
    QTextEdit*angleLine2;
    QTextEdit*curValueLine;
    QGridLayout*parLayout;
    DashBoard*dashBoard;
    QCheckBox* checkBox;
    Ui::Widget *ui;
};
#endif // WIDGET_H
