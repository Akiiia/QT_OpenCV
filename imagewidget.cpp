#include "imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent), selecting(false), moved(false)
{
}

void ImageWidget::setImage(const QPixmap &pixmap)
{
    image = pixmap;
    update(); // 请求更新界面
}

QRect ImageWidget::getSelectionRect() const
{
    return selectionRect; // 返回选择的矩形区域
}

QPixmap ImageWidget::getPixmap() const
{
    return image; // 返回当前的图像
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPoint = event->pos();
        selectionRect.setTopLeft(startPoint);
        selectionRect.setBottomRight(startPoint);
        selecting = true;
        moved = false;
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting) {
        selectionRect.setBottomRight(event->pos());
        update();
        emit selectionChanged();
        moved = true;
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        selecting = false;
        selectionRect.setBottomRight(event->pos());

        if (!moved) {
            emit restoreImageSignals();
        }
        else if (!selectionRect.isEmpty() && !selectionRect.isNull() &&
                 image.rect().contains(selectionRect.topLeft()) &&
                 image.rect().contains(selectionRect.bottomRight())) {
            emit selectionFinished();
        }

        selectionRect = QRect();
        moved = false;
        update();
    }
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event); // 确保调用父类的绘制方法

    QPainter painter(this);

    // 画图像
    if (!image.isNull()) {
        painter.drawPixmap(0, 0, image);
    }

    // 画选择框
    if (selecting || !selectionRect.isNull()) {
        painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
        painter.drawRect(selectionRect);
    }
}
