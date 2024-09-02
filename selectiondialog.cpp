#include "selectiondialog.h"

SelectionDialog::SelectionDialog(QWidget *parent)
    : QDialog(parent), imageWidget(new ImageWidget(this))
{
    setLayout(new QVBoxLayout);
    layout()->addWidget(imageWidget);
    layout()->setContentsMargins(0, 0, 0, 0);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    // 连接选择完成信号
    connect(imageWidget, &ImageWidget::selectionFinished, this, &SelectionDialog::onSelectionFinished);
    connect(imageWidget, &ImageWidget::restoreImageSignals, this, &SelectionDialog::onImageClicked);
}

void SelectionDialog::setImage(const QPixmap &pixmap)
{
    originalPixmap = pixmap; // 保留原始彩色图像
    imageWidget->setImage(pixmap);
    imageWidget->resize(pixmap.size());
    setFixedSize(pixmap.size());
}

QRect SelectionDialog::getSelectedRect() const
{
    return imageWidget->getSelectionRect();
}

void SelectionDialog::onSelectionFinished()
{
    //imageWidget->setImage(originalPixmap);  // 开始时重置，现已更新成 onImageClicked()
    matchTemplate(); // 在选择完成后调用匹配函数
}

void SelectionDialog::onImageClicked()
{
    restoreOriginalImage(); // 点击时恢复原始图像
}

void SelectionDialog::restoreOriginalImage()
{
    imageWidget->setImage(originalPixmap); // 将图片恢复为原彩色图像
}

void SelectionDialog::matchTemplate()
{
    QRect selectedRect = getSelectedRect();
    if (selectedRect.isEmpty()) {
        QMessageBox::warning(this, "无效的选择", "请选择一个有效的矩形区域。");
        return;
    }
    QImage originalQImage = imageWidget->getPixmap().toImage();

    if (selectedRect.isValid()) {
        QImage selectedArea = originalQImage.copy(selectedRect).convertToFormat(QImage::Format_Grayscale8);
        QImage grayOriginal = originalQImage.convertToFormat(QImage::Format_Grayscale8);

        cv::Mat originalImage = TemplateMatcher::qImageToMat(grayOriginal);
        cv::Mat templateImage = TemplateMatcher::qImageToMat(selectedArea);

        cv::Mat matchResult;
        cv::matchTemplate(originalImage, templateImage, matchResult, cv::TM_CCOEFF_NORMED);

        double threshold = 0.9;

        std::vector<cv::Point> matchLocations;
        for (int y = 0; y < matchResult.rows; y++) {
            for (int x = 0; x < matchResult.cols; x++) {
                if (matchResult.at<float>(y, x) >= threshold) {
                    matchLocations.push_back(cv::Point(x, y));
                }
            }
        }

        if (matchLocations.empty()) {
            QString message = "没有找到任何匹配位置";
            QMessageBox::information(this, "匹配结果", message);
            return;
        }

        // 非极大抑制
        std::vector<cv::Point> finalLocations;
        std::vector<double> scores;

        for (const auto &loc : matchLocations) {
            scores.push_back(matchResult.at<float>(loc));
        }

        while (!scores.empty()) {
            auto maxIt = std::max_element(scores.begin(), scores.end());
            int maxIdx = std::distance(scores.begin(), maxIt);
            finalLocations.push_back(matchLocations[maxIdx]);

            // 删除最大值及其附近元素（防止重叠区域）
            scores.erase(scores.begin() + maxIdx);
            matchLocations.erase(matchLocations.begin() + maxIdx);

            for (int i = 0; i < scores.size(); ) {
                if (cv::norm(matchLocations[i] - finalLocations.back()) < templateImage.cols / 2) {
                    scores.erase(scores.begin() + i);
                    matchLocations.erase(matchLocations.begin() + i);
                } else {
                    ++i;
                }
            }
        }

        QString message = QString("找到 %1 个匹配位置").arg(finalLocations.size());
        QMessageBox::information(this, "匹配结果", message);

        for (const auto &loc : finalLocations) {
            cv::rectangle(originalImage, loc,
                          cv::Point(loc.x + templateImage.cols, loc.y + templateImage.rows),
                          cv::Scalar(0, 255, 0), 2);
        }

        QImage resultImage = QImage((const unsigned char *)originalImage.data,
                                    originalImage.cols,
                                    originalImage.rows,
                                    originalImage.step,
                                    QImage::Format_Grayscale8);
        imageWidget->setImage(QPixmap::fromImage(resultImage));
    }
}
