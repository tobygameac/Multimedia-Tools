#pragma once

#include <algorithm>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <opencv\cv.hpp>

namespace MultimediaTools {

  namespace AutomaticExtractionOfRepresentativeKeyframesBasedOnSceneContent {

    inline double RGBToGrayIntensity(const cv::Vec3b &rgb_pixel) {
      return (rgb_pixel.val[0] * 0.114 + rgb_pixel.val[1] * 0.587 + rgb_pixel.val[2] * 0.299) / 255.0;
    }

    cv::Vec3d Kurtosis(const cv::Mat &image) {
      cv::Mat gray_image;
      cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
      cv::Vec3d kurt(0, 0, 0), mean, stddev;
      cv::meanStdDev(gray_image, mean, stddev, cv::Mat());

      cv::Vec3d den(0, 0, 0);

      for (int r = 0; r < image.rows; r++) {
        for (int c = 0; c < image.cols; c++) {
          cv::Vec3d sum;
          for (size_t pixel_index = 0; pixel_index < 3; ++pixel_index) {
            sum.val[pixel_index] = gray_image.at<uchar>(r, c) - mean.val[pixel_index];
          }

          for (size_t pixel_index = 0; pixel_index < 3; ++pixel_index) {
            kurt.val[pixel_index] += pow(sum.val[pixel_index], 4);
          }

          for (size_t pixel_index = 0; pixel_index < 3; ++pixel_index) {
            den.val[pixel_index] += sum.val[pixel_index] * sum.val[pixel_index];
          }
        }
      }

      const int N = image.rows * image.cols;

      kurt.val[0] = (kurt.val[0] * N * (N + 1) * (N - 1) / (den.val[0] * den.val[0] * (N - 2) * (N - 3))) - (3 * (N - 1) * (N - 1) / ((N - 2) * (N - 3)));

      for (size_t pixel_index = 1; pixel_index < 3; ++pixel_index) {
        kurt.val[pixel_index] = (kurt.val[pixel_index] * N / (den.val[pixel_index] * den.val[pixel_index])) - 3;
      }

      return kurt;
    }

    std::vector<size_t> AverageFramesMethod(const std::string &video_file_path) {
      cv::VideoCapture video_capture;

      video_capture.open(video_file_path);

      std::vector<std::vector<double> > average_intensity;

      cv::Mat video_frame;
      cv::Mat average_frame;

      // Calculate average frame
      while (video_capture.read(video_frame)) {
        if (!average_intensity.size()) {
          average_frame = video_frame.clone();
          average_intensity = std::vector<std::vector<double> >(video_frame.rows, std::vector<double>(video_frame.cols, 0));
        }

        for (int r = 0; r < video_frame.rows; ++r) {
          for (int c = 0; c < video_frame.cols; ++c) {
            average_intensity[r][c] += RGBToGrayIntensity(video_frame.at<cv::Vec3b>(r, c));
          }
        }
      }

      size_t frame_count = video_capture.get(CV_CAP_PROP_FRAME_COUNT);

      if (!frame_count) {
        std::vector<size_t> keyframes_index;
        return keyframes_index;
      }

      for (int r = 0; r < video_frame.rows; ++r) {
        for (int c = 0; c < video_frame.rows; ++c) {
          average_intensity[r][c] /= (double)frame_count;
          for (size_t pixel_index = 0; pixel_index < 3; ++pixel_index) {
            average_frame.at<cv::Vec3b>(r, c).val[pixel_index] = average_intensity[r][c] * 255.0;
          }
        }
      }

      //cv::imshow("average window", average_frame);

      // Calculate frame difference with average frame
      std::vector<std::pair<double, size_t> > frames_difference;

      size_t frame_index = 0;
      video_capture.open(video_file_path);
      while (video_capture.read(video_frame)) {
        double frame_difference = 0;
        for (int r = 0; r < video_frame.rows; ++r) {
          for (int c = 0; c < video_frame.cols; ++c) {
            frame_difference += average_intensity[r][c] - RGBToGrayIntensity(video_frame.at<cv::Vec3b>(r, c));
          }
        }
        frames_difference.push_back(std::make_pair(frame_difference, frame_index++));
      }

      std::sort(frames_difference.begin(), frames_difference.end());

      std::set<size_t> target_frames_index;

      target_frames_index.insert(frames_difference.front().second);
      target_frames_index.insert(frames_difference[frames_difference.size() / 2].second);
      target_frames_index.insert(frames_difference.back().second);

      std::vector<size_t> keyframes_index;

      for (size_t frame_index : target_frames_index) {
        keyframes_index.push_back(frame_index);
      }

      return keyframes_index;
    }

    std::vector<size_t> StatisticalMethod(const std::string &video_file_path) {
      cv::VideoCapture video_capture;

      video_capture.open(video_file_path);

      cv::Mat video_frame;

      std::vector<std::pair<double, size_t> > frames_kurtosis;

      size_t frame_index = 0;

      while (video_capture.read(video_frame)) {
        cv::Scalar kurtosis = Kurtosis(video_frame);
        //double kurtosis_value = kurtosis.val[0] * kurtosis.val[0] + kurtosis.val[1] * kurtosis.val[1] + kurtosis.val[2] * kurtosis.val[2];
        double kurtosis_value = kurtosis.val[0];
        frames_kurtosis.push_back(std::make_pair(kurtosis_value, frame_index++));
      }

      std::sort(frames_kurtosis.begin(), frames_kurtosis.end());

      std::set<size_t> target_frames_index;

      target_frames_index.insert(frames_kurtosis.front().second);
      target_frames_index.insert(frames_kurtosis[frames_kurtosis.size() / 2].second);
      target_frames_index.insert(frames_kurtosis.back().second);

      std::vector<size_t> keyframes_index;
      for (size_t frame_index : target_frames_index) {
        keyframes_index.push_back(frame_index);
      }

      return keyframes_index;
    }

  }
}
