#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>

const std::string getpathto(const char *file) {
  std::stringstream ss;
  ss << RESOURCES_PATH << file;
  return ss.str();
}

std::string get_ASCII_from_pixel(const int pixelintensity) {
  std::string chars_by_brightness = "$@B%8&#*/|-_+:,.  ";
  std::reverse(chars_by_brightness.begin(), chars_by_brightness.end());
  std::string return_string(
      1,
      chars_by_brightness[pixelintensity * chars_by_brightness.length() / 256]);
  return return_string;
}

int main() {
  const std::string video_path = getpathto("vid1.mp4");
  cv::VideoCapture video_capture(video_path);

  if (!video_capture.isOpened()) {
    std::cerr << "Failed to open video file." << std::endl;
    return -1;
  }

  static constexpr int screen_width = 500;
  static constexpr int screen_height = 150;
  static const double video_fps = video_capture.get(cv::CAP_PROP_FPS);
  static const int frame_duration = 1000 / video_fps;
  static const int frame_width = video_capture.get(cv::CAP_PROP_FRAME_WIDTH);
  static const int frame_height = video_capture.get(cv::CAP_PROP_FRAME_HEIGHT);

  cv::Mat original_frame, grayscaled_frame, grayscaled_resized_frame, final_img;
  video_capture >> original_frame;

  for (;;) {
    std::string output;
    video_capture >> original_frame;
    if (original_frame.empty())
      break;

    cv::cvtColor(original_frame, grayscaled_frame, cv::COLOR_BGR2GRAY);
    cv::resize(grayscaled_frame, grayscaled_resized_frame,
               cv::Size(screen_width, screen_height), 0, 0, cv::INTER_LINEAR);
    for (int x = 0; x < screen_height; ++x) {
      for (int y = 0; y < screen_width; ++y) {
        output +=
            get_ASCII_from_pixel(grayscaled_resized_frame.at<uchar>(x, y));
      }
      output += "\n";
    }
    std::system("clear");
    std::cout << output << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(frame_duration));
  }

  return 0;
}
