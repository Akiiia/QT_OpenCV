#ifndef TEMPLATEMATCHER_H
#define TEMPLATEMATCHER_H

#include <opencv2/opencv.hpp>
#include <QImage>

class TemplateMatcher
{
public:
    static cv::Mat matchTemplate(const QImage &image, const QImage &templateImage);
    static cv::Mat qImageToMat(const QImage &image);
};

#endif // TEMPLATEMATCHER_H
