#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);

    void setImage(const QPixmap &pixmap);
    QRect getSelectionRect() const;
    QPixmap getPixmap() const;

signals:
    void selectionChanged();
    void selectionFinished();
    void restoreImageSignals();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap image;
    QPoint startPoint;
    QRect selectionRect;
    bool selecting;
    bool moved;  // 判断是否进行了点击操作
};

#endif // IMAGEWIDGET_H
