#include "CircleFilter.hpp"
#include <opencv4/opencv2/core/types.hpp>

CircleFilter::CircleFilter() {}

CircleFilter::~CircleFilter() {}

cv::Mat CircleFilter::preprocessImageToHSV(const cv::Mat& inputImage)
{
    cv::Mat hsvImage;
    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);
    return hsvImage;
}

std::vector<cv::Vec3f> CircleFilter::filterByBinaryCount(const cv::Mat& inputImage, const std::vector<cv::Vec3f>& circles, const cv::Scalar& Lower, const cv::Scalar& Upper, float thresholdRatio)
{
    cv::Mat hsvImage = preprocessImageToHSV(inputImage);
    cv::Mat binaryImage;
    cv::inRange(hsvImage, Lower, Upper, binaryImage); // 根据颜色范围进行二值化

    cv::imshow("bi", binaryImage);

    std::vector<cv::Vec3f> filteredCircles;
    for (const auto& circle : circles)
    {
        cv::Point center(circle[0], circle[1]);
        int radius = circle[2];
        cv::Mat mask = cv::Mat::zeros(binaryImage.size(), CV_8UC1);
        cv::circle(mask, center, radius, cv::Scalar(255), -1);

        // 显示
        // cv::imshow("mask", mask);
        // cv::imshow("binaryImage & mask", binaryImage & mask);

        int totalPixels = cv::countNonZero(mask);
        int circlePixels = cv::countNonZero(binaryImage & mask);

        if (static_cast<float>(circlePixels) / totalPixels > thresholdRatio)
        {
            filteredCircles.push_back(circle);
        }
    }
    return filteredCircles;
}

std::vector<cv::Vec3f> CircleFilter::filterByColorAverage(const cv::Mat& inputImage, const std::vector<cv::Vec3f>& circles, const cv::Scalar& Lower, const cv::Scalar& Upper)
{
    cv::Mat hsvImage = preprocessImageToHSV(inputImage);

    std::vector<cv::Vec3f> filteredCircles;
    for (const auto& circle : circles)
    {
        cv::Point center(circle[0], circle[1]);
        int radius = circle[2];
        cv::Mat mask = cv::Mat::zeros(hsvImage.size(), CV_8UC1);
        cv::circle(mask, center, radius, cv::Scalar(255), -1);

        cv::Scalar meanColor = cv::mean(hsvImage, mask);

        if (meanColor[0] >= Lower[0] && meanColor[0] <= Upper[0] && meanColor[1] >= Lower[1] && meanColor[1] <= Upper[1] && meanColor[2] >= Lower[2] && meanColor[2] <= Upper[2])
        {
            filteredCircles.push_back(circle);
        }
    }
    return filteredCircles;
}