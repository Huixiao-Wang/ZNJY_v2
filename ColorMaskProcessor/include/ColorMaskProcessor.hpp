#ifndef COLOR_MASK_PROCESSOR_HPP
#define COLOR_MASK_PROCESSOR_HPP

#include <opencv2/opencv.hpp>

class ColorMaskProcessor {
public:
    ColorMaskProcessor();
    ~ColorMaskProcessor();

    /**
     * @brief 提取颜色掩码
     * @param image 输入图像
     * @param lowerBound HSV颜色空间下的下界
     * @param upperBound HSV颜色空间下的上界
     * @return 提取的颜色掩码
     */
    cv::Mat extractColorMask(const cv::Mat& image, const cv::Scalar& lowerBound, const cv::Scalar& upperBound);

    /**
     * @brief 根据轮廓面积筛选生成新的掩码
     * @param mask 原始掩码
     * @param minArea 面积的最小阈值
     * @return 筛选后的掩码
     */
    cv::Mat filterContoursByArea(const cv::Mat& mask, double minArea);
};

#endif // COLOR_MASK_PROCESSOR_HPP
