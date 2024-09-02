#ifndef SELECTIONDIALOG_H
#define SELECTIONDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QVBoxLayout>
#include <QMessageBox>
#include "imagewidget.h"
#include "templatematcher.h"

class SelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectionDialog(QWidget *parent = nullptr);

    void setImage(const QPixmap &pixmap);
    QRect getSelectedRect() const;

private slots:
    void onSelectionFinished();
    void onImageClicked();

private:
    ImageWidget *imageWidget;
    QPixmap originalPixmap;  // 保存原始图像
    void restoreOriginalImage();  // 恢复原始图像
    void matchTemplate();
};

#endif // SELECTIONDIALOG_H
