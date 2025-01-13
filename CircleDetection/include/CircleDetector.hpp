#ifndef CIRCLE_DETECTOR_HPP
#define CIRCLE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class CircleDetector {
public:
    CircleDetector();
    ~CircleDetector();

    /**
     * @brief Detect circles in the given image using Hough Circle Transform.
     * @param inputImage Input image in which circles will be detected.
     * @param mask Mask for restricting the detection region.
     * @return A vector of detected circles (x, y, radius).
     */
    std::vector<cv::Vec3f> detectCircles(const cv::Mat& inputImage, const cv::Mat& mask);

private:
    cv::Mat preprocessImage(const cv::Mat& inputImage, const cv::Mat& mask);
};

#endif // CIRCLE_DETECTOR_HPP
