#include <QtWidgets>
#include <QDebug>
#include <QString>
#include <iostream>
#if defined(QT_PRINTSUPPORT_LIB)
#endif
#include "paintarea.h"
#define PI 3.14159265
#define MOUSE_MODE 0
#define RECTA_MODE 1
#define ELIPS_MODE 2
#define LINES_MODE 3
#define STARS_MODE 4
using namespace std;
short int mode = MOUSE_MODE;
bool restore_mode = 0;
bool first_click = 1;
bool first_cadr = 1;
QPoint figureStartPoint;

void PaintArea::return_back()
{

    if (!image_history.empty())
    {
        restore_mode = 1;
        image = image_history.back();
        image_history.pop_back();
        repaint();
    }
}

PaintArea::PaintArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    myPenWidth = 1;
    myPenColor = Qt::blue;

    foo = new QAction(this);
    foo->setShortcut(Qt::Key_R);
    connect(foo, SIGNAL(triggered()), this, SLOT(return_back()));
    this->addAction(foo);
}

bool PaintArea::openImage(const QString &fileName)
{
    QImage loadedImage;

    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat))
    {
        modified = false;
        return true;
    }
    else
    {
        return false;
    }
}

void PaintArea::addRectangle()
{
    mode = RECTA_MODE;
}

void PaintArea::addEllipse()
{
    mode = ELIPS_MODE;
}

void PaintArea::addStar()
{
    mode = STARS_MODE;
}

void PaintArea::addLine()
{
    mode = LINES_MODE;
}

void PaintArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void PaintArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void PaintArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        switch(mode)
        {
            case MOUSE_MODE:
            {
                lastPoint = event->pos();
                scribbling = true;
                if (first_click)
                {
                    image_history.push_back(image);
                    first_click = 0;
                }
                break;
            }
            default:
            {
                if (first_cadr)
                {
                    image_history.push_back(image);
                    first_cadr = 0;
                }
                figureStartPoint = event->pos();
                scribbling = true;
                break;
            }
        }
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
    {
        switch(mode)
        {
            case MOUSE_MODE:
            {
                drawLineTo(event->pos());
                break;
            }
            case RECTA_MODE:
            {
                drawRectangle(event->pos());
                break;
            }
            case ELIPS_MODE:
            {
                drawEllipse(event->pos());
                break;
            }
            case LINES_MODE:
            {
                drawLine(event->pos());
                break;
            }
            case STARS_MODE:
            {
                drawStar(event->pos());
                break;
            }
        }
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling)
    {
        switch(mode)
        {
            case MOUSE_MODE:
            {
                drawLineTo(event->pos());
                scribbling = false;
                first_click = 1;
                break;
            }
            case RECTA_MODE:
            {
                drawRectangle(event->pos());
                scribbling = false;
                image = image_cpy;
                mode = MOUSE_MODE;
                first_cadr = 1;
                break;
            }
            case ELIPS_MODE:
            {
                drawEllipse(event->pos());
                scribbling = false;
                image = image_cpy;
                mode = MOUSE_MODE;
                first_cadr = 1;
                break;
            }
            case LINES_MODE:
            {
                drawLine(event->pos());
                scribbling = false;
                image = image_cpy;
                mode = MOUSE_MODE;
                first_cadr = 1;
                break;
            }
            case STARS_MODE:
            {
                drawStar(event->pos());
                scribbling = false;
                image = image_cpy;
                mode = MOUSE_MODE;
                first_cadr = 1;
                break;
            }
        }
    }
}

void PaintArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    if (mode == MOUSE_MODE)
        painter.drawImage(dirtyRect, image, dirtyRect);
    else
        painter.drawImage(dirtyRect, image_cpy, dirtyRect);
}

void PaintArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height())
    {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void PaintArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    painter.drawLine(lastPoint, endPoint);

    modified = true;

    int rad = (myPenWidth / 2) + 2;

    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;
}

void PaintArea::drawRectangle(const QPoint &endPoint)
{
    image_cpy = image;

    QPainter painter(&image_cpy);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    painter.drawRect(figureStartPoint.x(),
                     figureStartPoint.y(),
                     endPoint.x()-figureStartPoint.x(),
                     endPoint.y()-figureStartPoint.y());

    modified = true;

    // int rad = (myPenWidth/2) + 2;
    // update(QRect(figureStartPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    update(QRect(QPoint(0,0), QPoint(1280,760)).normalized());
}

void PaintArea::drawLine(const QPoint &endPoint)
{
    image_cpy = image;

    QPainter painter(&image_cpy);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    painter.drawLine(figureStartPoint, endPoint);

    modified = true;

    update(QRect(QPoint(0,0), QPoint(1280,760)).normalized());
}

void PaintArea::drawStar(const QPoint &endPoint)
{
    image_cpy = image;

    QPainter painter(&image_cpy);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    int rad = (endPoint.x() - figureStartPoint.x())/2;
    QPoint centralPoint(figureStartPoint.x() + rad, figureStartPoint.y() + rad);
    //1
    painter.drawLine(centralPoint+QPoint(rad*cos(18 * PI / 180), -rad*sin(18 * PI / 180)),
                     centralPoint+QPoint((6*rad/16)*cos(54 * PI / 180), -(6*rad/16)*sin(54 * PI / 180)));
    //2
    painter.drawLine(centralPoint+QPoint((6*rad/16)*cos(54 * PI / 180), -(6*rad/16)*sin(54 * PI / 180)),
                     centralPoint+QPoint(0,-rad));
    //3
    painter.drawLine(centralPoint+QPoint(0,-rad),
                     centralPoint+QPoint(-(6*rad/16)*cos(54 * PI / 180), -(6*rad/16)*sin(54 * PI / 180)));
    //4
    painter.drawLine(centralPoint+QPoint(-(6*rad/16)*cos(54 * PI / 180), -(6*rad/16)*sin(54 * PI / 180)),
                     centralPoint+QPoint(-rad*cos(18 * PI / 180), -rad*sin(18 * PI / 180)));
    //5
    painter.drawLine(centralPoint+QPoint(-rad*cos(18 * PI / 180), -rad*sin(18 * PI / 180)),
                     centralPoint+QPoint(-(6*rad/16)*cos(18 * PI / 180), (6*rad/16)*sin(18 * PI / 180)));
    //6
    painter.drawLine(centralPoint+QPoint(-(6*rad/16)*cos(18 * PI / 180), +(6*rad/16)*sin(18 * PI / 180)),
                     centralPoint+QPoint(-rad*cos(54 * PI / 180), rad*sin(54 * PI / 180)));
    //7
    painter.drawLine(centralPoint+QPoint(-rad*cos(54 * PI / 180), rad*sin(54 * PI / 180)),
                     centralPoint+QPoint(0,6*rad/16));
    //8
    painter.drawLine(centralPoint+QPoint(0,6*rad/16),
                     centralPoint+QPoint(rad*cos(54 * PI / 180), rad*sin(54 * PI / 180)));
    //9
    painter.drawLine(centralPoint+QPoint(rad*cos(54 * PI / 180), rad*sin(54 * PI / 180)),
                     centralPoint+QPoint((6*rad/16)*cos(18 * PI / 180), (6*rad/16)*sin(18 * PI / 180)));
    //10
    painter.drawLine(centralPoint+QPoint((6*rad/16)*cos(18 * PI / 180), (6*rad/16)*sin(18 * PI / 180)),
                     centralPoint+QPoint(rad*cos(18 * PI / 180), -rad*sin(18 * PI / 180)));

    modified = true;

    update(QRect(QPoint(0,0), QPoint(1280,760)).normalized());
}

void PaintArea::drawEllipse(const QPoint &endPoint)
{
    image_cpy = image;

    QPainter painter(&image_cpy);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    painter.drawEllipse(QRect(figureStartPoint.x(),
                              figureStartPoint.y(),
                              endPoint.x()-figureStartPoint.x(),
                              endPoint.y()-figureStartPoint.y()));

    modified = true;

    update(QRect(QPoint(0,0), QPoint(1280,760)).normalized());
}

void PaintArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));

    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void PaintArea::print()
{
#ifdef QT_FEATURE_printdialog
#if QT_FEATURE_printdialog == 1
#endif
#endif
}
