#include "CircleDetector.hpp"
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

CircleDetector::CircleDetector() {}

CircleDetector::~CircleDetector() {}

std::vector<cv::Vec3f> CircleDetector::detectCircles(const cv::Mat& inputImage)
{
    // 预处理图像
    // cv::Mat preprocessedImage = preprocessImage(inputImage, 0);
    cv::Mat preprocessedImage_ALT = preprocessImage(inputImage, 1);

    // 显示预处理图像
    // cv::imshow("Preprocessed Image", preprocessedImage);
    cv::imshow("Preprocessed Image ALT", preprocessedImage_ALT);

    // 存储检测到的圆
    std::vector<cv::Vec3f> circles, circles_alt;

    // 使用非ALT霍夫圆变换检测圆
    // cv::HoughCircles(preprocessedImage, circles, cv::HOUGH_GRADIENT, 1,
    //                  10,      // 圆心之间的最小距离
    //                  100, 30, // canny edge和accumulator阈值
    //                  10, 80); // 圆半径范围

    // 非极大值抑制筛选
    // nonMaximumSuppression(circles, 20);

    // 使用ALT霍夫圆变换检测圆
    cv::HoughCircles(preprocessedImage_ALT, circles_alt, cv::HOUGH_GRADIENT_ALT, 1, 10, 200, 0.85, 10, 80);

    // 合并
    // circles.insert(circles.end(), circles_alt.begin(), circles_alt.end());


    return circles_alt;
}

cv::Mat CircleDetector::preprocessImage(const cv::Mat& inputImage, bool flag)
{
    cv::Mat gray;

    // 转换为灰度图像
    cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);

    // 高斯滤波
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 2);

    if (flag)
    {
        return gray;
    }

    // 非ALT时，使用 Canny 边缘检测
    cv::Mat edges;
    cv::Canny(gray, edges, 50, 150);
    return edges;
}

float CircleDetector::computeDistance(const cv::Vec3f& circle1, const cv::Vec3f& circle2)
{
    cv::Point2f center1(circle1[0], circle1[1]);
    cv::Point2f center2(circle2[0], circle2[1]);
    return cv::norm(center1 - center2);
}

void CircleDetector::nonMaximumSuppression(std::vector<cv::Vec3f>& circles, float distanceThreshold)
{
    // 按照圆的半径从大到小排序
    std::sort(circles.begin(), circles.end(), [](const cv::Vec3f& a, const cv::Vec3f& b) {
        return a[2] > b[2]; // 根据半径排序，半径大优先
    });

    // 存储被保留的圆形
    std::vector<cv::Vec3f> selectedCircles;

    // 遍历所有圆形
    for (size_t i = 0; i < circles.size(); i++)
    {
        bool isOverlapped = false;

        // 检查当前圆与已选圆形的重叠情况
        for (size_t j = 0; j < selectedCircles.size(); j++)
        {
            if (computeDistance(circles[i], selectedCircles[j]) < distanceThreshold)
            {
                isOverlapped = true;
                break;
            }
        }

        // 如果没有重叠，则保留当前圆形
        if (!isOverlapped)
        {
            selectedCircles.push_back(circles[i]);
        }
    }

    // 将筛选后的圆形返回
    circles = selectedCircles;
}
