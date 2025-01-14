#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/core/types.hpp>
#include <vector>

#include "CircleDetection/include/CircleDetector.hpp"
#include "CircleDetection/include/CircleFilter.hpp"
#include "ColorMaskProcessor/include/ColorMaskProcessor.hpp"

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480
#define MIN_CONTOUR_AREA 100
#define THRESHOLDRATIO 0.6

// 定义你想要检测的颜色范围 (HSV)
cv::Scalar LOWER_RED1 = cv::Scalar(0, 120, 70);    // 红色低阈值
cv::Scalar UPPER_RED1 = cv::Scalar(10, 255, 255);  // 红色高阈值
cv::Scalar LOWER_RED2 = cv::Scalar(170, 120, 70);  // 红色低阈值（第二段）
cv::Scalar UPPER_RED2 = cv::Scalar(180, 255, 255); // 红色高阈值（第二段）

cv::Scalar LOWER_BLUE = cv::Scalar(100, 50, 50);   // 蓝色低阈值
cv::Scalar UPPER_BLUE = cv::Scalar(140, 255, 255); // 蓝色高阈值

cv::Scalar LOWER_BLACK = cv::Scalar(0, 0, 0);       // 黑色低阈值
cv::Scalar UPPER_BLACK = cv::Scalar(180, 255, 150); // 黑色高阈值

void drawCircles(cv::Mat& image, const std::vector<cv::Vec3f>& circles, const cv::Scalar& color)
{
    for (const auto& circle : circles)
    {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        double area = CV_PI * radius * radius;

        // 绘制圆
        cv::circle(image, center, radius, color, 2);

        // 标注圆的面积
        std::string text = "Area: " + std::to_string(static_cast<int>(area));
        cv::putText(image, text, center + cv::Point(-radius, -radius - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    }
}

int main()
{
    cv::VideoCapture cap(2); // 打开摄像头

    if (!cap.isOpened())
    {
        std::cerr << "Error: Unable to open the camera" << std::endl;
        return -1;
    }

    CircleDetector detector;
    ColorMaskProcessor maskProcessor;
    CircleFilter filter;

    // 设置摄像头的曝光
    cap.set(cv::CAP_PROP_AUTO_EXPOSURE, 1); // 设置自动曝光

    // 设置摄像头的分辨率
    cap.set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);   // 设置宽度像素
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT); // 设置高度像素

    // 获取摄像头分辨率
    int frame_width = (int) cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // cout << "Frame width: " << frame_width << ", Frame height: " <<
    // frame_height << endl;

    // 创建视频写入对象，用于保存输出视频
    cv::VideoWriter writer("output_video_with_fps.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(frame_width, frame_height));

    // 计算帧率
    int frameCount = 0;
    double tickFrequency = cv::getTickFrequency(); // 获取计时频率
    double startTime = cv::getTickCount();         // 获取当前时间（开始时间）

    while (true)
    {
        cv::Mat frame;
        cap >> frame; // 捕获帧

        if (frame.empty())
        {
            std::cerr << "Error: Empty frame captured" << std::endl;
            break;
        }

        /* 霍夫圆检测 */
        std::vector<cv::Vec3f> circles;
        circles = detector.detectCircles(frame);
        // 绘制霍夫检测到的圆 (白色)
        drawCircles(frame, circles, cv::Scalar(255, 255, 255));

        /* 使用二值化图像的点数统计筛选圆 */
        // 黑色圆筛选
        std::vector<cv::Vec3f> blackCircles = filter.filterByBinaryCount(frame, circles, LOWER_BLACK, UPPER_BLACK, THRESHOLDRATIO);
        // 红色圆筛选
        std::vector<cv::Vec3f> redCircles = filter.filterByBinaryCount(frame, circles, LOWER_RED1, UPPER_RED1, THRESHOLDRATIO);
        std::vector<cv::Vec3f> redCircles2 = filter.filterByBinaryCount(frame, circles, LOWER_RED2, UPPER_RED2, THRESHOLDRATIO);
        // 合并两段红色圆
        redCircles.insert(redCircles.end(), redCircles2.begin(), redCircles2.end());
        // 蓝色圆筛选
        std::vector<cv::Vec3f> blueCircles = filter.filterByBinaryCount(frame, circles, LOWER_BLUE, UPPER_BLUE, THRESHOLDRATIO);
        /* 绘制二值化图像检测到的圆 */
        // 绘制黑色圆
        drawCircles(frame, blackCircles, cv::Scalar(0, 255, 0));
        // 绘制红色圆
        drawCircles(frame, redCircles, cv::Scalar(0, 255, 0));
        // 绘制蓝色圆
        drawCircles(frame, blueCircles, cv::Scalar(0, 255, 0));

        /* 使用颜色平均值筛选圆 */
        // 红色圆筛选
        std::vector<cv::Vec3f> redCirclesByColor = filter.filterByColorAverage(frame, redCircles, LOWER_RED1, UPPER_RED1);
        std::vector<cv::Vec3f> redCirclesByColor2 = filter.filterByColorAverage(frame, redCircles2, LOWER_RED2, UPPER_RED2);
        redCirclesByColor.insert(redCirclesByColor.end(), redCirclesByColor2.begin(), redCirclesByColor2.end());
        // 蓝色圆筛选
        std::vector<cv::Vec3f> blueCirclesByColor = filter.filterByColorAverage(frame, blueCircles, LOWER_BLUE, UPPER_BLUE);
        // 黑色圆筛选
        std::vector<cv::Vec3f> blackCirclesByColor = filter.filterByColorAverage(frame, blackCircles, LOWER_BLACK, UPPER_BLACK);
        /* 绘制颜色平均值检测到的圆 */
        // 绘制黑色圆
        drawCircles(frame, blackCirclesByColor, cv::Scalar(0, 0, 0));
        // 绘制红色圆
        drawCircles(frame, redCirclesByColor, cv::Scalar(0, 0, 255));
        // 绘制蓝色圆
        drawCircles(frame, blueCirclesByColor, cv::Scalar(255, 0, 0));

        // 计算帧率
        frameCount++;
        double currentTime = cv::getTickCount();
        double elapsedTime = (currentTime - startTime) / tickFrequency; // 计算经过的时间（秒）
        double fps = frameCount / elapsedTime;                          // 计算帧率

        // 显示帧率等信息
        std::stringstream fpsText;
        fpsText << "FPS: " << int(fps);
        cv::putText(frame, fpsText.str(), cv::Point(frame.cols - 150, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);

        // 显示结果
        cv::imshow("Detection", frame);

        // 按下ESC退出
        if (cv::waitKey(30) == 27)
        {
            break;
        }
    }

    return 0;
}
