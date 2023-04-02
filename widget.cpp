#include "widget.h"
#include "ui_widget.h"
#if _MSC_VER >= 1600	// MSVC2015 > 1899,	MSVC_VER = 14.0
#pragma execution_character_set("utf-8")
#endif
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{//提供端口和串口通信两种方式
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    this->setMinimumSize(800,630);//避免: setGeometry: Unable to set geometry
    dashBoard=new DashBoard();//自定义控件也要分配到堆区要不出了构造函数作用域析构掉了

/*
    this->setStyleSheet("QWidget{"
                        "background-color:black;"
                        "}"
                        "QPushButton{"
                        "color:white;"
                        "}");
*/
/* 颜色管理 */
    QPushButton *buttonOutCircle      =new QPushButton("外圆");//外圆
    QPushButton *buttonInnerCircle    =new QPushButton("内圆");//内圆
    QPushButton *buttonCoverCircle    =new QPushButton("覆盖圆");//覆盖圆
    QPushButton* buttonScale          =new QPushButton("刻度尺");//刻度尺
    QPushButton* buttonPointer        =new QPushButton("指针");//指针
    QPushButton* buttonPieColorStart  =new QPushButton("起始饼圆");//起始饼圆
    QPushButton* buttonPieColorMid    =new QPushButton("中间饼圆");//中间饼圆
    QPushButton* buttonPieColorEnd    =new QPushButton("结束饼圆");//结束饼圆
    QPushButton* buttonOverlay        =new QPushButton("遮罩层");//遮罩层
    QPushButton* buttonCenterCircle   =new QPushButton("中心圆");//中心圆
    QPushButton* buttonText           =new QPushButton("中心字");//中心字

    QGridLayout *gColorLayout = new QGridLayout();
    gColorLayout->addWidget(buttonOutCircle,0,0);
    gColorLayout->addWidget(buttonInnerCircle,0,1);
    gColorLayout->addWidget(buttonCoverCircle,0,2);
    gColorLayout->addWidget(buttonScale,1,0);
    gColorLayout->addWidget(buttonPointer,1,1);
    gColorLayout->addWidget(buttonPieColorStart,1,2);
    gColorLayout->addWidget(buttonPieColorMid,2,0);
    gColorLayout->addWidget(buttonPieColorEnd,2,1);
    gColorLayout->addWidget(buttonOverlay,2,2);
    gColorLayout->addWidget(buttonCenterCircle,3,0);
    gColorLayout->addWidget(buttonText,3,1);
    QGroupBox *gColorGroup=new QGroupBox("颜色管理");
    gColorGroup->setLayout(gColorLayout);
    connect(buttonOutCircle,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setOuterCircleColor(color);
        }

    });
    connect(buttonInnerCircle,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setInnerCircleColor(color);
        }

    });
    connect(buttonCoverCircle,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setCoverCircleColor(color);
        }

    });
    connect(buttonScale,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setScaleColor(color);
        }

    });
    connect(buttonPointer,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setPointerColor(color);
        }

    });
    connect(buttonPieColorStart,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setPieColorStart(color);
        }

    });
    connect(buttonPieColorMid,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setPieColorMid(color);
        }

    });
    connect(buttonPieColorEnd,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setPieColorEnd(color);
        }

    });
    connect(buttonOverlay,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setOverlayColor(color);
        }

    });
    connect(buttonCenterCircle,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setCenterCircleColor(color);
        }
    });
    connect(buttonText,&QPushButton::clicked,this,[this](){
        QColor color=QColorDialog::getColor();
        if(color.isValid()){
            this->dashBoard->setTextColor(color);
        }
    });



/*内圆比例*/

    QSlider* sliderStartScale=new QSlider();
    sliderStartScale->setRange(0,100);

    QSlider* sliderMidScale=new QSlider();
    sliderMidScale->setRange(0,100);

    sliderStartScale->setValue(dashBoard->getStartScale()*100);
    sliderMidScale->setValue(dashBoard->getMidScale()*100);

    QGridLayout*scaleGroup=new QGridLayout();
    scaleGroup->addWidget(sliderStartScale,0,0);
    scaleGroup->addWidget(sliderMidScale,0,1);
    QGroupBox*scaleGroupBox=new QGroupBox("比例调节");
    scaleGroupBox->setLayout(scaleGroup);
    connect(sliderStartScale,&QAbstractSlider::valueChanged,this,[this](int startScale){
        //qDebug()<<"start:"<<startScale<<endl;
        this->dashBoard->setStartScale((double)startScale/100.0);
    });
    connect(sliderMidScale,&QSlider::valueChanged,this,[this](int midScale){
        //qDebug()<<"mid:"<<midScale<<endl;
        this->dashBoard->setMidScale((double)midScale/100.0);
    });

/*指针*/
    QPushButton* buttonCircle        =new QPushButton("圆形");//遮罩层
    QPushButton* buttonIndicator   =new QPushButton("指针");//中心圆
    QPushButton* buttonIndicatorR          =new QPushButton("圆角指针");//中心字
    QPushButton* buttonTriangle          =new QPushButton("三角形");//中心字
    connect(buttonCircle,&QPushButton::clicked,this,[this](){
        this->dashBoard->setPointerStyle(DashBoard::PointerStyle::PointerStyle_Circle);
    });
    connect(buttonIndicator,&QPushButton::clicked,this,[this](){
        this->dashBoard->setPointerStyle(DashBoard::PointerStyle::PointerStyle_Indicator);
    });
    connect(buttonIndicatorR,&QPushButton::clicked,this,[this](){
        this->dashBoard->setPointerStyle(DashBoard::PointerStyle::PointerStyle_IndicatorR);
    });
    connect(buttonTriangle,&QPushButton::clicked,this,[this](){
        this->dashBoard->setPointerStyle(DashBoard::PointerStyle::PointerStyle_Triangle);
    });
    QGridLayout *indicatorGroup=new QGridLayout();
    indicatorGroup->addWidget(buttonCircle,0,0);
    indicatorGroup->addWidget(buttonIndicator,0,1);
    indicatorGroup->addWidget(buttonIndicatorR,1,0);
    indicatorGroup->addWidget(buttonTriangle,1,1);
    QGroupBox *indicatorGroupBox=new QGroupBox("指针样式");
    indicatorGroupBox->setLayout(indicatorGroup);

/*参数设置*/
    //设置范围值
//    void setRange(double minValue, double maxValue);
//    void setRange(int minValue, int maxValue);

//    //设置最大最小值
//    void setMinValue(double minValue);
//    void setMaxValue(double maxValue);

//    //设置目标值
//    void setValue(double value);
//    void setValue(int value);

//    //设置精确度
//    void setPrecision(int precision);

//    //设置主刻度数量
//    void setScaleMajor(int scaleMajor);
//    //设置小刻度数量
//    void setScaleMinor(int scaleMinor);
//    //设置开始旋转角度
//    void setStartAngle(int startAngle);
//    //设置结束旋转角度
//    void setEndAngle(int endAngle);
    rangeLineL=new QTextEdit();
    rangeLineR=new QTextEdit();
    numberLine1=new QTextEdit();
    numberLine2=new QTextEdit();
    angleLine1=new QTextEdit();
    angleLine2=new QTextEdit();
    curValueLine=new QTextEdit();
    parLayout=new QGridLayout();
    parLayout->addWidget(new QLabel("最大刻度:"),0,0);
    parLayout->addWidget(rangeLineL,0,1);
    parLayout->addWidget(new QLabel("最小刻度:"),1,0);
    parLayout->addWidget(rangeLineR,1,1);
    parLayout->addWidget(new QLabel("主刻度数:"),2,0);
    parLayout->addWidget(numberLine1,2,1);
    parLayout->addWidget(new QLabel("小刻度数:"),3,0);
    parLayout->addWidget(numberLine2,3,1);
    parLayout->addWidget(new QLabel("起始角度:"),4,0);
    parLayout->addWidget(angleLine1,4,1);
    parLayout->addWidget(new QLabel("角度跨度:"),5,0);
    parLayout->addWidget(angleLine2,5,1);
    parLayout->addWidget(new QLabel("当前数值:"),6,0);
    parLayout->addWidget(curValueLine,6,1);
    QPushButton *buttonOk=new QPushButton("save");
    parLayout->addWidget(buttonOk,7,1);
    checkBox=new QCheckBox("tcp接收");
    parLayout->addWidget(checkBox,7,0);
    QGroupBox *parGroupBox=new QGroupBox("参数设置");
    parGroupBox->setLayout(parLayout);
    connect(checkBox,&QCheckBox::stateChanged,this,[this](){
        auto stat=this->checkBox->checkState();
        static bool isfirst=true;
        if(stat==Qt::CheckState::Checked){
            if(isfirst){
                server=ValueServer::getServer();
                std::thread([this](){
                    int preValue=0;
                    while(1){
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        int curValue=this->server->getValue();
                        if(curValue!=preValue){
                            preValue=curValue;
                            this->valueChanged(curValue);
                        }
                    }
                }).detach();
                isfirst=false;
            }else{
                this->server->startServer();
            }
        }else{
            this->server->pauseServer();
        }
    });
    connect(buttonOk,&QPushButton::clicked,this,[this](){
        static int rangeLineLText=0;
        static int rangeLineRText=0 ;
        static int numberLine1=0  ;
        static int numberLine2=0 ;
        static int angleLine1=0 ;
        static int angleLine2=0;
        static int curValue=0 ;
        try{
            bool ok;
            rangeLineLText = this->rangeLineL->toPlainText().toInt(&ok);
            if(!ok){throw "error";}
            rangeLineRText = this->rangeLineR->toPlainText().toInt(&ok);
            if(!ok){throw "error";}
            numberLine1    = this->numberLine1->toPlainText().toInt(&ok);
            if(!ok){throw "error";}
            numberLine2    = this->numberLine2->toPlainText().toInt(&ok);
            if(!ok){throw "error";}
            angleLine1     = this->angleLine1->toPlainText().toInt(&ok);
            if(!ok){throw "error";}
            angleLine2     = this->angleLine2->toPlainText().toInt(&ok);
            if(!ok){throw "error";}
            curValue       = this->curValueLine->toPlainText().toInt(&ok);
            if(!ok){throw "error";}
            this->dashBoard->setRange(rangeLineRText,rangeLineLText);
            this->dashBoard->setStartAngle(angleLine1);
            this->dashBoard->setEndAngle(angleLine2);
            this->dashBoard->setScaleMajor(numberLine1);
            this->dashBoard->setScaleMinor(numberLine2);
            this->dashBoard->setValue(curValue);
        }catch(...){
            QMessageBox::warning(nullptr, "error", "parment error",
                                     QMessageBox::Ok | QMessageBox::Cancel);
        }

    });


/*布局管理*/
    QHBoxLayout *hLayout=new QHBoxLayout(this);
    QVBoxLayout *vLayout=new QVBoxLayout();
    QHBoxLayout *hLayout1=new QHBoxLayout();
    vLayout->addWidget(gColorGroup,3);
    //vLayout->addWidget(buttonOutCircle,1);
    hLayout1->addWidget(scaleGroupBox,1);
    hLayout1->addWidget(indicatorGroupBox,2);
    vLayout->addLayout(hLayout1);
    vLayout->addWidget(parGroupBox,1);
    //vLayout->addWidget(sliderMidScale);

    hLayout->addWidget(dashBoard,3);
    hLayout->addLayout(vLayout,1);

    connect(this,&Widget::valueChanged,this,[this](int val){
        this->dashBoard->setValue(val);
    });

}

Widget::~Widget()
{
    delete ui;
}

