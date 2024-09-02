#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "selectiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 可用摄像头列表
    ui->cameraComboBox->addItem("请选择摄像头");
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : cameras) {
        ui->cameraComboBox->addItem(cameraInfo.description());
    }

    // 定时器，timeout 后更新画面 1000/30 = 四舍五入 30 帧
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);

    // 选中摄像头后开始输出画面
    connect(ui->cameraComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),  // 指定函数重载版本为 currentIndexChanged(int)
            this,
            &MainWindow::onCameraSelected);

    // 捕获当前帧（其实就是截图哈哈）
    connect(ui->captureButton,
            &QPushButton::clicked,
            this,
            &MainWindow::onCaptureButtonClicked);

    // 状态标签
    ui->statusLabel->setText("请选择摄像头并开始捕捉");
}

MainWindow::~MainWindow()
{
    delete ui;
    if (cap.isOpened()) {
        cap.release();
    }
}

void MainWindow::onCameraSelected(int index)
{
    if (index == 0) {
        ui->statusLabel->setText("请选择摄像头并开始捕捉");
        return;
    }

    // 停止当前摄像头捕获和定时器
    if (cap.isOpened()) {
        cap.release();
    }
    timer->stop();

    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (index > 0 && index <= cameras.size()) { // 只有在选择了有效摄像头之后
        // 开启新摄像头
        cap.open(index - 1); // 索引调节
        if (!cap.isOpened()) {
            ui->statusLabel->setText("无法打开摄像头");
            return;
        }

        // 固定分辨率
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

        ui->statusLabel->setText("正在捕捉视频...");
        timer->start(30);  // 每30毫秒更新一次，QTimer 会自动重复，所以其实是循环 30ms 倒计时并发送信号
    } else {
        ui->statusLabel->setText("选择无效的摄像头");
    }
}

void MainWindow::onCaptureButtonClicked()
{
    // 检查摄像头是否已经打开
    if (cap.isOpened()) {
        cv::Mat capturedFrame;
        cap >> capturedFrame;

        if (capturedFrame.empty()) {
            ui->statusLabel->setText("无法捕捉到图像");
            return;
        }

        // 转换为QImage
        cv::cvtColor(capturedFrame, capturedFrame, cv::COLOR_BGR2RGB);
        QImage image(capturedFrame.data, capturedFrame.cols, capturedFrame.rows, capturedFrame.step, QImage::Format_RGB888);

        // 显示选择对话框
        showSelectionDialog(image);
    } else {
        ui->statusLabel->setText("摄像头不可用");
    }
}

void MainWindow::showSelectionDialog(const QImage &image)
{
    // 直接阻塞，因为并不依赖接下来的行为了
    std::unique_ptr<SelectionDialog> dialog = std::make_unique<SelectionDialog>(this);
    dialog->setImage(QPixmap::fromImage(image));
    dialog->exec();
}

void MainWindow::updateFrame()
{
    if (cap.isOpened()) {
        // 抓取一帧图像到 frame
        cap >> frame;

        if (!frame.empty()) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            ui->videoDisplayLabel->setPixmap(QPixmap::fromImage(qimg));
        }
    }
}
