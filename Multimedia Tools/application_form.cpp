#include "application_form.h"

namespace MultimediaTools {

  [STAThread]
  int main(array<String^> ^args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    MultimediaTools::ApplicationForm form;
    Application::Run(%form);
    return 0;
  }

  ApplicationForm::ApplicationForm() {
    InitializeComponent();

    open_images_tool_strip_menu_item_->Click += gcnew System::EventHandler(this, &MultimediaTools::ApplicationForm::OnButtonsClick);
    open_videos_tool_strip_menu_item_->Click += gcnew System::EventHandler(this, &MultimediaTools::ApplicationForm::OnButtonsClick);

    select_keyframes_button_->Click += gcnew System::EventHandler(this, &MultimediaTools::ApplicationForm::OnButtonsClick);
    edit_videos_button_->Click += gcnew System::EventHandler(this, &MultimediaTools::ApplicationForm::OnButtonsClick);
    stitch_button_->Click += gcnew System::EventHandler(this, &MultimediaTools::ApplicationForm::OnButtonsClick);

    flip_check_box_->CheckedChanged += gcnew System::EventHandler(this, &MultimediaTools::ApplicationForm::OnCheckedChanged);
  }

  void ApplicationForm::OnButtonsClick(System::Object ^sender, System::EventArgs ^e) {
    if (sender == open_images_tool_strip_menu_item_) {
      OpenFileDialog ^open_images_file_dialog = gcnew OpenFileDialog();
      open_images_file_dialog->Multiselect = true;
      open_images_file_dialog->Filter = "image files|*.*";
      open_images_file_dialog->Title = "Open image files.";

      if (open_images_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        source_images_file_path.clear();
        source_images_file_name.clear();

        source_images_file_directory = msclr::interop::marshal_as<std::string>(System::IO::Path::GetDirectoryName(open_images_file_dialog->FileName)) + "\\";

        for each (System::String ^raw_file_path in open_images_file_dialog->FileNames) {
          std::string file_path = msclr::interop::marshal_as<std::string>(raw_file_path);
          source_images_file_path.push_back(file_path);

          std::string file_name = msclr::interop::marshal_as<std::string>(System::IO::Path::GetFileNameWithoutExtension(raw_file_path));
          source_images_file_name.push_back(file_name);

          std::string file_extension = msclr::interop::marshal_as<std::string>(System::IO::Path::GetExtension(raw_file_path));
          source_images_file_extension.push_back(file_extension);
        }
      }
    } else if (sender == open_videos_tool_strip_menu_item_) {
      OpenFileDialog ^open_videos_file_dialog = gcnew OpenFileDialog();
      open_videos_file_dialog->Multiselect = true;
      open_videos_file_dialog->Filter = "video files|*.*";
      open_videos_file_dialog->Title = "Open video files.";

      if (open_videos_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        source_videos_file_path.clear();
        source_videos_file_name.clear();

        source_videos_file_directory = msclr::interop::marshal_as<std::string>(System::IO::Path::GetDirectoryName(open_videos_file_dialog->FileName)) + "\\";

        for each (System::String ^raw_file_path in open_videos_file_dialog->FileNames) {
          std::string file_path = msclr::interop::marshal_as<std::string>(raw_file_path);
          source_videos_file_path.push_back(file_path);

          std::string file_name = msclr::interop::marshal_as<std::string>(System::IO::Path::GetFileNameWithoutExtension(raw_file_path));
          source_videos_file_name.push_back(file_name);

          std::string file_extension = msclr::interop::marshal_as<std::string>(System::IO::Path::GetExtension(raw_file_path));
          source_videos_file_extension.push_back(file_extension);
        }
      }
    } else if (sender == select_keyframes_button_) {
      for (size_t i = 0; i < source_videos_file_path.size(); ++i) {
        std::vector<size_t> keyframes_index;
        //keyframes_index = AutomaticExtractionOfRepresentativeKeyframesBasedOnSceneContent::AverageFramesMethod(source_videos_file_path[i]);
        keyframes_index = AutomaticExtractionOfRepresentativeKeyframesBasedOnSceneContent::StatisticalMethod(source_videos_file_path[i]);
        cv::Mat video_frame;

        cv::VideoCapture video_capture(source_videos_file_path[i]);
        video_capture.read(video_frame);

        const cv::Rect LEFT_ROI(0, 0, video_frame.cols / 2, video_frame.rows);
        const cv::Rect RIGHT_ROI(video_frame.cols / 2, 0, video_frame.cols / 2, video_frame.rows);

        const std::string KEYFRAMES_DIRECTORY = source_videos_file_directory + source_videos_file_name[i] + "\\";
        // Delete old data
        system(("if exist \"" + KEYFRAMES_DIRECTORY + "\" rmdir \"" + KEYFRAMES_DIRECTORY + "\" /s /q").c_str());
        system(("mkdir \"" + KEYFRAMES_DIRECTORY + "\"").c_str());

        for (size_t i = 0; i < keyframes_index.size(); ++i) {
          video_capture.set(CV_CAP_PROP_POS_FRAMES, keyframes_index[i]);
          video_capture.read(video_frame);

          cv::imwrite(KEYFRAMES_DIRECTORY + "L_" + std::to_string(keyframes_index[i]) + ".png", video_frame(LEFT_ROI));
          cv::imwrite(KEYFRAMES_DIRECTORY + "R_" + std::to_string(keyframes_index[i]) + ".png", video_frame(RIGHT_ROI));
        }

        std::cout << "Done : " << source_videos_file_path[i] << "\n";
      }

      std::cout << "Everything done\n";

    } else if (sender == edit_videos_button_) {
      const size_t NEW_WIDTH = System::Decimal::ToInt32(width_numeric_up_down_->Value);
      const size_t NEW_HEIGHT = System::Decimal::ToInt32(height_numeric_up_down_->Value);
      const size_t START_FRAME = System::Decimal::ToInt32(begin_frame_numeric_up_down_->Value);
      const size_t END_FRAME = System::Decimal::ToInt32(end_frame_numeric_up_down_->Value);

      for (size_t i = 0; i < source_videos_file_path.size(); ++i) {
        cv::VideoCapture video_capture(source_videos_file_path[i]);

        cv::Mat video_frame;

        const size_t FRAME_WIDTH = video_capture.get(CV_CAP_PROP_FRAME_WIDTH);
        const size_t FRAME_HEIGHT = video_capture.get(CV_CAP_PROP_FRAME_HEIGHT);
        const size_t FRAME_COUNT = video_capture.get(CV_CAP_PROP_FRAME_COUNT);

        cv::Size new_size(NEW_WIDTH ? NEW_WIDTH : FRAME_WIDTH, NEW_HEIGHT ? NEW_HEIGHT : FRAME_HEIGHT);

        const int FLIP_CODE = System::Decimal::ToInt32(flip_code_numeric_up_down_->Value);

        if (transpose_check_box_->Checked) {
          new_size = cv::Size(new_size.height, new_size.width);
        }

        const std::string edited_video_path = source_videos_file_directory + source_videos_file_name[i] + "(" + std::to_string(new_size.width) + "x" + std::to_string(new_size.height) + "_" + std::to_string(START_FRAME) + "-" + std::to_string(END_FRAME ? END_FRAME : FRAME_COUNT) + ")" + source_videos_file_extension[i];
        cv::VideoWriter edited_video_writer;

        // Temp solution
        const int FOURCC = video_capture.get(CV_CAP_PROP_FOURCC);
        edited_video_writer.open(edited_video_path, (FOURCC == CV_FOURCC('H', '2', '6', '4')) ? 0 : FOURCC, video_capture.get(CV_CAP_PROP_FPS), new_size);

        size_t current_frame_count = 0;
        while (video_capture.read(video_frame)) {
          if (END_FRAME && current_frame_count >= END_FRAME) {
            break;
          }

          if (current_frame_count >= START_FRAME) {

            if (transpose_check_box_->Checked) {
              cv::transpose(video_frame, video_frame);
            }

            if (flip_check_box_->Checked) {
              cv::flip(video_frame, video_frame, FLIP_CODE);
            }

            cv::Mat edited_frame;
            cv::resize(video_frame, edited_frame, new_size);
            edited_video_writer.write(edited_frame);

          }
          ++current_frame_count;
        }

        std::cout << "Done : " << edited_video_path << "\n";
      }

      std::cout << "Everything done\n";

    } else if (sender == stitch_button_) {
      OpenFileDialog ^open_image_files_dialog = gcnew OpenFileDialog();
      open_image_files_dialog->Multiselect = true;
      open_image_files_dialog->Filter = "image files|*.*";
      open_image_files_dialog->Title = "Open image files.";

      //std::vector<cv::cuda::GpuMat> gpu_images;
      std::vector<cv::Mat> images;

      for (size_t i = 0; i < source_images_file_path.size(); ++i) {
        cv::Mat image = cv::imread(source_images_file_path[i]);
        //cv::cuda::GpuMat gpu_image;
        //gpu_image.upload(image);
        //gpu_images.push_back(gpu_image);
        images.push_back(image);
      }
      cv::Mat pano;

      //cv::cuda::GpuMat gpu_pano;
      cv::Stitcher stitcher = cv::Stitcher::createDefault(true);
      //stitcher.setWarper(new cv::StereographicWarper());
      //stitcher.setSeamEstimationResol(0.1);
      //cv::Stitcher::Status stitching_status = stitcher.estimateTransform(gpu_images);
      cv::Stitcher::Status stitching_status = stitcher.estimateTransform(images);
      if (stitching_status != cv::Stitcher::OK) {
        std::cout << "Stitch failed, error code = " << int(stitching_status) << "\n";
      }
      //stitching_status = stitcher.composePanorama(gpu_images, gpu_pano);
      stitching_status = stitcher.composePanorama(images, pano);
      if (stitching_status != cv::Stitcher::OK) {
        std::cout << "Stitch failed, error code = " << int(stitching_status) << "\n";
      }

      //gpu_pano.download(pano);
      //cv::imshow("Pano", pano);
      cv::imwrite(source_images_file_directory + "Pano.jpg", pano);

      std::cout << "Everything done\n";
    }
  }

  void ApplicationForm::OnCheckedChanged(System::Object ^sender, System::EventArgs ^e) {
    if (sender == flip_check_box_) {
      flip_code_numeric_up_down_->Enabled = flip_check_box_->Checked;
    }
  }
}
