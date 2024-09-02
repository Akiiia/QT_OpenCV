#include "templatematcher.h"

cv::Mat TemplateMatcher::qImageToMat(const QImage &image)
{
    switch (image.format()) {
    case QImage::Format_RGB32: {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
        return mat.clone(); // 深拷贝
    }
    case QImage::Format_RGB888: {
        cv::Mat mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        return mat.clone();
    }
    case QImage::Format_Grayscale8: {
        cv::Mat mat(image.height(), image.width(), CV_8UC1, (void*)image.bits(), image.bytesPerLine());
        return mat.clone();
    }
    default:
        throw std::runtime_error("QImage 不支持该格式。");
    }
}

cv::Mat TemplateMatcher::matchTemplate(const QImage &image, const QImage &templateImage)
{
    // 转换为 OpenCV Mat
    cv::Mat originalImage = qImageToMat(image);
    cv::Mat templateImageMat = qImageToMat(templateImage);

    // 进行模板匹配
    cv::Mat result;
    cv::matchTemplate(originalImage, templateImageMat, result, cv::TM_CCOEFF_NORMED);

    return result;
}
