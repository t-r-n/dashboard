#ifndef DASHBOARD_H
#define DASHBOARD_H


#include <QWidget>
#include<QPainter>
#include<complex>
#include<QtMath>
#include<QDebug>
#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT GaugeCar : public QWidget
#else
class DashBoard : public QWidget
#endif

{
    Q_OBJECT
public:
    enum PieStyle {
        PieStyle_Three = 0,         	//三色圆环
        PieStyle_Current = 1        	//当前圆环
    };

    enum PointerStyle {
        PointerStyle_Circle = 0,        //圆形指示器
        PointerStyle_Indicator = 1,     //指针指示器
        PointerStyle_IndicatorR = 2,    //圆角指针指示器
        PointerStyle_Triangle = 3       //三角形指示器
    };

    explicit DashBoard(QWidget *parent = 0);
    ~DashBoard();

protected:
    void paintEvent(QPaintEvent *);
    void drawOuterCircle(QPainter *painter);
    void drawInnerCircle(QPainter *painter);
    void drawColorPie(QPainter *painter);
    void drawCoverCircle(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawScaleNum(QPainter *painter);
    void drawPointerCircle(QPainter *painter);
    void drawPointerIndicator(QPainter *painter);
    void drawPointerIndicatorR(QPainter *painter);
    void drawPointerTriangle(QPainter *painter);
    void drawRoundCircle(QPainter *painter);
    void drawCenterCircle(QPainter *painter);
    void drawValue(QPainter *painter);
    void drawOverlay(QPainter *painter);

//private slots:
//    void updateValue();

private:
    double minValue;                //最小值
    double maxValue;                //最大值
    double value;                   //目标值
    int precision;                  //精确度,小数点后几位

    int scaleMajor;                 //大刻度数量
    int scaleMinor;                 //小刻度数量
    int startAngle;                 //开始旋转角度
    int endAngle;                   //结束旋转角度

    bool animation;                 //是否启用动画显示
    double animationStep;           //动画显示时步长

    QColor outerCircleColor;        //外圆背景颜色
    QColor innerCircleColor;        //内圆背景颜色

    QColor pieColorStart;           //饼圆开始颜色
    QColor pieColorMid;             //饼圆中间颜色
    QColor pieColorEnd;             //饼圆结束颜色
    double startScale;              //起始饼圆比例
    double midScale;                //中间饼圆比例

    QColor coverCircleColor;        //覆盖圆背景颜色
    QColor scaleColor;              //刻度尺颜色
    QColor pointerColor;            //指针颜色
    QColor centerCircleColor;       //中心圆颜色
    QColor textColor;               //文字颜色

    bool showOverlay;               //显示遮罩层
    QColor overlayColor;            //遮罩层颜色

    PieStyle pieStyle;              //饼图样式
    PointerStyle pointerStyle;      //指针样式

    bool reverse;                   //是否往回走
    double currentValue;            //当前值
    QTimer *timer;                  //定时器绘制动画

public:
#if 1
    double getMinValue()            const;
    double getMaxValue()            const;
    double getValue()               const;
    int getPrecision()              const;

    int getScaleMajor()             const;
    int getScaleMinor()             const;
    int getStartAngle()             const;
    int getEndAngle()               const;

    bool getAnimation()             const;
    double getAnimationStep()       const;
#endif
#if 1
    QColor getOuterCircleColor()    const;
#endif
#if 1
    QColor getInnerCircleColor()    const;

    QColor getPieColorStart()       const;
    QColor getPieColorMid()         const;
    QColor getPieColorEnd()         const;

    double getStartScale()          const;
    double getMidScale()            const;

    QColor getCoverCircleColor()    const;
    QColor getScaleColor()          const;
    QColor getPointerColor()        const;
    QColor getCenterCircleColor()   const;
    QColor getTextColor()           const;

    bool getShowOverlay()           const;
    QColor getOverlayColor()        const;

    PieStyle getPieStyle()          const;
    PointerStyle getPointerStyle()  const;

    //QSize sizeHint()                const;
    //QSize minimumSizeHint()         const;
/*
`sizeHint`是Qt中的一个函数，用于指定控件（widget）的建议大小（preferred size）。
当使用布局管理器（layout manager）时，布局将根据控件的`sizeHint`来调整它们的大小和位置。

在没有明确指定控件大小时，Qt会自动返回一个合理的`sizeHint`值。但是，如果需要更细粒度地控制控件的大小，
则可以重写`sizeHint`函数，并返回所需的大小。例如，如果要创建一个具有特定宽度和高度的自定义按钮，则可以重写`sizeHint`函数，并返回指定的大小。

总之，`sizeHint`函数可以帮助布局管理器确定控件的最佳大小和位置，以便在不同平台和分辨率下实现一致的布局效果。
*/
#endif


public Q_SLOTS:

    //设置范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);

    //设置最大最小值
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);

    //设置目标值
    void setValue(double value);
    void setValue(int value);

    //设置精确度
    void setPrecision(int precision);

    //设置主刻度数量
    void setScaleMajor(int scaleMajor);
    //设置小刻度数量
    void setScaleMinor(int scaleMinor);
    //设置开始旋转角度
    void setStartAngle(int startAngle);
    //设置结束旋转角度
    void setEndAngle(int endAngle);

    //设置是否启用动画显示
    void setAnimation(bool animation);
    //设置动画显示的步长
    void setAnimationStep(double animationStep);

    //设置外圆背景颜色
    void setOuterCircleColor(const QColor &outerCircleColor);

    //设置内圆背景颜色
    void setInnerCircleColor(const QColor &innerCircleColor);
    //设置饼圆三种颜色
    void setPieColorStart(const QColor &pieColorStart);
    void setPieColorMid(const QColor &pieColorMid);
    void setPieColorEnd(const QColor &pieColorEnd);

    //设置覆盖圆背景颜色
    void setCoverCircleColor(const QColor &coverCircleColor);
    //设置刻度尺颜色
    void setScaleColor(const QColor &scaleColor);
    //设置指针颜色
    void setPointerColor(const QColor &pointerColor);
    //设置中心圆颜色
    void setCenterCircleColor(const QColor &centerCircleColor);
    //设置文本颜色
    void setTextColor(const QColor &textColor);

    //设置是否显示遮罩层
    void setShowOverlay(bool showOverlay);
    //设置遮罩层颜色
    void setOverlayColor(const QColor &overlayColor);

    //设置饼图样式
    void setPieStyle(const PieStyle &pieStyle);
    //设置指针样式
    void setPointerStyle(const PointerStyle &pointerStyle);

    //设置起始饼圆比例
    void setStartScale(double startScale);
    void setMidScale(double startScale);
#if 0
Q_SIGNALS:
    void valueChanged(int value);
#endif
};


#endif // DASHBOARD_H


