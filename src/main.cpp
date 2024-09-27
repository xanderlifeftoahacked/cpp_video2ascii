#include <chrono>
#include <iostream>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <string>
#include <thread>

#include <ncurses.h>

const std::string getpathto(const char *file) {
  std::stringstream ss;
  ss << RESOURCES_PATH << file;
  return ss.str();
}

const char get_ASCII_from_pixel(const int pixelintensity) {
  std::string chars_by_brightness = "$@B%8&#*/|(-_+;:,.  ";
  std::reverse(chars_by_brightness.begin(), chars_by_brightness.end());
  return chars_by_brightness[static_cast<float>(pixelintensity * chars_by_brightness.length()) / 256.f];
}

int main() {
  cv::VideoCapture video_capture(getpathto("vid1.mp4"));

  if (!video_capture.isOpened()) {
    std::cerr << "Failed to open video file.\n";
    return -1;
  }

  const float correction_factor = 4.75;
  const int targetfps = 10;
  const int fps = video_capture.get(cv::CAP_PROP_FPS);
  const int fpsdif = fps / targetfps;
  const int frame_duration = 1000 / fps;
  const int frame_width = video_capture.get(cv::CAP_PROP_FRAME_WIDTH);
  const int frame_height = video_capture.get(cv::CAP_PROP_FRAME_HEIGHT);
  const int screen_height = 120;
  const int screen_width = screen_height * (frame_width / frame_height) * correction_factor;

  int starttime = 0, endtime = 0, framedrawtime = 0, i = 0;
  cv::Mat original_frame, grayscaled_frame, grayscaled_resized_frame;

  initscr();
  for (;;) {
    starttime =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    i = fpsdif;
    while (--i) {
      video_capture.grab();
    }

    video_capture >> original_frame;
    if (original_frame.empty())
      break;

    cv::cvtColor(original_frame, grayscaled_frame, cv::COLOR_BGR2GRAY);
    cv::resize(grayscaled_frame, grayscaled_resized_frame, cv::Size(screen_width, screen_height), 0, 0,
               cv::INTER_LINEAR);

    for (int x = 0; x < screen_height; ++x) {
      for (int y = 0; y < screen_width; ++y) {
        mvaddch(x, y, get_ASCII_from_pixel(grayscaled_resized_frame.at<uchar>(x, y)));
      }
    }

    refresh();

    endtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                  .count();

    framedrawtime = endtime - starttime;

    std::this_thread::sleep_for(std::chrono::milliseconds(fpsdif * frame_duration - framedrawtime));
  }

  endwin();

  return 0;
}
