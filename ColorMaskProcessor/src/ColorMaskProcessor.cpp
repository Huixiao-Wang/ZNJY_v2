#include "ColorMaskProcessor.hpp"

ColorMaskProcessor::ColorMaskProcessor() {}

ColorMaskProcessor::~ColorMaskProcessor() {}

cv::Mat ColorMaskProcessor::extractColorMask(const cv::Mat& image, const cv::Scalar& lowerBound, const cv::Scalar& upperBound) {
    cv::Mat hsv, mask;

    // 转换为HSV颜色空间
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    
    // 提取指定范围的颜色掩码
    cv::inRange(hsv, lowerBound, upperBound, mask);

    // 膨胀和腐蚀去除噪声
    cv::Mat kernel =
        cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

    return mask;
}

cv::Mat ColorMaskProcessor::filterContoursByArea(const cv::Mat& mask, double minArea) {
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat filteredMask = cv::Mat::zeros(mask.size(), CV_8UC1);

    // 找到所有轮廓
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);

        // 如果轮廓面积大于阈值，则保留该轮廓
        if (area >= minArea) {
            cv::drawContours(filteredMask, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), cv::FILLED);
        }
    }

    return filteredMask;
}
