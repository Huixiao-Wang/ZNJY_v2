#include "CircleDetector.hpp"
#include <opencv2/highgui.hpp>

CircleDetector::CircleDetector() {}

CircleDetector::~CircleDetector() {}

std::vector<cv::Vec3f> CircleDetector::detectCircles(const cv::Mat& inputImage, const cv::Mat& mask) {
    // 预处理图像
    cv::Mat preprocessedImage = preprocessImage(inputImage, mask);
    
    // 存储检测到的圆
    std::vector<cv::Vec3f> circles;

    // 使用霍夫圆变换检测圆
    cv::HoughCircles(preprocessedImage, circles, cv::HOUGH_GRADIENT, 1,
                     preprocessedImage.rows / 8, // 圆心之间的最小距离
                     100, 30, // canny edge和accumulator阈值
                     10, 20); // 圆半径范围

    return circles;
}

cv::Mat CircleDetector::preprocessImage(const cv::Mat& inputImage, const cv::Mat& mask) {
    cv::Mat middle, gray, masked;
    
    // 高斯滤波
    cv::GaussianBlur(inputImage, middle, cv::Size(5, 5), 2);
    
    // 转换为灰度图像
    cv::cvtColor(middle, gray, cv::COLOR_BGR2GRAY);

    // 应用掩码
    gray.copyTo(masked, mask);

    // 高斯滤波
    cv::GaussianBlur(masked, masked, cv::Size(5, 5), 2);

    return masked;
}
