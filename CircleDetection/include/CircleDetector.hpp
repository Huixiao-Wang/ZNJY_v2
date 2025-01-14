#ifndef CIRCLE_DETECTOR_HPP
#define CIRCLE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class CircleDetector
{
public:
    CircleDetector();
    ~CircleDetector();

    /**
	 * @brief Detect circles in the given image using Hough Circle Transform.
	 * @param inputImage Input image in which circles will be detected.
	 * @return A vector of detected circles (x, y, radius).
	 */
    std::vector<cv::Vec3f> detectCircles(const cv::Mat& inputImage);

private:
    // 预处理 (flag==1 -> alt)
    cv::Mat preprocessImage(const cv::Mat& inputImage, bool flag);

    // 计算两个圆心之间的距离
    float computeDistance(const cv::Vec3f& circle1, const cv::Vec3f& circle2);

    // 非极大值抑制去除重叠的圆形
    void nonMaximumSuppression(std::vector<cv::Vec3f>& circles, float distanceThreshold);
};

#endif // CIRCLE_DETECTOR_HPP
