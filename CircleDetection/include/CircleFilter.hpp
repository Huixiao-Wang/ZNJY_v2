#ifndef CIRCLE_FILTER_HPP
#define CIRCLE_FILTER_HPP

#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/core/types.hpp>
#include <vector>

class CircleFilter
{
public:
    CircleFilter();
    ~CircleFilter();

    /**
     * @brief 根据二值化图像中的点数统计筛选圆
     * @param inputImage 输入图像
     * @param circles 待筛选的圆
     * @param thresholdRatio 阈值比例
     * @return 筛选后的圆
     */
    std::vector<cv::Vec3f> filterByBinaryCount(const cv::Mat& inputImage, const std::vector<cv::Vec3f>& circles, const cv::Scalar& Lower, const cv::Scalar& Upper, float thresholdRatio);

    /**
     * @brief 根据颜色平均值筛选圆
     * @param inputImage 输入图像
     * @param circles 待筛选的圆
     * @param colorThreshold 颜色阈值
     * @return 筛选后的圆
     */
    std::vector<cv::Vec3f> filterByColorAverage(const cv::Mat& inputImage, const std::vector<cv::Vec3f>& circles, const cv::Scalar& Lower, const cv::Scalar& Upper);

private:
    cv::Mat preprocessImageToHSV(const cv::Mat& inputImage);
};

#endif // CIRCLE_FILTER_HPP