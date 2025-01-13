#include <opencv2/opencv.hpp>
#include "CircleDetection/include/CircleDetector.hpp"

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

void drawCircles(cv::Mat& image, const std::vector<cv::Vec3f>& circles, const cv::Scalar& color) {
    for (const auto& circle : circles) {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        double area = CV_PI * radius * radius;

        // 绘制圆
        cv::circle(image, center, radius, color, 2);

        // 标注圆的面积
        std::string text = "Area: " + std::to_string(static_cast<int>(area));
        cv::putText(image, text, center + cv::Point(-radius, -radius - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    }
}

cv::Mat extractColorMask(const cv::Mat& image, const cv::Scalar& lowerBound, const cv::Scalar& upperBound) {
    cv::Mat hsv, mask;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, lowerBound, upperBound, mask);
    return mask;
}

int main() {
    cv::VideoCapture cap(2); // 打开摄像头

    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open the camera" << std::endl;
        return -1;
    }

    CircleDetector detector;

    // 设置摄像头的分辨率
    cap.set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);  // 设置宽度为640像素
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);  // 设置高度为480像素

    // 获取摄像头分辨率
    int frame_width = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // cout << "Frame width: " << frame_width << ", Frame height: " << frame_height << endl;

    // 创建视频写入对象，用于保存输出视频
    cv::VideoWriter writer("output_video_with_fps.avi",
                           cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30,
                           cv::Size(frame_width, frame_height));

    // 计算帧率
    int frameCount = 0;
    double tickFrequency = cv::getTickFrequency();  // 获取计时频率
    double startTime = cv::getTickCount();  // 获取当前时间（开始时间）
    
    while (true) {
        cv::Mat frame;
        cap >> frame; // 捕获帧

        if (frame.empty()) {
            std::cerr << "Error: Empty frame captured" << std::endl;
            break;
        }

        // 提取颜色掩码
        cv::Mat redMask = extractColorMask(frame, cv::Scalar(0, 120, 70), cv::Scalar(10, 255, 255));
        cv::Mat blueMask = extractColorMask(frame, cv::Scalar(100, 150, 0), cv::Scalar(140, 255, 255));
        cv::Mat blackMask = extractColorMask(frame, cv::Scalar(0, 0, 0), cv::Scalar(180, 255, 50));

        // 膨胀和腐蚀去除噪声
        cv::morphologyEx(redMask, redMask, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1, -1), 3);
        cv::morphologyEx(blueMask, blueMask, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1, -1), 3);
        cv::morphologyEx(blackMask, blackMask, cv::MORPH_CLOSE, cv::Mat(),
                         cv::Point(-1, -1), 3);

        // 显示掩码
        cv::imshow("Red Mask", redMask);
        cv::imshow("Blue Mask", blueMask);
        cv::imshow("Black Mask", blackMask);

        // 检测红色圆
        std::vector<cv::Vec3f> redCircles = detector.detectCircles(frame, redMask);
        drawCircles(frame, redCircles, cv::Scalar(0, 0, 255));

        // 检测蓝色圆
        std::vector<cv::Vec3f> blueCircles = detector.detectCircles(frame, blueMask);
        drawCircles(frame, blueCircles, cv::Scalar(255, 0, 0));

        // 检测黑色圆
        std::vector<cv::Vec3f> blackCircles = detector.detectCircles(frame, blackMask);
        drawCircles(frame, blackCircles, cv::Scalar(0, 0, 0));

        // 计算帧率
        frameCount++;
        double currentTime = cv::getTickCount();
        double elapsedTime =
            (currentTime - startTime) / tickFrequency; // 计算经过的时间（秒）
        double fps = frameCount / elapsedTime;  // 计算帧率

        // 显示帧率等信息
        std::stringstream fpsText;
        fpsText << "FPS: " << int(fps);
        cv::putText(frame, fpsText.str(), cv::Point(frame.cols - 150, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
        
        // 显示结果
        cv::imshow("Ping Pong Detection", frame);

        // 按下ESC退出
        if (cv::waitKey(30) == 27) {
            break;
        }
    }

    return 0;
}
