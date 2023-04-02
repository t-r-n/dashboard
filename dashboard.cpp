#include "dashboard.h"



DashBoard::DashBoard(QWidget *parent) : QWidget(parent)
{
#if 1
    scaleMajor=10;                 //大刻度数量
    scaleMinor=5;                 //小刻度数量

//扇形是3点种方向，rotate是6点钟方向正值顺时针旋转
    precision=0;                  //精确度,小数点后几位
    outerCircleColor=QColor(39,39,39);//外圆颜色
    innerCircleColor=QColor(28,40,57);//内圆颜色
    startAngle=-60;                 //开始旋转角度//这里的封装0度在圆的下面逆时针旋转部分不画
    endAngle=300;                   //结束旋转角度//跨越弧度3//跨越弧度除刻度数（大刻度乘小刻度）非整数会损失精度
    pieColorStart=QColor(154,228,156);           //饼圆开始颜色
    pieColorMid=QColor(154,178,86);             //饼圆中间颜色
    pieColorEnd=QColor(64,188,76);             //饼圆结束颜色
    currentValue=50;            //当前值
    minValue=0;                //最小值
    maxValue=100;                //最大值
    value=40;                   //目标值
    pieColorStart=QColor(205,19,19);           //饼圆开始颜色
    pieColorMid=QColor(247,244,50);             //饼圆中间颜色
    pieColorEnd= QColor(7,233,53);           //饼圆结束颜色
    startScale=0.2;
    midScale=0.3;
    coverCircleColor=QColor(37,25,47);        //覆盖圆背景颜色
    scaleColor=QColor(203,50,22);              //刻度尺颜色
    pointerColor=QColor(143,178,56);            //指针颜色
    centerCircleColor=QColor(224,163,86);       //中心圆颜色
    textColor=QColor(175,228,86);               //文字颜色
    showOverlay=true;               //显示遮罩层
    overlayColor=QColor(55,216,153);            //遮罩层颜色

    pieStyle=PieStyle::PieStyle_Three;              //饼图样式
    pointerStyle=PointerStyle::PointerStyle_Triangle;      //指针样式

#endif
}
DashBoard::~DashBoard()
{

}
void DashBoard::paintEvent(QPaintEvent *)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);//圆跟随较窄屏幕缩放

    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);//这个保证圆的大小随窗口放大缩小而移动
    //把刻度按当前宽高中小的分成200份，把坐标移到窗口正中间那取其中100份就是圆的半径
    //绘制外圆
    drawOuterCircle(&painter);

    //绘制内圆
    drawInnerCircle(&painter);

    //绘制饼圆（扇形图）
    drawColorPie(&painter);

    //绘制覆盖圆 用以遮住饼圆多余部分
    drawCoverCircle(&painter);

    //绘制刻度线
    drawScale(&painter);

    //绘制刻度值
    drawScaleNum(&painter);

    //根据指示器形状绘制指示器
    if (pointerStyle == PointerStyle_Circle) {
        drawPointerCircle(&painter);
    } else if (pointerStyle == PointerStyle_Indicator) {
        drawPointerIndicator(&painter);
    } else if (pointerStyle == PointerStyle_IndicatorR) {
        drawPointerIndicatorR(&painter);
    } else if (pointerStyle == PointerStyle_Triangle) {
        drawPointerTriangle(&painter);
    }

    //绘制指针中心圆外边框
    drawRoundCircle(&painter);
#if 1
    //绘制指针中心圆
    drawCenterCircle(&painter);
    //绘制当前值
    drawValue(&painter);
    //绘制遮罩层
    drawOverlay(&painter);
#endif
}

void DashBoard::drawOuterCircle(QPainter *painter)
{
    int radius = 99;//指圆体，球体等半径
    painter->save();//保存之前设置
    painter->setPen(Qt::NoPen);
    painter->setBrush(outerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();//恢复之前设置
}

void DashBoard::drawInnerCircle(QPainter *painter)
{
    int radius = 90;
    painter->save();
    painter->setPen(Qt::NoPen);//不画边框
    painter->setBrush(innerCircleColor);//设置画刷
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void DashBoard::drawColorPie(QPainter *painter)
{
    int radius = 60;
    painter->save();
    painter->setPen(Qt::NoPen);

    QRectF rect(-radius, -radius, radius * 2, radius * 2);
    double startAngleScale=startScale;
    double midAngleScale=midScale;
    if((1-startAngle)<midScale){//防越界
        midScale=(1-startScale);
        midAngleScale=midScale;
    }
    if((midScale+startScale)>1){//防越界
        midScale=(1-startScale);
        midAngleScale=midScale;
    }
    double endAngelScale=(1-startAngleScale-midAngleScale)>0?(1-startAngleScale-midAngleScale):0;
    //qDebug()<<startAngleScale<<" "<<midAngleScale<<" "<<endAngelScale<<endl;
    //绘制开始饼圆
    painter->setBrush(pieColorStart);
    //painter->drawPie(rect, (270 - startAngle - angleStart) * 16, angleStart * 16);
    painter->drawPie(rect,startAngle* 16,endAngle*startAngleScale * 16);//draw函数从圆的右边开始为0度逆时针旋转

    painter->setBrush(pieColorMid);
    painter->drawPie(rect,(startAngle+endAngle*startAngleScale)* 16,endAngle*midAngleScale * 16);//draw函数从圆的右边开始为0度逆时针旋转
    painter->setBrush(pieColorEnd);
    painter->drawPie(rect,(startAngle+endAngle*startAngleScale+endAngle*midAngleScale)* 16,endAngle*endAngelScale * 16);//draw函数从圆的右边开始为0度逆时针旋转
    painter->restore();
}
void DashBoard::drawCoverCircle(QPainter *painter)
{
    int radius = 50;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(coverCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void DashBoard::drawScale(QPainter *painter)
{
    int radius = 72;
    painter->save();

    painter->rotate(270-startAngle-endAngle);
    int steps = (scaleMajor * scaleMinor);//刻度数
    //double angleStep = (360.0 - startAngle - endAngle) / steps;
    double angleStep = endAngle / steps;
    QPen pen;
    pen.setColor(scaleColor);
    pen.setCapStyle(Qt::RoundCap);

    for (int i = 0; i <= steps; i++) {
        if (i % scaleMinor == 0) {
            pen.setWidthF(1.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 10, 0, radius);
        } else {
            pen.setWidthF(0.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 5, 0, radius);
        }

        painter->rotate(angleStep);
    }

    painter->restore();
}

void DashBoard::drawScaleNum(QPainter *painter)
{
    int radius = 80;
    painter->save();
    painter->setPen(scaleColor);

    double startRad = (0+startAngle+endAngle) * (M_PI / 180);//0度在3点钟方向逆时针为正//弧度=角度*PI/180
    double deltaRad = ( endAngle) * (M_PI / 180) / scaleMajor;//增量弧度

    for (int i = 0; i <= scaleMajor; i++) {
        double sina = qSin(startRad - i * deltaRad);
        double cosa = qCos(startRad - i * deltaRad);
        double value = 1.0 * i * ((maxValue - minValue) / scaleMajor) + minValue;

        QString strValue = QString("%1").arg((double)value, 0, 'f', precision);
        double textWidth = fontMetrics().width(strValue);
        double textHeight = fontMetrics().height();
        int x = radius * cosa - textWidth / 2;//坐标系是下x右y
        /* 坐标系
         --------->y
         |
         |
         |
        \/
         x
        */
        int y = -radius * sina + textHeight / 4;
        painter->drawText(x, y, strValue);
    }

    painter->restore();
}
void DashBoard::drawPointerCircle(QPainter *painter)
{
    int radius = 6;
    int offset = 30;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    painter->rotate(270-startAngle-endAngle);
    double degRotate = ( endAngle)  / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawEllipse(-radius, radius + offset, radius * 2, radius * 2);

    painter->restore();
}

void DashBoard::drawPointerIndicator(QPainter *painter)
{
    int radius = 75;
    painter->save();
    painter->setOpacity(0.8);
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(3, -5, 0, 5, 0, 0, radius);

    painter->rotate(270-startAngle-endAngle);
    double degRotate = ( endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);

    painter->restore();
}

void DashBoard::drawPointerIndicatorR(QPainter *painter)
{
    int radius = 75;
    painter->save();
    painter->setOpacity(1.0);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(pointerColor);
    painter->setPen(pen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(3, -5, 0, 5, 0, 0, radius);

    painter->rotate(270-startAngle-endAngle);
    double degRotate =( endAngle)  / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);

    //增加绘制圆角直线,与之前三角形重叠,形成圆角指针
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(4);
    painter->setPen(pen);
    painter->drawLine(0, 0, 0, radius);

    painter->restore();
}

void DashBoard::drawPointerTriangle(QPainter *painter)
{
    int radius = 10;
    int offset = 38;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(3, -5, 0 + offset, 5, 0 + offset, 0, radius + offset);

    painter->rotate(270-startAngle-endAngle);
    double degRotate = ( endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);

    painter->restore();
}

void DashBoard::drawRoundCircle(QPainter *painter)
{
    int radius = 18;
    painter->save();
    painter->setOpacity(0.8);
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void DashBoard::drawCenterCircle(QPainter *painter)
{
    int radius = 15;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(centerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void DashBoard::drawValue(QPainter *painter)
{
    int radius = 100;
    painter->save();
    painter->setPen(textColor);

    QFont font;
    font.setPixelSize(18);
    painter->setFont(font);

    QRectF textRect(-radius, -radius, radius * 2, radius * 2);
    QString strValue = QString("%1").arg((double)currentValue, 0, 'f', precision);
    painter->drawText(textRect, Qt::AlignCenter, strValue);

    painter->restore();
}

void DashBoard::drawOverlay(QPainter *painter)
{
    if (!showOverlay) {
        return;
    }

    int radius = 90;
    painter->save();
    painter->setPen(Qt::NoPen);

    QPainterPath smallCircle;
    QPainterPath bigCircle;
    radius -= 1;
    smallCircle.addEllipse(-radius, -radius, radius * 2, radius * 2);
    radius *= 2;
    bigCircle.addEllipse(-radius, -radius + 140, radius * 2, radius * 2);

    //高光的形状为小圆扣掉大圆的部分
    QPainterPath highlight = smallCircle - bigCircle;

    QLinearGradient linearGradient(0, -radius / 2, 0, 0);
    overlayColor.setAlpha(100);
    linearGradient.setColorAt(0.0, overlayColor);
    overlayColor.setAlpha(30);
    linearGradient.setColorAt(1.0, overlayColor);
    painter->setBrush(linearGradient);
    painter->rotate(-20);
    painter->drawPath(highlight);

    painter->restore();
}


double DashBoard::getMinValue()            const{
    return minValue;
}
double DashBoard::getMaxValue()            const{
    return maxValue;
}
double DashBoard::getValue()               const{
    return value;
}
int DashBoard::getPrecision()              const{
    return precision;
}

int DashBoard::getScaleMajor()             const{
    return scaleMajor;
}
int DashBoard::getScaleMinor()             const{
    return scaleMinor;
}
int DashBoard::getStartAngle()             const{
    return startAngle;
}
int DashBoard::getEndAngle()               const{
    return endAngle;
}

bool DashBoard::getAnimation()             const{
    return animation;
}
double DashBoard::getAnimationStep()       const{
    return animationStep;
}
QColor DashBoard::getOuterCircleColor()    const{
    return outerCircleColor;
}
QColor DashBoard::getInnerCircleColor()    const{
    return innerCircleColor;
}

QColor DashBoard::getPieColorStart()       const{
    return pieColorStart;
}
QColor DashBoard::getPieColorMid()         const{
    return pieColorMid;
}
QColor DashBoard::getPieColorEnd()         const{
    return pieColorEnd;
}

QColor DashBoard::getCoverCircleColor()    const{
    return coverCircleColor;
}
QColor DashBoard::getScaleColor()          const{
    return scaleColor;
}
QColor DashBoard::getPointerColor()        const{
    return pointerColor;
}
QColor DashBoard::getCenterCircleColor()   const{
    return centerCircleColor;
}
QColor DashBoard::getTextColor()           const{
    return textColor;
}
bool DashBoard::getShowOverlay()           const{
    return showOverlay;
}
QColor DashBoard::getOverlayColor()        const{
    return overlayColor;
}
DashBoard::PieStyle DashBoard::getPieStyle()          const{
    return pieStyle;
}
DashBoard::PointerStyle DashBoard::getPointerStyle()  const{
    return pointerStyle;
}

double DashBoard::getStartScale()          const{
    return startScale;
}
double DashBoard::getMidScale()            const{
    return midScale;
}
//QSize DashBoard::sizeHint()                const{

//}
//QSize DashBoard::minimumSizeHint()         const{

//}

//QColor DashBoard::getOuterCircleColor(){
//    auto it=const_cast<DashBoard*>(this);

//    QColor q(it->outerCircleColor);
//    return q;
//}
//设置范围值
void DashBoard::setRange(double minValue, double maxValue){
    this->minValue=minValue;
    this->maxValue=maxValue;
    update();
}
void DashBoard::setRange(int minValue, int maxValue){
    this->minValue=minValue;
    this->maxValue=maxValue;
    update();
}

//设置最大最小值
void DashBoard::setMinValue(double minValue){
    this->minValue=minValue;
    update();
}
void DashBoard::setMaxValue(double maxValue){
    this->maxValue=maxValue;
    update();
}

//设置目标值
void DashBoard::setValue(double value){
    this->currentValue=value;
    update();
}
void DashBoard::setValue(int value){
    this->currentValue=value;
    update();
}

//设置精确度
void DashBoard::setPrecision(int precision){
    this->precision=precision;
    update();
}

//设置主刻度数量
void DashBoard::setScaleMajor(int scaleMajor){
    this->scaleMajor=scaleMajor;
    update();
}
//设置小刻度数量
void DashBoard::setScaleMinor(int scaleMinor){
    this->scaleMinor=scaleMinor;
    update();
}
//设置开始旋转角度
void DashBoard::setStartAngle(int startAngle){
    this->startAngle=startAngle;
    update();
}
//设置结束旋转角度
void DashBoard::setEndAngle(int endAngle){
    this->endAngle=endAngle;
    update();
}

//设置是否启用动画显示
void DashBoard::setAnimation(bool animation){

}
//设置动画显示的步长
void DashBoard::setAnimationStep(double animationStep){

}
void DashBoard::setOuterCircleColor(const QColor &outerCircleColor){
    this->outerCircleColor=outerCircleColor;
    update();
}
//设置内圆背景颜色
void DashBoard::setInnerCircleColor(const QColor &innerCircleColor){
    this->innerCircleColor=innerCircleColor;
    update();
}
//设置饼圆三种颜色
void DashBoard::setPieColorStart(const QColor &pieColorStart){
    this->pieColorStart=pieColorStart;
    update();
}
void DashBoard::setPieColorMid(const QColor &pieColorMid){
    this->pieColorMid=pieColorMid;
    update();
}
void DashBoard::setPieColorEnd(const QColor &pieColorEnd){
    this->pieColorEnd=pieColorEnd;
    update();
}

//设置覆盖圆背景颜色
void DashBoard::setCoverCircleColor(const QColor &coverCircleColor){
    this->coverCircleColor=coverCircleColor;
    update();
}
//设置刻度尺颜色
void DashBoard::setScaleColor(const QColor &scaleColor){
    this->scaleColor=scaleColor;
    update();
}
//设置指针颜色
void DashBoard::setPointerColor(const QColor &pointerColor){
    this->pointerColor=pointerColor;
    update();
}
//设置中心圆颜色
void DashBoard::setCenterCircleColor(const QColor &centerCircleColor){
    this->centerCircleColor=centerCircleColor;
    update();
}
//设置文本颜色
void DashBoard::setTextColor(const QColor &textColor){
    this->textColor=textColor;
    update();
}
//设置是否显示遮罩层
void DashBoard::setShowOverlay(bool showOverlay){
    this->showOverlay=showOverlay;
    update();
}
//设置遮罩层颜色
void DashBoard::setOverlayColor(const QColor &overlayColor){
    this->overlayColor=overlayColor;
    update();
}
//设置饼图样式
void DashBoard::setPieStyle(const PieStyle &pieStyle){
    this->pieStyle=pieStyle;
    update();
}
//设置指针样式
void DashBoard::setPointerStyle(const PointerStyle &pointerStyle){
    this->pointerStyle=pointerStyle;
    update();
}
//设置起始饼圆比例
void DashBoard::setStartScale(double startScale){
    if(startScale>=0&&startScale<=1){
        this->startScale=startScale;
        update();
    }
}
void DashBoard::setMidScale(double midScale){
    if(midScale>=0&&midScale<=1){
        this->midScale=midScale;
        update();
    }
}
