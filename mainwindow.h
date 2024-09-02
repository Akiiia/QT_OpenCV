#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCameraInfo>
#include <QTimer>
#include <QCameraInfo>
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
#include <memory>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCameraSelected(int index);
    void onCaptureButtonClicked();
    void showSelectionDialog(const QImage &image);
    void updateFrame();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    cv::VideoCapture cap;
    cv::Mat frame;
};

#endif // MAINWINDOW_H
